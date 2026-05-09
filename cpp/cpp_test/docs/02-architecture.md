# 02 — Architecture

System layering, the persistent data model, and the database abstraction. Three diagrams.

## Renderer note: mermaid + LaTeX

Several chapters (especially the math chapters 12–17) embed LaTeX in mermaid via `$$…$$`. This requires:

- mermaid **≥ 10.9.0** with KaTeX support enabled, **and**
- a viewer that runs the mermaid theme (VS Code's mermaid preview, GitHub markdown rendering, mkdocs-material with the `pymdownx.arithmatex` + mermaid plugins).

Setup snippet for a custom mkdocs config:

```yaml
markdown_extensions:
  - pymdownx.superfences:
      custom_fences:
        - name: mermaid
          class: mermaid
          format: !!python/name:pymdownx.superfences.fence_code_format
extra_javascript:
  - https://cdn.jsdelivr.net/npm/mermaid@10.9.0/dist/mermaid.min.js
```

In a label, quote with double quotes and use `$$…$$` for math:

```text
A["$$\sigma^2 = \tfrac{1}{n}\sum (x_i - \bar{x})^2$$"]
```

If the viewer doesn't support math, the formula degrades to literal text — readable but not pretty.

---

## High-level system architecture

```mermaid
graph TB
    subgraph "Clients"
        UI[Web / Mobile / Admin Portal]
    end

    subgraph "Service Layer"
        ES[EnrollmentService]
        AS[AttendanceService]
        RS[RoleService]
        MS[MemberService]
        CS[CourseService]
        WL[WaitlistManager]
    end

    subgraph "Domain Logic"
        PG[PrerequisiteGraph<br/>DFS · BFS · TopoSort]
        CD[ScheduleConflictDetector<br/>Strategy]
        RFR[RoleFactoryRegistry<br/>Abstract Factory]
        EG[EmailGenerator]
    end

    subgraph "Repository Layer"
        DBI["CunyDatabase &lt;interface&gt;"]
        IDX[Dual-Index Cache]
    end

    subgraph "Persistence"
        SQL[(SQLite / Postgres)]
        MEM[(HashMap Indexes<br/>byCourseDate · byStudentDate)]
    end

    UI --> ES & AS & RS & MS & CS
    ES --> PG & CD & WL
    ES --> DBI
    AS --> IDX
    AS --> DBI
    RS --> RFR
    MS --> EG
    RFR --> DBI
    DBI --> SQL
    IDX --> MEM
```

The four layers correspond to four directories in the C++ source tree (see `conventions/20-build-system.md`):

| Layer | Source dir | Responsibility |
|-------|-----------|----------------|
| Service | `src/services/` | Use-case orchestration; transaction boundary |
| Domain Logic | `src/domain/` | Pure business rules; no I/O |
| Repository | `src/persistence/` | Database abstraction + index maintenance |
| Persistence | bundled deps | SQLite (later) + in-process HashMaps |

---

## Physical data model — ER diagram

```mermaid
erDiagram
    PERSONS         ||--|| CUNY_MEMBERS         : "is"
    CAMPUSES        ||--o{ CUNY_MEMBERS         : "houses"
    CAMPUSES        ||--o{ COURSES              : "offers"
    CUNY_MEMBERS    ||--o{ ROLES                : "has"
    CUNY_MEMBERS    ||--o{ ENROLLMENTS          : "enrolls"
    CUNY_MEMBERS    ||--o{ WAITLISTS            : "queued in"
    COURSES         ||--o{ ENROLLMENTS          : "filled by"
    COURSES         ||--o{ WAITLISTS            : "queues for"
    COURSES         ||--o{ PREREQUISITES        : "requires"
    COURSES         ||--o{ COREQUISITES         : "co-meets"
    ENROLLMENTS     ||--o{ ATTENDANCE_RECORDS   : "tracks"

    PERSONS {
        int    id PK
        string firstName
        string lastName
        date   dob
        string ssn
    }
    CAMPUSES {
        int    id PK
        string code UK
        string name
        string emailDomain
        json   config
    }
    CUNY_MEMBERS {
        int    id PK
        int    personId FK
        int    campusId FK
        int    employeeId UK "9-digit"
        string email UK
        bool   isEPermitStudent
    }
    ROLES {
        int    id PK
        int    memberId FK
        string roleType "STUDENT|PROFESSOR|TA|ADMIN"
        string adminLevel "IT|REGISTRAR|DEPT_CHAIR|DEAN|PROVOST (when ADMIN)"
        json   roleData
        ts     createdAt
    }
    COURSES {
        string courseId PK
        int    campusId FK
        string department "MATH|CS|STATS|PHYS"
        string name
        int    capacity
        json   schedule
    }
    PREREQUISITES {
        int    id PK
        string courseId FK
        string prerequisiteId FK
        string logic "AND|OR"
        string groupKey "for OR-grouping"
    }
    COREQUISITES {
        int    id PK
        string courseId FK
        string corequisiteId FK
    }
    ENROLLMENTS {
        int    enrollmentId PK
        int    studentId FK
        string courseId FK
        string status "ENROLLED|WAITLISTED|DROPPED|COMPLETED"
        date   enrollmentDate
        string grade
        json   prereqsAtEnrollment
    }
    WAITLISTS {
        int    waitlistId PK
        string courseId FK
        int    studentId FK
        int    position
        ts     queuedAt
    }
    ATTENDANCE_RECORDS {
        int    recordId PK
        int    enrollmentId FK
        string courseId FK
        date   date
        string status "PRESENT|ABSENT|TARDY|EXCUSED"
        int    minutesAttended
        time   arrivalTime
        time   departureTime
        string notes
    }
```

Note: `COURSES.department` is restricted to `MATH | CS | STATS | PHYS` for the first build (see `01-overview.md`). `ROLES.adminLevel` is meaningful only when `roleType = ADMIN`; otherwise `NULL`.

---

## Repository pattern — database abstraction

Business logic depends on the interface, not the implementation. Phase 6 of the roadmap ships **only `InMemoryCunyDatabase`**; SQLite arrives later.

```mermaid
classDiagram
    class CunyDatabase {
        <<interface>>
        +getMemberById(id) CunyMember
        +getMemberByEmail(email) CunyMember
        +getMemberByEmployeeId(empId) CunyMember
        +saveMember(member)
        +getCompletedCourses(studentId) List~Course~
        +getActiveEnrollments(studentId) List~Enrollment~
        +saveEnrollment(enrollment)
        +saveAttendanceRecord(record)
        +getCourse(courseId) Course
        +getPrerequisites(courseId) List~PrereqEdge~
        +getWaitlist(courseId) Queue~Member~
    }

    class InMemoryCunyDatabase {
        -members: unordered_map
        -courses: unordered_map
        -enrollments: vector
    }
    class SQLiteCunyDatabase
    class PostgresCunyDatabase

    CunyDatabase <|.. InMemoryCunyDatabase
    CunyDatabase <|.. SQLiteCunyDatabase
    CunyDatabase <|.. PostgresCunyDatabase

    note for SQLiteCunyDatabase "Phase 7+ — deferred"
    note for PostgresCunyDatabase "future"
```

Why this matters: the dual-index attendance cache (see `algorithms/07-attendance-indexing.md`) is a *read* layer in front of `CunyDatabase`. Swapping in SQLite later is purely an implementation change; the cache, and every service above it, is unaffected.
