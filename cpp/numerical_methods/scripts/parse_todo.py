#!/usr/bin/env python3
"""Extract checked checkbox slugs from TODO.md and emit a ctest -L regex.

A "checked item" is a line of the form
    - [x] **<slug>** — description
in the project's TODO.md. We collect every slug and join them with `|`,
plus the always-on `baseline` label, producing a string suitable for
`ctest -L "<regex>"`.

Usage:
    python scripts/parse_todo.py [path/to/TODO.md]

If TODO.md cannot be parsed (or no items are checked), we still emit
`baseline` so CI runs the always-on tests.
"""
from __future__ import annotations

import re
import sys
from pathlib import Path

CHECKED = re.compile(r"^\s*-\s*\[x\]\s*\*\*([A-Za-z0-9_]+)\*\*", re.MULTILINE)


def main() -> int:
    todo_path = Path(sys.argv[1] if len(sys.argv) > 1 else "TODO.md")
    if not todo_path.exists():
        print("baseline")
        return 0

    text = todo_path.read_text(encoding="utf-8")
    slugs = sorted(set(CHECKED.findall(text)))
    labels = ["baseline", *slugs]
    # ctest -L uses POSIX regex; '^(a|b|c)$' is the safe form.
    print("^(" + "|".join(re.escape(s) for s in labels) + ")$")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
