# 0033: `loop_state.py collect` crashed on a RESULT line with no value

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-10
- **Milestone:** M1

## Problem

A results file with a bare `RESULT:` line (wrapup11.txt, line 1) made `cmd_collect` raise IndexError on `b["RESULT"].split()[0]`. Collection stopped for every group after it.

## Fix

An empty value counts as the kind `empty`, so the group is `partial` or `failed`, and collect prints one line per group naming how many blocks had no value. `tools/tests/test_loop_state.py` covers it and runs under `make test`.
