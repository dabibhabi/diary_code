"""Make the built `nmpy` extension importable from these tests."""
from __future__ import annotations

import os
import sys
from pathlib import Path

_HERE = Path(__file__).resolve().parent
_REPO = _HERE.parent.parent
_BUILD = _REPO / "build"

# The extension lands somewhere under build/ depending on generator. Walk
# the tree once and prepend any directory that contains an `nmpy*.so`.
for so in _BUILD.rglob("nmpy*.so"):
    sys.path.insert(0, str(so.parent))
    break
else:
    if "NMPY_PATH" in os.environ:
        sys.path.insert(0, os.environ["NMPY_PATH"])
