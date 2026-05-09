# 15 — Combinatorics

Counting arguments that come up in the prereq DAG, course scheduling, and section assignment.

## Linear extensions of the prereq DAG

**The question.** Given the prereq DAG, how many *valid* orderings are there in which a student could complete all courses?

**Definition.** A linear extension of a poset $$(P, \le)$$ is a total order $$\sigma: P \to \{1, \ldots, n\}$$ such that $$x \le y \implies \sigma(x) \le \sigma(y)$$. Counting them is denoted $$e(P)$$.

**Hardness.** Computing $$e(P)$$ exactly is **#P-complete** (Brightwell-Winkler, 1991). No polynomial-time algorithm is expected.

**What we can do.**

- **Sample uniformly** from linear extensions via the Karzanov-Khachiyan Markov chain (mixes in polynomial time).
- **Estimate** $$e(P)$$ via the FPRAS based on Markov-chain Monte Carlo (Bubley-Dyer 1999).
- **Bound** $$e(P)$$:
  - Trivial: $$e(P) \le n!$$ with equality iff $$P$$ is an antichain (no edges).
  - For DAGs of width $$w$$: $$e(P) \le n! / \prod_i (\text{layer sizes})$$ (loose).

**Use case.** Showing a student "here are 5 different ways to order your remaining 15 courses, sampled uniformly at random from all valid plans" is a friendlier UX than "here is *the* topological order" (Kahn's gives one, arbitrarily).

---

## Catalan numbers — valid prereq tree shapes

**Definition.**

$$C_n = \frac{1}{n+1} \binom{2n}{n} = \binom{2n}{n} - \binom{2n}{n+1}$$

Sequence: $$1, 1, 2, 5, 14, 42, 132, 429, \ldots$$.

**Where it shows up here.** $$C_n$$ counts the number of binary tree shapes on $$n$$ internal nodes — relevant if we model each AND-prereq node as a binary AND-tree. For purely-AND prereq trees with $$n$$ courses, the number of distinct tree shapes is $$C_{n-1}$$.

**Worked example.** AD401 ← (CS301, M) where M is itself a child structure. With 4 prereq leaves under one AND root, the number of binary-tree shapes is $$C_3 = 5$$.

---

## Inclusion-exclusion — OR-group satisfaction

**The question.** A course requires at least one course from each of $$k$$ OR-groups. Given a student's completed-course set, what fraction of *random students* of comparable size also satisfy the OR-groups?

**Inclusion-exclusion.** Let $$A_i$$ be the event "OR-group $$i$$ is unsatisfied". Then

$$P\bigl(\bigcap_{i} A_i^c\bigr) = \sum_{S \subseteq \{1,\ldots,k\}} (-1)^{|S|} P\bigl(\bigcap_{i \in S} A_i\bigr)$$

For independent or symmetric assumptions on the underlying course-completion distribution, the joint probabilities decouple and the sum has $$2^k$$ terms.

**Use case.** Quantify "how restrictive is this prereq policy?" — what fraction of plausible student transcripts pass it? High restriction may discourage enrollment; low restriction may admit unprepared students.

---

## Stirling numbers (second kind) — sectioning students

**Definition.** $$S(n, k)$$ counts the number of ways to partition $$n$$ labeled students into $$k$$ non-empty unlabeled sections.

$$S(n, k) = k \cdot S(n-1, k) + S(n-1, k-1)$$

Closed form:

$$S(n, k) = \frac{1}{k!} \sum_{j=0}^{k} (-1)^j \binom{k}{j} (k-j)^n$$

**Use case.** "Split 120 students into 4 lab sections of similar size" — the *number of ways* is enormous; we don't enumerate, but we use Stirling's approximation to reason about how much variation is possible. More immediately, $$\binom{n}{k}$$ counts the number of balanced section assignments (treating sections as labeled by time slot).

---

## The pigeonhole principle — capacity proofs

**The principle.** $$n$$ items in $$m$$ boxes with $$n > m$$: some box has $$\ge \lceil n/m \rceil$$ items.

**Use case.** If 250 students need CS101 and the campus offers 4 sections of 40, then $$250 > 160$$ — at least one student goes unseated. Pigeonhole *proves* the under-capacity; LP (`16-optimization.md`) tells you the *least-painful* allocation.

---

## Counting prereq closures

For the DAG with $$V$$ courses and $$E$$ edges, the *transitive closure* has at most $$\binom{V}{2}$$ ordered pairs but typically far fewer. Empirical observation: in real curriculum DAGs, the closure is roughly $$O(V \log V)$$ pairs (each course transitively requires $$O(\log V)$$ of its ancestors). This is what makes the LRU cache (`algorithms/11-caching-memoization.md`) cheap — the per-course closure list is short.

---

## Cross-references

- DAG algorithms producing one valid order — `algorithms/06-prerequisite-graph.md`
- Probability distributions over student outcomes — `13-probability.md`
- Optimization over discrete choices — `16-optimization.md`
