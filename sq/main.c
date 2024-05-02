#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifndef _INC_STDIO
# define _INC_STDIO
#endif

#include "../kollektions/kallok/kallok.h"

#include "sq.h"
#include "../libsq/sequencia.h"
#include "../libsqanalysis/analysis.h"
#include "../packedcdoc/rt/rt.h"

#define CLI_IMPL
#include "../minilibs/cli.h"
#include "../minilibs/filelib.h"
#include "../minilibs/utils.h"

static const char *flag(const int argc, char **argv, const char *name1, const char *name2, const char *defaul) {
    Flag flag = getFlag(argc, argv, name1);
    if (!flagExist(flag))
        flag = getFlag(argc, argv, name2);
    const char *str = defaul;
    if (flag.str != NULL)
        str = flag.str[1];
    return str;
}

static void normal(char *str, const SQCommand cmd, const bool debugOutput) {
    SQValue res = sqexecs(SQVAL_STR(zdupc(str)), cmd);
    sqoutput(&res, stdout, debugOutput, false, 0);
    fputc('\n', stdout);
}

#ifndef USE_NOTCURSES
# warning Not using notcurses.
#endif

#ifdef USE_NOTCURSES
# include <notcurses/notcurses.h>

static const char *run_selector(struct notcurses* nc, struct ncselector* ns) {
    static int item = 1;
    if (ns == NULL){
        notcurses_stop(nc);
        fprintf(stderr, "Error creating selector %d\n", item);
        exit(1);
    }
    notcurses_render(nc);
    uint32_t keypress;
    ncinput ni;
    while ((keypress = notcurses_get_blocking(nc, &ni)) != (uint32_t)-1) {
        if (!ncselector_offer_input(ns, &ni)) {
            if (ni.evtype == NCTYPE_RELEASE) {
                continue;
            }
            switch(keypress) {
                case NCKEY_ENTER: {
                    const char *selected = strdup(ncselector_selected(ns));
                    ncselector_destroy(ns, NULL);
                    return selected;
                }

                case 'M':
                case 'J': {
                    if (ncinput_ctrl_p(&ni)) {
                        ncselector_destroy(ns, NULL);
                        return NULL;
                    }
                }
            }

            if (keypress == 'q') {
                break;
            }
        }
        notcurses_render(nc);
    }
    ncselector_destroy(ns, NULL);
    return NULL;
}
#endif

static void lsdoc() {
    DocEntries entries = getDocEntries();
#ifdef USE_NOTCURSES
    struct ncselector_item *items = malloc(sizeof(struct ncselector_item) * (entries.len + 1));
    for (size_t i = 0; i < entries.len; i ++) {
        items[i].option = entries.items[i].name;
        items[i].desc = NULL;
    }
    items[entries.len] = (struct ncselector_item) { NULL, NULL };

    notcurses_options opts = { 0 };
    opts.loglevel = NCLOGLEVEL_SILENT;
    opts.flags = NCOPTION_SUPPRESS_BANNERS;
    struct notcurses* nc = notcurses_init(&opts, NULL);
    if (nc == NULL)
        exit(1);
    notcurses_mice_enable(nc, NCMICE_BUTTON_EVENT);
    ncselector_options sopts;
    memset(&sopts, 0, sizeof(sopts));
    sopts.items = items;
    sopts.title = "Sequencia Documentation Topics";
    sopts.footer = "press q to exit";
    sopts.defidx = 1;
//    sopts.boxchannels = NCCHANNELS_INITIALIZER(0x20, 0xe0, 0x40, 0x20, 0x20, 0x20);

    sopts.opchannels = NCCHANNELS_INITIALIZER(0xFF, 0xFF, 0xFF, 0, 0, 0); 
/*    sopts.descchannels = NCCHANNELS_INITIALIZER(0x80, 0xe0, 0x40, 0, 0, 0);
    sopts.footchannels = NCCHANNELS_INITIALIZER(0xe0, 0, 0x40, 0x20, 0, 0);
    sopts.titlechannels = NCCHANNELS_INITIALIZER(0xff, 0xff, 0x80, 0, 0, 0x20);
    uint64_t bgchannels = NCCHANNELS_INITIALIZER(0, 0x20, 0, 0, 0x20, 0);
    ncchannels_set_fg_alpha(&bgchannels, NCALPHA_BLEND);
    ncchannels_set_bg_alpha(&bgchannels, NCALPHA_BLEND);
*/
    struct ncplane* n = notcurses_stdplane(nc);
    //ncplane_set_fg_rgb(n, 0x40f040);
    struct ncplane_options nopts = {
        .y = 3,
        .x = 0,
        .rows = 1,
        .cols = 1,
        .userptr = NULL,
        .name = NULL,
        .resizecb = NULL,
        .flags = 0,
    };
    struct ncplane* seln = ncplane_create(n, &nopts);
    struct ncselector* ns = ncselector_create(seln, &sopts);
    const char *selected = run_selector(nc, ns);

    if (notcurses_stop(nc))
        exit(1);

    if (selected != NULL) {
        DocEntry *entry = findDocEntry(selected);
        if (entry == NULL)
            assert(false);
        puts(md_render(entry->text, 80));
    }
#else
    for (size_t i = 0; i < entries.len; i ++)
        printf("\"%s\"\n", entries.items[i].name);
#endif
}

