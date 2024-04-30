package main

/*
#include "../libsqanalysis/analysis.h"
#include "../sq/sq.h"

static SQAOp sqa_ops_get(int i) {
    return sqa_ops[i];
}
*/
import "C"

import (
	"errors"
	"strings"

	"github.com/tliron/commonlog"
	"github.com/tliron/glsp"
	protocol "github.com/tliron/glsp/protocol_3_16"
	"github.com/tliron/glsp/server"

	_ "github.com/tliron/commonlog/simple"
)

var (
	handler protocol.Handler
)

var documents = map[protocol.DocumentUri][]string{}

//export runLsp
func runLsp() {
	commonlog.Configure(1, nil)

	handler = protocol.Handler{
		Initialize:  initialize,
		Initialized: initialized,
		Shutdown:    shutdown,
		SetTrace:    setTrace,

        TextDocumentDidOpen: func(ctx *glsp.Context, req *protocol.DidOpenTextDocumentParams) (err error) {
            documents[req.TextDocument.URI] = strings.Split(req.TextDocument.Text, "\n")
            return nil
        },
        TextDocumentDidClose: func(ctx *glsp.Context, req *protocol.DidCloseTextDocumentParams) (err error) {
            delete(documents, req.TextDocument.URI)
            return nil
        },
        TextDocumentDidChange: func(ctx *glsp.Context, req *protocol.DidChangeTextDocumentParams) (err error) {
            doc, ok := documents[req.TextDocument.URI]
            if !ok {
                return errors.New("Document never opened!")
            }
            documents[req.TextDocument.URI] = ApplyChanges(doc, req.ContentChanges)
            return nil
        },

        TextDocumentHover: hover,
        TextDocumentCompletion: func(ctx *glsp.Context, req *protocol.CompletionParams) (result any, err error) {
    		d := protocol.CompletionItemKindFunction
            items := []protocol.CompletionItem{}
            i := 0
            for {
                op := C.sqa_ops_get(C.int(i))
                if op.name == nil {
                    break
                }
                name := C.GoString(op.name)
                docEntry := C.findDocEntry(op.docPage)
                doc := C.GoString(docEntry.text)

                items = append(items, protocol.CompletionItem{
                    Label:          name,
                    Kind:           &d,
                    InsertText:     &name,
                    Documentation:  doc,
                })
            }
		    return &items, nil
	    },
	}

	server := server.NewServer(&handler, "sequencia", false)

	server.RunStdio()

/*
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
*/
}

func initialize(context *glsp.Context, params *protocol.InitializeParams) (any, error) {
	capabilities := handler.CreateServerCapabilities()
    capabilities.TextDocumentSync = protocol.TextDocumentSyncKindFull
    version := "0.0.1"
	return protocol.InitializeResult{
		Capabilities: capabilities,
		ServerInfo: &protocol.InitializeResultServerInfo{
			Name:    "sequencia",
			Version: &version,
		},
	}, nil
}

func initialized(context *glsp.Context, params *protocol.InitializedParams) error {
	return nil
}

func shutdown(context *glsp.Context) error {
	protocol.SetTraceValue(protocol.TraceValueOff)
	return nil
}

func setTrace(context *glsp.Context, params *protocol.SetTraceParams) error {
	protocol.SetTraceValue(params.Value)
	return nil
}

func hover(context *glsp.Context, params *protocol.HoverParams) (*protocol.Hover, error) {
    word := GetWord(documents[params.TextDocument.URI], params.Position)
    if word == "" {
        return nil, nil
    }

    //op, ok := operations[word]
    //if !ok {
    //    return nil, nil
    //}

	return &protocol.Hover{Contents: protocol.MarkupContent{Kind: protocol.MarkupKindMarkdown, Value: word}}, nil
}

func GetWord(file []string, pos protocol.Position) string {
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

func ApplyChanges(file []string, changes []any) []string {
    for _, changen := range changes {
        change := changen.(protocol.TextDocumentContentChangeEventWhole)
        file = strings.Split(change.Text, "\n")
    }

    return file
}

func main() {}
