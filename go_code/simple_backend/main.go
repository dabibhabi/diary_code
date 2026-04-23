package main

import (
	"log"
	"net/http"
	"os"

	"simple_backend/internal/handlers"
	"simple_backend/internal/store"
)

func main() {
	addr := os.Getenv("ADDR")
	if addr == "" {
		addr = ":8080"
	}

	db := store.NewMemoryStore()
	h := handlers.New(db)

	mux := http.NewServeMux()
	mux.HandleFunc("GET /healthz", h.Health)
	mux.HandleFunc("GET /items", h.ListItems)
	mux.HandleFunc("POST /items", h.CreateItem)
	mux.HandleFunc("GET /items/{id}", h.GetItem)
	mux.HandleFunc("DELETE /items/{id}", h.DeleteItem)

	log.Printf("listening on %s", addr)
	if err := http.ListenAndServe(addr, mux); err != nil {
		log.Fatal(err)
	}
}
