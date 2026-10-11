#!/usr/bin/env python3
"""Check that loop_state.py collect survives a RESULT line with no value.

    python3 tools/tests/test_loop_state.py

Runs cmd_collect on a throwaway LOOP_DIR. No ROM or build needed.
"""

import argparse
import io
import os
import sys
import tempfile
from contextlib import redirect_stdout
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(TOOLS))

with tempfile.TemporaryDirectory() as tmp:
    os.environ["LOOP_DIR"] = tmp
    import loop_state as ls

    (Path(tmp) / "results").mkdir()
    (Path(tmp) / "results/a.txt").write_text("RESULT:\nFUNCTION: x\n\nRESULT: matched\nLANDED: abcdef1\n")
    (Path(tmp) / "results/b.txt").write_text("RESULT: matched\nLANDED: 1234567\n")
    group = {"status": "running", "order": 0, "started": "2000-01-01 00:00", "result": ""}
    state = {"groups": {"a": dict(group), "b": dict(group, order=1)}}
    out = io.StringIO()
    with redirect_stdout(out):
        ls.cmd_collect(argparse.Namespace(), state)
    text = out.getvalue()
    failed = 0
    if state["groups"]["a"]["status"] != "partial" or state["groups"]["b"]["status"] != "done":
        print("FAIL: statuses", {k: v["status"] for k, v in state["groups"].items()})
        failed = 1
    if "no value" not in text:
        print("FAIL: empty block not flagged")
        failed = 1
    print(text, end="")
    print("test_loop_state: " + ("FAILED" if failed else "OK"))
    sys.exit(failed)
