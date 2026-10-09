#!/usr/bin/env python3
"""Check tools/trivial.py on a few shapes.

    python3 tools/tests/test_trivial.py

Runs `trivial.py --dry-run` for the const, getter and tailcall patterns with
a small limit and fails unless each finds at least one match. It writes
nothing under src/ or config/.
"""

import re
import subprocess
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent

failed = 0
for pattern in ("const", "getter", "tailcall"):
    out = subprocess.run([sys.executable, str(TOOLS / "trivial.py"), "--dry-run", "--pattern", pattern, "--limit", "3"],
                         capture_output=True, text=True).stdout
    m = re.search(rf"^{pattern}\s+\d+\s+(\d+)", out, re.M)
    ok = bool(m) and int(m.group(1)) > 0
    print(f"{pattern}: {'ok' if ok else 'FAIL'}")
    failed += not ok
sys.exit(1 if failed else 0)
