package main

import (
	"context"
	"errors"
	"flag"
	"fmt"
	"log"
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
    documents := map[defines.DocumentUri][]string{}

	server.OnHover(func(ctx context.Context, req *defines.HoverParams) (result *defines.Hover, err error) {
        word := GetWord(documents[req.TextDocument.Uri], req.Position)
        if word == "" {
            return nil, nil
        }

        //op, ok := operations[word]
        //if !ok {
        //    return nil, nil
        //}

		return &defines.Hover{Contents: defines.MarkupContent{Kind: defines.MarkupKindMarkdown, Value: word}}, nil
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

    server.OnDidOpenTextDocument(func(ctx context.Context, req *defines.DidOpenTextDocumentParams) (err error) {
        documents[req.TextDocument.Uri] = strings.Split(req.TextDocument.Text, "\n")
        return nil
    })

    server.OnDidCloseTextDocument(func(ctx context.Context, req *defines.DidCloseTextDocumentParams) (err error) {
        delete(documents, req.TextDocument.Uri)
        return nil
    })

    server.OnDidChangeTextDocument(func(ctx context.Context, req *defines.DidChangeTextDocumentParams) (err error) {
        doc, ok := documents[req.TextDocument.Uri]
        if !ok {
            return errors.New("Document never opened!")
        }
        documents[req.TextDocument.Uri] = ApplyChanges(doc, req.ContentChanges)
        return nil
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
        tr := true
        syk := defines.TextDocumentSyncKindFull;
        s.Capabilities.TextDocumentSync = &defines.TextDocumentSyncOptions{
            OpenClose: &tr,
            Change: &syk, 
        }
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

    // Search left for the beginning of the word
    start := pos.Character
    for start > 0 && line[start-1] != ' ' {
        start--
    }

    // Search right for the end of the word
    end := pos.Character
    for int(end) < len(line) && line[end] != ' ' {
        end++
    }

    return line[start:end]
}

func ApplyChanges(file []string, changes []defines.TextDocumentContentChangeEvent) []string {
    for _, change := range changes {
        file = strings.Split(change.Text.(string), "\n")
    }

    return file
}

func main() {
    runLsp()
}
