# L02 writeup

## Host
- CPU model:
- N paths simulated:
- Wall time:

## Top 5 leaves (from flame graph)
| # | function | % CPU time |
|---|---|---|
| 1 |  |  |
| 2 |  |  |
| 3 |  |  |
| 4 |  |  |
| 5 |  |  |

## Where I expected the time to be vs where it actually was

## What I'd try next
- Replace the inverse-CDF with Box-Muller and re-flame
- Vectorize with AVX2 and re-flame
- Try `perf record --call-graph dwarf` if frame pointers were unreliable
