// indexer.go
package main

import (
	"fmt"
	"strings"
)

type Document struct {
	ID   int
	Text string
}

type Index struct {
	InvIndex map[string][]int
	Docs     map[int]string
}

func NewIndex() *Index {
	return &Index{
		InvIndex: make(map[string][]int),
		Docs:     make(map[int]string),
	}
}

func (idx *Index) Add(doc Document) {
	idx.Docs[doc.ID] = doc.Text
	words := strings.Fields(strings.ToLower(doc.Text))
	for _, word := range words {
		idx.InvIndex[word] = append(idx.InvIndex[word], doc.ID)
	}
}

func (idx *Index) Search(query string) []int {
	words := strings.Fields(strings.ToLower(query))
	var results []int
	for _, word := range words {
		results = append(results, idx.InvIndex[word]...)
	}
	return results
}

func main() {
	index := NewIndex()

	// Add documents
	index.Add(Document{ID: 1, Text: "Python crawler for NLP tasks"})
	index.Add(Document{ID: 2, Text: "Go backend for search engine indexing"})
	index.Add(Document{ID: 3, Text: "C++ high-performance algorithms"})

	// Search query
	query := "NLP"
	results := index.Search(query)

	fmt.Printf("Results for query '%s': %v\n", query, results)
}
