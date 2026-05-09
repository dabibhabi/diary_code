# 26 — Open Questions

A parking lot for decisions deferred from the first build. Re-examine when the relevant phase begins.

## Persistence

- **When does SQLite arrive?** Currently deferred until after Phase 7 (Python bindings). Trigger: when in-memory state outgrows a single process restart's memory budget, or when we need durability.
- **Postgres later?** Only if multi-process / multi-host. Don't pre-commit.
- **WAL / write-ahead log for the in-memory DB?** Adds durability without full SQLite — could be a Phase 6.5 if we want crash-recovery.

## Cross-campus

- **Can a student at CCNY take a course at Hunter?** Currently no. `CunyMember` belongs to one `Campus`. Three options if we change our minds:
  - One member, multiple campus enrollments (cleaner data, schema change).
  - Distinct member rows per campus (simpler code, identity confusion).
  - Shared "system-wide" courses (a third campus type).

## Authentication / identity

- **Who issues tokens?** Out of scope. The system models *who* you are; it doesn't *prove* it.
- **Password reset flow?** Belongs in IT-admin tooling; not in the engine.

## Notification delivery

- **Emails sent on waitlist promotion?** Currently fire-and-forget. Real delivery (SMTP, push) is an integration, not core.

## Scaling

- **What if a single course has 10k waitlisted?** FIFO `std::deque` is fine up to ~10⁶; UI will ask for paginated reads. Add when needed.
- **Distributed index for attendance?** Out of scope for in-process design. If needed: Redis cluster + consistent hashing.

## Math / analytics

- **Recommendation system?** Mutual information + course-clustering give the building blocks. Productizing them is a separate effort.
- **Predicting at-risk students?** Logistic regression on attendance + GPA + course load. Out of scope; the data model supports it.

## CI / packaging

- **Cross-platform wheels?** `cibuildwheel` for Linux/macOS/Windows. Add when there's a second user.
- **Conda packaging?** Lower priority; pip wheel covers most cases.

## Testing

- **Mutation testing?** `mutmut` for Python, `mull` or hand-rolled for C++. Worthwhile after Phase 9 hardening.
- **Fuzzing the prereq graph?** AFL++ on the deserializer once we have one. After Phase 6.

## API surface

- **GraphQL? REST?** Neither is in scope. The C++ engine + Python bindings is the only "API" today.
- **gRPC for service decomposition?** If/when this becomes a multi-process system. Not today.

---

When this list grows past ~30 items, move resolved ones to a `docs/decided.md` and keep this lean.
