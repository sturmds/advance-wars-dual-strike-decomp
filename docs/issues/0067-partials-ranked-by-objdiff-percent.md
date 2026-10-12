# 0067: Partials are ranked by objdiff's percentage, which favours register differences over a closer match

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-12
- **Milestone:** M2

## Problem

Agents and the orchestrator keep the version of a partial with the highest `fn.py diff` percentage. That number is objdiff's `match_percent`, and it can rank a version with more differing instructions above one with fewer.

CampaignMap_Init (main 0x0204f960), scored with objdiff-cli 3.8.2 against the delinked original, 759 rows:

| Version | objdiff rows that differ | `fn.py check` | objdiff |
|---|---|---|---|
| `n = 0; n += call` in the first half (kept as "best" from batch 9 on) | 7 instructions, 9 registers | 7 differ | 99.9275% |
| `(u8)` cast first half, `n = 0; co[n + call]` second half (64c0b82a6) | 1 opcode | 1 differ | 99.9078% |
| `(u8)` cast in both halves (mainline until 64c0b82a6) | 2 opcodes | 2 differ | 99.8287% |

`partials-tried.md` and `partials-parked.md` recorded the 7-differ shape as the best (99.93%), and the retries they list (p4, reopen24-2, the permuter runs) started from it. None of them records the 1-differ combination.

## Notes

- objdiff's score, as fitted to these three versions (all within rounding): `100 × (1 − penalty / (100 × rows))`, with 60 for each row whose opcode differs (`DIFF_REPLACE`) and 5 for each differing argument (`DIFF_ARG_MISMATCH`, counted per register or relocation, not per row). One wrong opcode costs as much as 12 wrong registers. The objdiff source is not on this machine; the numbers come from `objdiff-cli diff -1 <original .o> -2 <ours .o> --format json`.
- The `=` rows of `fn.py diff` (a relocation that names another symbol at the same address, issue 0017) still cost 5 each in objdiff's score, so two functions with the same real difference can score differently.
- A version with only register differences is usually further from matching than one with a single wrong opcode: the registers come from the order the whole function is compiled in, while an opcode often comes from one expression.
- Places that use the percentage: the `BEST:` line of `.claude/skills/decomp-function/SKILL.md`, the `best %` column of `work/partials-parked.md`, the batch notes in `docs/loop.md`, `tools/triage_partials.py` (sorts by it within a class) and `tools/propagate.py` (keeps the template with the highest one). `fn.py check`, `permute.py` and `pragmas.py` already count differing instructions.

## Done when

- `fn.py diff` prints the number of differing instructions (`fn.py check`'s count, `=` rows not counted) next to objdiff's percentage, and the decomp-function skill reports it as `BEST:` (for example `BEST: 1 differ (99.91%)`).
- `triage_partials.py`, `propagate.py`, `partials-parked.md` and the partials job templates rank by the count first.
- The partials guidance in `docs/workflow.md` says to try combining the best halves of different attempts, since each half can match on its own (CampaignMap_Init).

## Fixed (2026-10-12)

`fn.py diff` ends with `NAME: N differ, P% match`: N counts the `|` rows, so `=` rows do not count. `fn.parse_diff` reads that output and `fn.rank` sorts by N, then P; `triage_partials.py` (within a class, `Differ rows` before `Match`), `progress.py` (the "Partial functions" list, `Differ` before `Match`) and `propagate.py` (`better()`, the closest failed template) use them. The decomp-function skill reports `BEST: <n> differ (<percent>%)`, and the partials job templates, `docs/workflow.md` ("Order of work", step 6) and `docs/loop.md` rank by the count and say to try combining the best halves of different attempts. `partials-parked.md` has the count for every row, from `fn.py diff` on the mainline file.

N is objdiff's aligned count, not always `fn.py check`'s: on the 32 parked functions they differ for 13, from AiBuild_RateSiteDanger (26 in `diff`, 82 in `check`, which compares by position and counts everything after a missing instruction) to func_0204f0c0 (17 and 15, where objdiff shows a changed instruction as two one-sided rows). The parked list gives check's count next to diff's where they differ.

Tests: `make test` (`tools/tests/test_ranking.py`: parsing the last line, and the three CampaignMap_Init versions in fn.rank, triage_partials.sort_key and propagate.better).
