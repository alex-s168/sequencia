package main

import (
	"context"
	"flag"
	"fmt"
	"log"
	"net/url"
	"os"
	"strings"

	"github.com/TobiasYin/go-lsp/logs"
	"github.com/TobiasYin/go-lsp/lsp"
	"github.com/TobiasYin/go-lsp/lsp/defines"
)

func strPtr(str string) *string {
	return &str
}

var logPath *string

func init() {
	var logger *log.Logger
	defer func() {
		logs.Init(logger)
	}()
	logPath = flag.String("logs", "", "logs file path")
	if logPath == nil || *logPath == "" {
		logger = log.New(os.Stderr, "", 0)
		return
	}
	p := *logPath
	f, err := os.Open(p)
	if err == nil {
		logger = log.New(f, "", 0)
		return
	}
	f, err = os.Create(p)
	if err == nil {
		logger = log.New(f, "", 0)
		return
	}
	panic(fmt.Sprintf("logs init error: %v", *logPath))
}

var operations = map[string]string{
    "any":      "any",
    "append":   "append",
    "axis":     "axis",
    "contains": "contains",
    "deshape":  "deshape",
    "falpha":   "falpha fdigit falnum fpunct fwhite",
    "fdigit":   "falpha fdigit falnum fpunct fwhite",
    "falnum":   "falpha fdigit falnum fpunct fwhite",
    "fpunct":   "falpha fdigit falnum fpunct fwhite",
    "fwhite":   "falpha fdigit falnum fpunct fwhite",
    "fnalpha":  "fnalpha fndigit fnalnum fnpunct fnwhite",
    "fndigit":  "fnalpha fndigit fnalnum fnpunct fnwhite",
    "fnalnum":  "fnalpha fndigit fnalnum fnpunct fnwhite",
    "fnpunct":  "fnalpha fndigit fnalnum fnpunct fnwhite",
    "fnwhite":  "fnalpha fndigit fnalnum fnpunct fnwhite",
    "filter":   "filter",
    "flatmap":  "flatmap",
    "flatten":  "flatten",
    "group":    "group",
    "invert":   "invert",
    "join":     "join",
    "len":      "len",
    "make":     "make",
    "map":      "map",
    "noempty":  "noempty",
    "parse":    "parse",
    "prepend":  "prepend",
    "ranges":   "ranges",
    "rev":      "rev",
    "run":      "run",
    "same":     "same",
    "select":   "select",
    "slit":     "split",
    "str":      "str",
    "tokens":   "tokens",
    "tolower":  "tolower toupper",
    "toupper":  "tolower toupper",
    "transform":"transform",
    "use":      "use",
    "with":     "with",
}

//export runLsp
func runLsp() {
	server := lsp.NewServer(&lsp.Options{})

	server.OnHover(func(ctx context.Context, req *defines.HoverParams) (result *defines.Hover, err error) {
		file, err := ReadFile(req.TextDocument.Uri)
		if err != nil {
			logs.Println("hover: failed to read req.TextDocument.Uri" + err.Error())
		}

        word := GetWord(file, req.Position)
        if word == "" {
            return nil, nil
        }

        op, ok := operations[word]
        if !ok {
            return nil, nil
        }

		return &defines.Hover{Contents: defines.MarkupContent{Kind: defines.MarkupKindMarkdown, Value: op}}, nil
	})

	server.OnCompletion(func(ctx context.Context, req *defines.CompletionParams) (result *[]defines.CompletionItem, err error) {
		d := defines.CompletionItemKindFunction
        items := make([]defines.CompletionItem, len(operations))
        i := 0
        for op, doc := range operations { 
            items[i] = defines.CompletionItem{
                Label:          op,
                Kind:           &d,
                InsertText:     strPtr(op),
                Documentation:  doc,
            }
            i += 1
        }
		return &items, nil
	})

	server.OnDidChangeWatchedFiles(func(ctx context.Context, req *defines.DidChangeWatchedFilesParams) (err error) {
		return nil
	})

    server.OnInitialized(func(ctx context.Context, req *defines.InitializeParams) (err error) {
		return nil
	})

	server.OnDidChangeConfiguration(func(ctx context.Context, req *defines.DidChangeConfigurationParams) (err error) {
		return nil
	})
    
    server.OnDidSaveTextDocument(func(ctx context.Context, req *defines.DidSaveTextDocumentParams) (err error) {
        return nil
    })

	server.OnInitialize(func(ctx context.Context, req *defines.InitializeParams) (result *defines.InitializeResult, err *defines.InitializeError) {
		s := &defines.InitializeResult{}
		s.Capabilities.HoverProvider = true
		s.Capabilities.WorkspaceSymbolProvider = true
		s.Capabilities.DocumentFormattingProvider = true
        s.Capabilities.HoverProvider = true
        s.Capabilities.CompletionProvider = &defines.CompletionOptions{}
		return s, nil
	})    

	server.Run()
}

func GetWord(file []string, pos defines.Position) string {
    if int(pos.Line) >= len(file) {
        return ""
    }
    line := file[pos.Line]
    if int(pos.Character) >= len(line) {
        return ""
    }
    sstr := line[pos.Character:]
    if len(sstr) == 0 {
        return ""
    }
    index := strings.IndexByte(sstr, ' ')
    if index < 0 {
        index = len(sstr) - 1
    }
    return sstr[:index]
}

func ReadFile(filename defines.DocumentUri) ([]string, error) {
	enEscapeUrl, _ := url.QueryUnescape(string(filename))
	data, err := os.ReadFile(enEscapeUrl[6:])
	if err != nil {
		return nil, err
	}
	content := string(data)
	line := strings.Split(content, "\n")
	return line, nil
}

func main() {
    runLsp()
}