Ally gAlloc;
static struct AllyStats allocStats;

int main(const int argc, char **argv) {
    if (flagExist(getFlag(argc, argv, "--help")) ||
        flagExist(getFlag(argc, argv, "-h")) || argc == 1) {
        printf("\"%s\" - Sequencia utility interpreter\n", argv[0]);
        printf("  -S  --file [path]     REQUIRED: The path to a sequence script source file\n");
        printf("  -s  --script [script] ^ ALTERNATIVE: The script to evaluate\n");
        printf("  -I  --input [path]    The input file for data; default: \"-\"\n");
        printf("  -d  --debug-output    Enable debug output (ignored with interactive debugger)\n");
        printf("  -g  --debugger        Start interactive debugger; Requires \"-I\"!\n");
        printf("  -h  --help            Show this help message\n");
        printf("      --doc (topic)     Print out the documentation for the given topic OR list all topics available\n");
        printf("      --stats           Print allocation statistics and similar\n");
        printf("      --analyze         Run code analyzer\n");
        printf("\n");
        return 0;
    }

    bool printAllocStats = flagExist(getFlag(argc, argv, "--stats"));
    gAlloc = getLIBCAlloc();
    if (printAllocStats) {
        gAlloc = getStatAlloc(gAlloc, &allocStats);
    }

    Flag docFlag = getFlag(argc, argv, "--doc");
    if (flagExist(docFlag)) {
        if (docFlag.len == 1) {
            lsdoc();
            return 0;
        }
        if (docFlag.len != 2) {
            fprintf(stderr, "\"%s\" --doc (name)\n", argv[0]);
            return 1;
        }
        DocEntry *entry = findDocEntry(docFlag.str[1]);
        if (entry == NULL) {
            fprintf(stderr, "Topic not found!\n");
            return 1;
        }
        puts(md_render(entry->text, 80));
        return 0;
    }

    char *scriptCode;
    {
        const char *scriptFile = flag(argc, argv, "-S", "--file", NULL);
        if (scriptFile != NULL) {
            FILE *f = fopen(scriptFile, "r");
            if (f == NULL) {
                fprintf(stderr, "Script file not found!\n");
                return 1;
            }
            scriptCode = readFile(f);
            fclose(f);
            if (scriptCode == NULL) {
                fprintf(stderr, "Error reading script file!\n");
                return 1;
            }
        } else {
            const char *f = flag(argc, argv, "-s", "--script", NULL);
            if (f == NULL) {
                fprintf(stderr, "Either script code or script file needs to be specified!\n");
                return 1;
            }
            scriptCode = strdup(f);
        }
    }

    if (flagExist(getFlag(argc, argv, "--analyze"))) {
        SQAScript script;
        struct DynamicList lines;
        DynamicList_init(&lines, sizeof(const char *), gAlloc, 0);
        SPLITERATE(scriptCode, "\n", line) {
            char *copy = strdup(line);
            DynamicList_add(&lines, &copy);
        }
        script.lines = lines.fixed;
        
        SQAErrors errors;
        DynamicList_init(&errors, sizeof(SQAError), gAlloc, 0);

        sqa_analyze(script, &errors);

        for (size_t i = 0; i < errors.fixed.len; i ++) {
            SQAError err = *(SQAError*)FixedList_get(errors.fixed, i);
            if (err.line >= lines.fixed.len)
                continue;
            const char *line = *(const char**)FixedList_get(lines.fixed, err.line);
            fprintf(stderr, "Error (:%zu): %s\n%s\n", err.line, err.msg, line);
            if (err.col <= strlen(line)) {
                for (size_t i = 0; i < err.col; i ++)
                    fputc(' ', stderr);
                if (err.len <= strlen(line)) {
                    for (size_t i = 0; i < err.len; i ++)
                        fputc('^', stderr);
                } else {
                    fputc('^', stderr);
                }
                fputc('\n', stderr);
            }
            fputc('\n', stderr);
        }

        return errors.fixed.len > 0 ? 1 : 0;
    }

    SQCommand cmd = sqparseheap(scriptCode);

    const bool debugOutput = flagExist(getFlag(argc, argv, "-d")) ||
                             flagExist(getFlag(argc, argv, "--debug-output"));

    const bool debugger = flagExist(getFlag(argc, argv, "-g")) ||
                          flagExist(getFlag(argc, argv, "--debugger"));

    const char *input = flag(argc, argv, "-I", "--input", "-");
    FILE *f;
    if (strcmp(input, "-") == 0) {
        if (debugger) {
            fprintf(stderr, "Interactive debugger requires input file to be set!\n");
            return 1;
        }
        f = stdin;
    } else {
        f = fopen(input, "r");
        if (f == NULL) {
            fprintf(stderr, "Could not open input file!\n");
            return 1;
        }
    }

    char *str = readFile(f);
    if (str == NULL)
        return 1;

    if (f != stdin)
        fclose(f);

    gDebug = debugger;
    normal(str, cmd, debugOutput);

    if (printAllocStats) {
        outputStats(&allocStats, stdout);
    }

    return 0;
}
