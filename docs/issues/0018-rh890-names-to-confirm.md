# 0018: Names from document 890 to confirm

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-08
- **Milestone:** M2

## Problem

The review of romhacking.net document 890 (`docs/third-party/romhacking-890.md`) proposed 309 renames. The 66 "high" rows were imported on 2026-10-08 (65 applied). The other 243 are not: 50 "medium" (their asm labels and constants) and 193 "low" (names made from note text, generic helper labels such as `Damage_To_Funds_Sub2`, RAM addresses seen in an emulator).

Known wrong or partial readings from the review: `0x0208e134` is not a function start; `func_020e3110` returns "player is human" in one mode, not only "Allied Nations CO"; map header +0x56/+0x58 are CO ids, not "speed limit".

## Notes

- The plan and its sources: `work/awds-disasm-review/rename-plan.csv`, `names.csv` (outside the repo).
- Import a row only when a decompiled function confirms it, with `tools/import_names.py` on a reviewed copy, so `names-provenance.csv` records the source.
- Batch 3 agents found a few hint names narrower than the code: `UnitHighlightNonFootUnits` also requires the unit to have moved; `CheckUnitType` returns a combat class (suggested `GetUnitCombatClass`).

## Done when

Every medium and low row is imported, rewritten, or marked rejected in the reviewed plan.
