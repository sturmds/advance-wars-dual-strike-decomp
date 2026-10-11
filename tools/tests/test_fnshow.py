#!/usr/bin/env python3
"""Check the `refs:` line of `fn.py show` (tools/fn.py refs_to).

    python3 tools/tests/test_fnshow.py

The function at main 0x0201d320 has no direct caller; one entry of the table
at main 0x021585c0 points at it (issue 0003). Reads config/ only: no build
and no ROM needed.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import fn  # noqa: E402

f = fn.find("0x0201d320")
table = fn.modules()["main"].syms[0x021585c0]
refs = fn.refs_to(f)
ok = f"{table}+0x14c (main)" in refs and not fn.callers_of(f)
print(f"refs_to {f.name}: {'ok' if ok else 'FAIL'} ({'; '.join(refs) or 'none'})")
sys.exit(0 if ok else 1)
