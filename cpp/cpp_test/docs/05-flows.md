# 05 — Sequence Flows

End-to-end runtime behavior: enrollment, attendance recording, waitlist promotion. Three sequence diagrams. Async arrows use `--)` (mermaid's correct syntax — the earlier draft used the invalid `-)` form).

## Enrollment flow — happy path + waitlist branch

```mermaid
sequenceDiagram
    actor Student
    participant ES as EnrollmentService
    participant PG as PrerequisiteGraph
    participant CD as ConflictDetector
    participant DB as Database
    participant WLM as WaitlistManager

    Student->>ES: enroll(studentId, courseId)

    ES->>PG: canEnrollInCourse(studentId, courseId)
    PG->>PG: DFS prereq closure
    PG->>DB: getCompletedCourses(studentId)
    DB-->>PG: completed[]
    PG-->>ES: eligible? + missing[]

    alt prereqs not met
        ES-->>Student: REJECTED (missing: [...])
    else prereqs ok
        ES->>CD: hasConflict(activeEnrollments, newCourse)
        CD->>DB: getActiveEnrollments(studentId)
        DB-->>CD: enrollments[]
        CD-->>ES: ConflictResult

        alt time conflict
            ES-->>Student: REJECTED (conflict with COURSEx)
        else no conflict
            ES->>DB: getEnrolledCount(courseId)
            DB-->>ES: count, capacity

            alt count < capacity
                ES->>DB: saveEnrollment(ENROLLED, prereqsAtTime)
                ES-->>Student: ENROLLED
            else course full
                ES->>WLM: addToWaitlist(studentId, courseId)
                WLM->>DB: insertWaitlistEntry(position=N+1)
                WLM-->>ES: position
                ES-->>Student: WAITLISTED at #N+1
            end
        end
    end
```

`prereqsAtTime` (a JSON snapshot of the prereq edges at enrollment time) is persisted so a later prereq change can't retroactively invalidate an existing enrollment — see `01-overview.md` on temporal-correctness invariants.

---

## Attendance recording — dual-index update

```mermaid
sequenceDiagram
    actor Prof as Professor
    participant AS as AttendanceService
    participant CDX as byCourseDate Index
    participant SDX as byStudentDate Index
    participant DB as Database

    Prof->>AS: record(enrollmentId, courseId, studentId, date, status, minutes)
    AS->>AS: build AttendanceRecord

    par dual-index write
        AS->>CDX: byCourseDate[(courseId,date)].append(record)
        Note right of CDX: O(1)
    and
        AS->>SDX: byStudentDate[(studentId,date)].append(record)
        Note right of SDX: O(1)
    end

    AS--)DB: saveAttendanceRecord(record)  «async»
    AS-->>Prof: ok

    Note over Prof,DB: Read path — class roster on a date
    Prof->>AS: getClassAttendance(CS101, 5/9)
    AS->>CDX: lookup[(CS101, 5/9)]
    CDX-->>AS: List of Records  «O(1)»
    AS-->>Prof: roster

    Note over Prof,DB: Read path — student's day
    Prof->>AS: getStudentAttendance(johnId, 5/9)
    AS->>SDX: lookup[(johnId, 5/9)]
    SDX-->>AS: List of Records  «O(1)»
    AS-->>Prof: day's classes
```

Why two indexes instead of one + scan? Because both query patterns are equally hot in practice (the professor pulls the class roster every meeting; the student pulls their schedule every morning). A single index would force one of these paths into an O(n) scan — see `algorithms/07-attendance-indexing.md` for the load-factor and Robin-Hood-hashing analysis.

---

## Waitlist promotion on drop

```mermaid
sequenceDiagram
    actor Bob
    participant ES as EnrollmentService
    participant DB as Database
    participant WLM as WaitlistManager
    actor Carol

    Note over Bob,Carol: CS101 41/40, queue [Carol no.1, Dave no.2, Eve no.3]

    Bob->>ES: drop(bobId, CS101)
    ES->>DB: updateEnrollment(bob, DROPPED)
    ES->>WLM: promoteFromWaitlist(CS101)

    WLM->>DB: peekFirst(CS101)
    DB-->>WLM: Carol (no.1)

    WLM->>DB: tx_begin
    WLM->>DB: insertEnrollment(carol, CS101, ENROLLED)
    WLM->>DB: deleteWaitlistEntry(carol, CS101)
    WLM->>DB: renumberQueue(CS101)
    Note right of DB: Dave 2 to 1, Eve 3 to 2
    WLM->>DB: tx_commit

    WLM--)Carol: notifyEnrolled(CS101)
    WLM-->>ES: promoted=carol
    ES-->>Bob: dropped
```

The promotion is a single transaction across `enrollments` and `waitlists` — atomic, so a crash mid-promotion can't leave Carol both enrolled *and* waitlisted (which is the only invariant violation worth worrying about here). The notification is fire-and-forget (`--)`).

For the queue data-structure trade-offs (FIFO `std::deque` vs priority heap vs Fibonacci heap) see `algorithms/09-waitlist-queues.md`.
