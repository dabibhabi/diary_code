# CUNY Course Catalog Data

Course-catalog data for three CUNY campuses (CCNY, Brooklyn College, Hunter College),
across four departments (Math, CS, Statistics, Physics). Twelve TSV files total,
all fetched on **2026-05-09**.

Each TSV has a `course_id<TAB>level<TAB>title<TAB>credits` schema. Lines starting with
`#` are comments. Credits are integers when known; `?` indicates the source did not
list credits (these were never invented). Only undergraduate courses (100-499) are
included; 500-level (graduate) courses are filtered out.

## File Index

| File | Source URL(s) | Courses | Caveats |
|------|---------------|--------:|---------|
| `ccny_math.tsv` | https://math.sci.ccny.cuny.edu/courses/ | 61 | Source page lists only course numbers + titles; **all credits are `?`**. |
| `ccny_cs.tsv` | https://ccny-undergraduate.catalog.cuny.edu/departments/CSC-CTY/courses (JS-rendered, returned skeleton) plus https://ccny-undergraduate.catalog.cuny.edu/courses/<id> hits via web search | 22 | **PARTIAL.** Catalog page is JS-rendered and could not be scraped; list compiled from web-search hits against course-detail pages. Likely missing some upper-level electives. Credits = standard 3 cr verified from search snippets. |
| `ccny_stats.tsv` | https://math.sci.ccny.cuny.edu/courses/ | 11 | CCNY has no separate Statistics dept; this is a filter of `ccny_math.tsv` (titles containing Statistics / Probability / Stochastic). All credits `?`. |
| `ccny_physics.tsv` | https://www.ccny.cuny.edu/physics/ugcourses (HTTP 403) and https://ccny-undergraduate.catalog.cuny.edu/departments/PHY-CTY/courses (JS-rendered) | 11 | **PARTIAL.** Both candidate sources blocked direct extraction. List compiled from CCNY syllabi pages and search-result snippets; almost certainly missing a number of upper-division and lab courses. Credits "?" where unverified. |
| `brooklyn_math.tsv` | https://websql.brooklyn.cuny.edu/courses/acad/courses_list.jsp?div=U&disc=MATH. and https://www.brooklyn.edu/wp-content/uploads/2023-2024-Undergraduate-Bulletin.pdf | 63 | Course list from inventory page; credits parsed from the 2023-24 Undergraduate Bulletin PDF. A few special-topics / independent-study courses still have `?` credits. |
| `brooklyn_cs.tsv` | https://brooklyncisdept.github.io/brochures/UndergradContent/courses.html and https://websql.brooklyn.cuny.edu/courses/acad/courses_list.jsp?div=U&disc=CISC. | 90 | Brooklyn's CS dept uses the `CISC` prefix. Both sources agreed on titles; credits taken from the dept brochure. |
| `brooklyn_stats.tsv` | https://websql.brooklyn.cuny.edu/courses/acad/courses_list.jsp?div=U&disc=MATH. + 2023-24 Bulletin | 8 | Brooklyn has no separate Statistics dept; this is a filter of `brooklyn_math.tsv` (titles containing Statistics / Probability / Time Series / Biostatistics). |
| `brooklyn_physics.tsv` | https://websql.brooklyn.cuny.edu/courses/acad/courses_list.jsp?div=U&disc=PHYS. and https://www.brooklyn.edu/wp-content/uploads/2023-2024-Undergraduate-Bulletin.pdf | 43 | Inactive courses (e.g. PHYS 1010, 1020, 3350, 3760, 3970, 4950) have `?` credits because the bulletin marks them inactive without listing credits. |
| `hunter_math.tsv` | https://hunter.catalog.acalog.com/content.php?catoid=48&navoid=15944&expand=1 | 49 | Clean acalog data with credits. |
| `hunter_cs.tsv` | https://www.cs.hunter.cuny.edu/courses.html | 30 | Hunter's CS dept publishes courses with 3-digit numbers (e.g. `CSCI 127`); the central Hunter catalog uses 5-digit forms (`CSCI 12700`). Per the spec ("preserve the campus's spacing/formatting"), the dept-page 3-digit form is preserved. |
| `hunter_stats.tsv` | https://hunter.catalog.acalog.com/content.php?catoid=48&navoid=15944&expand=1 | 17 | Hunter has a true STAT prefix shared with the Math dept; this is the STAT subset, not a filter. |
| `hunter_physics.tsv` | https://hunter.catalog.acalog.com/content.php?catoid=48&navoid=15946&print=&expand=1 | 47 | Includes ASTRO-prefixed astronomy courses, which are administered by the Physics & Astronomy dept. Some upper-level engineering-physics and advanced research courses have `?` credits where the source listed `Not specified` or a range like `1-3`. |

## Cross-listing decisions

* **Statistics treated as a filter of Math** at CCNY and Brooklyn (no separate stats
  dept). The `*_stats.tsv` files for those campuses contain a single comment header
  noting this and then list only the relevant Math courses.
* **Hunter has a true STAT prefix** maintained by the same Math & Stats dept.
  `hunter_stats.tsv` is the STAT subset of that dept; `hunter_math.tsv` is the MATH
  subset. They do not duplicate.
* **Astronomy at Hunter** is administered by the "Physics and Astronomy" dept and
  uses the ASTRO prefix. ASTRO courses are included in `hunter_physics.tsv` since
  they are part of the same department; they are not duplicated anywhere else.

## Successful files

`ccny_math.tsv`, `ccny_stats.tsv`, `brooklyn_math.tsv`, `brooklyn_cs.tsv`,
`brooklyn_stats.tsv`, `brooklyn_physics.tsv`, `hunter_math.tsv`, `hunter_stats.tsv`,
`hunter_cs.tsv`, `hunter_physics.tsv`.

## Partial files

* `ccny_cs.tsv` - CCNY catalog is JS-rendered; could not be scraped. Compiled
  partial list from search-result hits.
* `ccny_physics.tsv` - CCNY physics page returned HTTP 403; catalog is also
  JS-rendered. Compiled partial list from syllabi pages and search-result hits.

## Unavailable files

None. All 12 files were produced.
