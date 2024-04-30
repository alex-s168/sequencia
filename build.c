#ifndef CC
# define CC        "clang"
#endif
#ifndef CC_ARGS
# define CC_ARGS   "-O2"
#endif
#ifndef CXX_ARGS
# define CXX_ARGS  "-O2"
#endif

#define VERBOSE 1

#include "build_c/build.h"

/* ========================================================================= */

enum CompileResult target_clean() {
    rmdir("build/");
    rmdir("kollektions/build/");
    rmfile("sqlsp/sqlsp.a");
    rmfile("sqlsp/sqlsp.h");
    return CR_OK;
}


/* ========================================================================= */

enum CompileResult target_deps() {
    START;
    ss("kollektions/", {
        DO(ss_task("kollektions.a"));
        DO(ss_task("kallok.a"));
        DO(ss_task("kash.a"));
        DO_IGNORE(ss_task("tests"));
    });
    ss("u-xxd/", { // needed for doc
        DO(ss_task("uxxd.exe"));
    });
    END;
}

/* ========================================================================= */

enum CompileResult target_doc_text() {
    START;
    DO(shell("CC=" CC " ./build_doc.sh"));
    ss("packedcdoc/", {
        DO(ss_task("rt/text"));
    });
    END;
}

enum CompileResult target_doc_glamour() {
    START;
    DO(shell("CC=" CC " ./build_doc.sh"));
    ss("packedcdoc/", {
        DO(ss_task("rt/glamour"));
    });
    END;
}

/* ========================================================================= */

struct CompileData target_sq_files[] = {
    DIR("build/"),

    DIR("build/sq/"),
    SP(CT_C, "sq/main.c"),
    SP(CT_C, "sq/debugger.c"),
    SP(CT_C, "sq/doc.c"),

    DIR("build/build/"),
    SP(CT_C, "build/doc_lut.c"),

    DEP("build/doc.a"),
    DEP("packedcdoc/rt/rt.a"),

    DEP("build/libsq.a"),

    DEP("kollektions/build/kollektions.a"),
    DEP("kollektions/build/kallok.a"),
    DEP("kollektions/build/kash.a"),
};

enum CompileResult target_sq() {
    START;
    DO(verifyDependencies(LI(target_sq_files)));
    DO(compile(LI(target_sq_files)));
    DO(link_exe(LI(target_sq_files), "build/sq.exe"));
    END;
}

/* ========================================================================= */

struct CompileData target_libsq_files[] = {
    DIR("build/"),

    DIR("build/libsq/"),
    SP(CT_C, "libsq/array.c"),
    SP(CT_C, "libsq/command.c"),
    SP(CT_C, "libsq/exec.c"),
    SP(CT_C, "libsq/utils.c"),
    SP(CT_C, "libsq/value.c"),

    DIR("build/libsq/exec/"),
    SP(CT_C, "libsq/exec/any.c"),
    SP(CT_C, "libsq/exec/append.c"),
    SP(CT_C, "libsq/exec/axis.c"),
    SP(CT_C, "libsq/exec/bool.c"),
    SP(CT_C, "libsq/exec/contains.c"),
    SP(CT_C, "libsq/exec/filter.c"),
    SP(CT_C, "libsq/exec/filter_chars.c"),
    SP(CT_C, "libsq/exec/flatten.c"),
    SP(CT_C, "libsq/exec/group.c"),
    SP(CT_C, "libsq/exec/join.c"),
    SP(CT_C, "libsq/exec/make.c"),
    SP(CT_C, "libsq/exec/map.c"),
    SP(CT_C, "libsq/exec/noempty.c"),
    SP(CT_C, "libsq/exec/num.c"),
    SP(CT_C, "libsq/exec/ranges.c"),
    SP(CT_C, "libsq/exec/rev.c"),
    SP(CT_C, "libsq/exec/run.c"),
    SP(CT_C, "libsq/exec/same.c"),
    SP(CT_C, "libsq/exec/select.c"),
    SP(CT_C, "libsq/exec/split.c"),
    SP(CT_C, "libsq/exec/stats.c"),
    SP(CT_C, "libsq/exec/tokens.c"),
    SP(CT_C, "libsq/exec/transform.c"),
    SP(CT_C, "libsq/exec/use.c"),
    SP(CT_C, "libsq/exec/with.c"),
    SP(CT_C, "libsq/exec/strmod.c"),
};

enum CompileResult target_libsq() {
    ONLY_IF({
        NOT_FILE("build/libsq.a");
        CHANGED("libsq/");
    });

    START;
    DO(verifyDependencies(LI(target_libsq_files)));
    DO(compile(LI(target_libsq_files)));
    DO(linkTask(LI(target_libsq_files), "build/libsq.a"));
    END;
}

/* ========================================================================= */

struct CompileData target_libsqanalysis_files[] = {
    DIR("build"),

    DIR("build/libsqanalysis"),
    SP(CT_C, "libsqanalysis/all.c"),
    SP(CT_C, "libsqanalysis/doc.c"),
    SP(CT_C, "libsqanalysis/op.c"),
    SP(CT_C, "libsqanalysis/utils.c"),
    SP(CT_C, "libsqanalysis/value.c"),
};

enum CompileResult target_libsqanalysis() {
    ONLY_IF({
        NOT_FILE("build/libsqanalysis.a");
        CHANGED("libsqanalysis/");
    });

    START;
    DO(verifyDependencies(LI(target_libsqanalysis_files)));
    DO(compile(LI(target_libsqanalysis_files)));
    DO(linkTask(LI(target_libsqanalysis_files), "build/libsqanalysis.a"));
    END;
}

/* ========================================================================= */

enum CompileResult target_sqlsp() {
    ONLY_IF({
        NOT_FILE("sqlsp/sqlsp.a");
        NOT_FILE("sqlsp/sqlsp.h");
        CHANGED("sqlsp/main.go");
    });
    return shell("cd sqlsp && ./build.sh");
}

/* ========================================================================= */

struct Target targets[] = {
    { .name = "clean",           .run = target_clean },
    { .name = "deps",            .run = target_deps },
    { .name = "doc/glamour",     .run = target_doc_glamour },
    { .name = "doc/text",        .run = target_doc_text },
    { .name = "libsq.a",         .run = target_libsq },
    { .name = "libsqanalysis.a", .run = target_libsqanalysis },
    { .name = "sqlsp",           .run = target_sqlsp },
    { .name = "sq.exe",          .run = target_sq },
};

#define TARGETS_LEN (sizeof(targets) / sizeof(targets[0]))

int main(int argc, char **argv) {
    return build_main(argc, argv, targets, TARGETS_LEN);
}
