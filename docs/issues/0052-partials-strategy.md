# 0052: The game partials need a better tool than permute.py and agent retries

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-11
- **Milestone:** M2

## Problem

About 77 game functions in ARM9 are started in C but not linked (the "Started, not linked" table in `PROGRESS.md`). Game code is 96.07% of bytes in C; these are most of the rest. sturmds wants the non-SDK ARM9 code finished before new territory (more SDK, ARM7), and is not satisfied with stopping here.

The current method matches few of them:

- Batch 22 sent Opus agents at the partials, with the pragma sweep first, then twins, field types, hand variants and `permute.py`. Result: 8 matches in about 39 functions tried (about one in five), at 100k to 300k tokens per group of four or five.
- Most misses are 95% to 99.9%. The diff that is left is two registers swapped, or the same instructions in another order. Agents report "register-only, no pragma helped, permute found nothing" and stop.

Triage (`tools/triage_partials.py`, 2026-10-11, 77 functions):

| Class | Functions | What differs |
|---|---:|---|
| register-only | 21 | same instructions, register numbers differ |
| order | 26 | same instructions in another order |
| missing/extra | 20 | up to max(4, n/25) instructions more or fewer |
| structural | 10 | anything bigger |

## What we have tried

- **Pragma sweep** (`tools/pragmas.py`). Compiles the function under each code-generation pragma mwccarm 2.0-72 accepts. It found 8 matches from a pragma alone or a pragma plus a small change (partials21, then batch 22). On the latest triage, no function reaches 100% from a pragma alone.
- **permute.py** (`tools/permute.py`, based on khdays-decomp's `declperm.py`). Axes: order of leading local declarations, swaps of adjacent simple statements, local types. Agents run it for 240 to 300 s per function. It found the declaration order for a few matches (LinkScreen_SavePanelOrder, MbGame_SetPlayers, MainMenu_Setup), and nothing on the register-only rest.
- **Compiler build.** Ruled out on 2026-10-11: func_020a6ac4 under mwccarm 2.0-72 differs in 7 instructions; 2.0-87 33, 3.0-114 and 3.0-139 52, 4.0-1051 52 at best. `docs/awre-facts.md` has the fingerprint (2.0 builds 72 to 82 give the same code).
- **Hand variants by agents.** What worked, from the batch 22 results (`Skill tip` lines):
  - Read a struct field in the loop condition instead of caching it in a local; mwcc hoists the load itself and picks the original registers (EventQueue_Drain).
  - With `opt_common_subs off`, keep an index local the original holds in a register, or it is recomputed at each use (CombatSetup_AddShopButtons).
  - Copy a mirrored twin's shape, then sweep pragmas on it: `optimization_level 2` (LinkScreen_MovePlayerUp).
  - A store at a fixed offset from a computed address comes from `(&a[y][x])[32]`, not `a[y][x + 32]` (EventChoice_DrawCursor).
  - Two save/restore blocks of the same globals with different registers: give the second block its own locals, then swap the first pair's declaration order (MainMenu_Setup).
  - An explicit `return;` at the end of a final if block can move an argument setup after a constant store (TagGauge_PopIn).
  - Index a table as one flat array (`counts[kind * 4 + i]`): 93.6% to 95.1% (func_0208395c).
  - A value the original copies to a scratch register instead of reusing a callee-saved one can mean the source computes a later use after the copy (SoundStream_FillBlocks).

## Ideas to scope

1. **Random source mutation, decomp-permuter style.** simonlindholm/decomp-permuter applies random, type-aware rewrites to the C and keeps the variants that score better. Its passes include: swap commutative operands, add or remove temporaries, reorder statements and declarations, change loop forms (for/while/do-while), invert if/else, add or drop casts, change `a[i]` to `*(a + i)`, split or merge expressions, add `do {} while (0)`, move an assignment into a condition. Most of these are what fixed our matches by hand. Options:
   - Extend `permute.py` with these passes. It already scores with the build's compiler and relocation-aware diff. Parsing C well needs pycparser (pure Python; would be pinned in `tools.lock`).
   - Pin decomp-permuter itself in `tools.lock` with pycparser. Its scorer calls an ARM objdump, so it needs `sudo apt install binutils-arm-none-eabi` from sturmds, or an adapter to our scorer.
2. **Run without model tokens, for hours.** A batch driver that runs the permuter over every partial, longest-first or closest-first, under `nice` and a job count that leaves room for the loop (see the 12-agent memory cap). Agents then finish only the ones it gets close, starting from its best variant.
3. **Use the diff to aim the search.** For register-only diffs, map each swapped register pair back to the C variables that live in them (from mwcc's output or by elimination) and mutate only statements that touch those variables. For order diffs, mutate only the statements behind the reordered instructions.
4. **Inline helpers.** The original may have used `static inline` functions or macros we do not know about; inlining changes the order mwcc numbers its virtual registers. A pass that wraps a block in an inline helper (and one that removes one) is worth trying on the order class.
5. **Cookbook for agents.** The tips above, plus the relevant ones in `partials-tried.md`, as a section in `docs/workflow.md` or the decomp-function skill, so agents try the rewrites that worked before ad hoc ones.
6. **Stop rule and parking.** Decide when a function is parked (for example: permuter ran N hours, agents tried twice) so the loop stops spending tokens on it, and record it so it can be reopened when the tool improves.

## Context and pointers

- Report of the hard assembly shapes we already matched, with examples (not in the repo).
- `work/partials-triage.md`: per-function class, differing rows, sweep result, nearest pragma'd file. Rerun: `python3 tools/triage_partials.py --out work/partials-triage.md --jobs 3` (about 10 minutes).
- `work/partials-tried.md`: what every agent tried per function, and the result (about 700 lines, 210 entries). Read before trying a function again.
- `work/loop/results/b22-p*.txt` and `partials21.txt`: RESULT blocks with `Skill tip` lines.
- `work/loop/journal.md`: the loop's log, including partials yields per group.
- Tools: `tools/permute.py`, `tools/pragmas.py`, `tools/triage_partials.py`, `tools/fn.py check` (fast scorer) and `fn.py diff`, `tools/ds.py field` (signedness of loads), `tools/propagate.py` (twins).
- Credits: khdays-decomp is in `docs/third-party/khdays-decomp.md`. decomp-permuter, if used or followed, needs its own page in `docs/third-party/`.

Constraints: Linux only, every tool pinned in `tools.lock`, no network except tool downloads listed there, no decomp.me. Ask sturmds before installing anything. The decomp loop keeps running and landing on mainline meanwhile; batch 22 partials groups (p11 to p16) are working on some of these functions now.

## Done when

The approach is chosen and written here, the tool exists and is listed in `docs/workflow.md`, and it has been run over the open partials with the result recorded in this issue.
