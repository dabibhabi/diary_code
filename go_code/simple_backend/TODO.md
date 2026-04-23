# TODO

## Core
- [ ] Replace `MemoryStore` with a real DB (Postgres via `database/sql` + `pgx`, or sqlite for local dev)
- [ ] Add migrations (goose or golang-migrate) and a `migrations/` directory
- [ ] Replace the hand-rolled sequential ID in `store.go` with UUIDs (`github.com/google/uuid`)
- [ ] Add an `UpdateItem` (PUT/PATCH) handler and store method
- [ ] Add request validation (e.g. `go-playground/validator`) beyond the manual empty-string check

## HTTP layer
- [ ] Logging middleware (method, path, status, latency)
- [ ] Panic-recovery middleware
- [ ] Request ID middleware + structured logs (`log/slog`)
- [ ] CORS middleware if a browser client will call this
- [ ] Read/write/idle timeouts on `http.Server` (don't use `ListenAndServe` bare in prod)
- [ ] Graceful shutdown on SIGINT/SIGTERM with `server.Shutdown(ctx)`

## Config
- [ ] Move config into a `internal/config` package — parse env once, pass the struct down
- [ ] Support a `.env` file for local dev (or just document required env vars)

## Testing
- [ ] Unit tests for `store` (table-driven)
- [ ] Handler tests using `httptest.NewRecorder` + `httptest.NewRequest`
- [ ] Integration test that boots the real mux and hits it end-to-end

## Observability
- [ ] `/metrics` endpoint (Prometheus) or OpenTelemetry exporter
- [ ] Structured logging with `slog` everywhere (no `log.Printf`)

## Delivery
- [ ] `Dockerfile` (multi-stage, distroless base)
- [ ] `Makefile` or `justfile` with `run`, `test`, `lint`, `build` targets
- [ ] CI: `go vet`, `go test ./...`, `staticcheck`, `golangci-lint`

## Stretch
- [ ] Auth (middleware that validates a bearer token or session cookie)
- [ ] Rate limiting (`golang.org/x/time/rate`)
- [ ] OpenAPI spec + generated client/server types
