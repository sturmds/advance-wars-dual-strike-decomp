# 0004: Identical functions each cost a full agent run

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-07
- **Milestone:** M1

## Problem

func_02003f6c and func_02003ff8 have the same body and the same relocation targets. Each one cost a full agent run (about 45k tokens). The ROM likely has many such pairs (small setters, getters, copies of inline helpers).

## Notes

- A `fn.py twins` command could hash each function's bytes with relocations masked, plus its relocation targets, and list groups.
- When one member of a group matches, the others can get the same C (with their own names) without an agent. Run `fn.py diff` to confirm each.
- Also worth listing groups that differ only in relocation targets: same C shape, different globals.

## Done when

`fn.py twins` lists identical groups, and the orchestrator handles them without starting agents.
