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
5. **Cookbook for agents.** The tips above, plus the relevant ones in `partials-tried.md`, as a section in `docs/workflow.md` or the decomp-function skill, so agents try the rewrites that worked before ad hoc ones. Done 2026-10-11: "Near misses: the cookbook" in `docs/workflow.md`; the decomp-function skill and the partials and renames job templates point to it.
6. **Stop rule and parking.** Decide when a function is parked (for example: permuter ran N hours, agents tried twice) so the loop stops spending tokens on it, and record it so it can be reopened when the tool improves. Done 2026-10-11: "When to stop: parking" in `docs/workflow.md`, numbers under "Measured" below.
7. **Machine Learning to technique to try and map assembly code => Code.** I am not an ML expert, but it feels possible that we can train a model on the assembly code to produce C. However, this may be time intensive and low-value, especially if we have to train a larger number of ASM and C code that we don't have.

## Measured (2026-10-11)

What past partial retries say about a stop rule and about a cookbook, from existing records only (no new attempts). Data and scripts: `work/0052/` (`attempts.csv` one row per function per retry, `jobs.csv` tokens and times, `holdout.csv`, `analyze.py`, `holdout_tally.py`). Sources: `partials-tried.md`, the loop results files, `loop/state.json`, the batch 8 to 10 logs in `loop-prompts/`, and `git log mainline` (6bdcd2031). b22/p15 and p16 were still running; their rows so far are in, without tokens.

Terms: a *retry* is one model job's work on a function after the group that first wrote it (batch 8 and 9 partials jobs, the batch 10 follow-ups p1 to p5, `partials10` to `partials20`, the batch 22 p-groups). The `partials21` pragma sweep spent about 2k tokens per function and is counted apart. Tokens per retry are the job's tokens divided by the functions it tried; four jobs have no token record and get the mean (33k).

Dataset: 136 functions, 211 model retries, about 6.1M tokens; 74 matched by a model retry and 8 by the sweep.

### Matches by retry number

| Retry | Functions tried | Matched | Rate | Tokens | Tokens per match |
|---:|---:|---:|---:|---:|---:|
| 1 | 136 | 64 | 47% | 3,955k | 62k |
| 2 | 49 | 8 | 16% | 1,361k | 170k |
| 3 | 20 | 2 | 10% | 596k | 298k |
| 4 | 6 | 0 | 0% | 170k | - |

The pragma sweep matched 8 at about 2k tokens each: 5 had never been retried, 2 had one failed retry, 1 had two.

### Stop rules applied to the past

| Rule | Matches kept | Matches lost | Tokens saved |
|---|---:|---:|---:|
| Park after 1 failed retry | 64 | 10 | 2,128k (35%) |
| **Park after 2 failed retries** | 72 | 2 | 767k (13%) |
| Park after 3 failed retries | 74 | 0 | 170k (3%) |
| Park after the first retry with no gain | 65 | 9 | 1,802k (30%) |
| Park after two retries with no gain | 73 | 1 | 428k (7%) |

A gain does not predict a match: after a retry that gained but did not match, the next retry matched 1 time in 13 (8%); after a retry with no gain, 9 in 62 (15%).

Match percent before the retry does not help either, and near misses are the worst:

| Match before the retry | Retries | Matched | Rate | Tokens per match |
|---|---:|---:|---:|---:|
| below 90% | 31 | 17 | 55% | 60k |
| 90-95% | 44 | 18 | 41% | 72k |
| 95-98% | 50 | 14 | 28% | 99k |
| 98-99.5% | 32 | 14 | 44% | 58k |
| 99.5% and up | 54 | 11 | 20% | 142k |

First retries alone show the same pattern (99.5% and up: 8 of 25).

Triage class, batch 22 retries only (the triage ran just before them; small samples): register-only 4 of 13, order 7 of 21, missing/extra 2 of 17, structural 2 of 7.

The 10 matches at retry 2 or later:

| Function | Retry | Fix | Would a reopen condition have caught it? |
|---|---:|---|---|
| Player_GetCoDefense | 2 | wrong slot argument, found against its twin | yes: twin |
| UnitPath_CountSteps | 2 | dsd split tail, same as func_ov005_0235ad60 | yes: twin matched |
| GfxQueue_AddKind4 | 2 | written like AddKind1-3 with their header | yes: twin |
| SurvivalSelect_Draw | 2 | PageStripTask_Draw's redundant else | yes: twin matched |
| Ov005_LoadCoGridTextures | 2 | `opt_dead_assignments off`, then decl order | no: the sweep alone took it from 8 to 7 differing instructions |
| BattleCells_RunImpactList | 2 | `side` local for one argument | no |
| MainMenu_Setup | 2 | own locals for the second save block | no |
| MapView_ClearUnitTile | 2 | full address sum, no pointer | no (but the cookbook had it) |
| BattleCell_ApplyStep | 3 | signedness split stops CSE | no |
| LinkScreen_MovePlayerUp | 3 | MovePlayerDown's shape plus `optimization_level 2` | yes: twin and pragma |

### Chosen rule

Park after two retries without a match (gained or not); passes with no model tokens do not count. Reopen for one retry when a no-token pass gets within two instructions, a twin or same-shape function matches, a callee prototype, struct type or symbol it uses changes, or a new cookbook line fits. Retry functions with fewer retries first, not the closest to 100%. Applied to the past: 2 of 74 matches lost (one, LinkScreen_MovePlayerUp, a twin would have reopened), 13% of retry tokens saved. The rule is in `docs/workflow.md`; the 37 game partials that meet it now are listed in `work/partials-parked.md`, with sturmds's four in `steer.md`.

### Cookbook hold-out test

For each function matched after at least one failed attempt (all 82 retry matches), the fix was classified from the notes and compared with the tips recorded before the matching job started (decomp-function `SKILL.md`, `partials-tried.md`, results NOTES and `Skill tip` lines, batch logs; commit messages and job prompts not counted). *Hit*: an earlier tip names this rewrite for this kind of diff. *Partial*: an earlier tip names the same rewrite for another kind of diff, or only a needed second fix. *Miss*: nothing. Fixes found on a sibling in the same job count as misses ("same job").

| Set | Matches | Hit | Partial | Miss | Hit rate | Hit or partial |
|---|---:|---:|---:|---:|---:|---:|
| All | 82 | 26 | 12 | 44 | 32% | 46% |
| Model retries only | 74 | 18 | 12 | 44 | 24% | 41% |
| Model, without same-job siblings | 64 | 18 | 12 | 34 | 28% | 47% |
| Batch 8-10 retries (10-08, 10-09) | 22 | 0 | 4 | 18 | 0% | 18% |
| partials11-20 (10-10, 10-11) | 37 | 8 | 5 | 24 | 22% | 35% |
| Pragma sweep (partials21) | 8 | 8 | 0 | 0 | 100% | 100% |
| Batch 22 (10-11) | 15 | 10 | 3 | 2 | 67% | 87% |

By fix kind: pragma 11 of 11 hits (circular: the sweep is how the tip was applied), address forms 4 of 9 (6 with partials), reading in place 3 of 6 (6 with partials), aliasing 2 of 4, twins 2 of 3, reuse of one variable 2 of 6; control-flow forms 0 of 10, expression forms 0 of 7, statement placement 0 of 4, bugs 1 of 7. By triage class (batch 22 matches only): register-only 3 hits and 1 partial of 4, order 4, 1 and 2 misses of 7, missing/extra 1 and 1 of 2, structural 2 of 2.

What this says: about half of the fixes so far were new each time, mostly control-flow and expression forms, so a cookbook does not replace trying things. Its share grows as it fills: none of the batch 8-10 fixes had an earlier tip, two thirds of the batch 22 ones did. Of the hits, only MapView_ClearUnitTile had its whole tip available at an earlier failed retry and went without it (the tip sat in `partials-tried.md`, not in the skill), so the cookbook's gain is mainly in finding a fix sooner, which these records cannot measure.

Judgement calls: the fix kind and the hit/partial line are mine, from the notes; the rows marked "judgement" in the table below are the closest calls. A hit means an earlier tip named the fix, not that an agent with the cookbook would have tried it first.

Not measured: tokens per function (only per job, so per-retry tokens are averages); time to match with and without the cookbook (needs new attempts, which this task did not spend); the triage class of the functions matched before the triage existed (it ran first on 2026-10-11); the batch 7 and 8 retries are poorly recorded (no tokens, attempt count from a prompt).

<details><summary>All 82 cases</summary>

| Function | Matched in | Fix | Prior tip named it? | Earlier tip (source) | Note |
|---|---|---|---|---|---|
| Player_GetCoDefense | b8-partials | BUG | miss |  | semantic fix found by comparing with the matched twin (prompt said so; no recorded tip) |
| MenuCursor_Update | b9-partials | REUSE | miss |  |  |
| Ai_ClearThreatInAntiAirCover | b10f-p1 | CFLOW | miss |  |  |
| AiUnit_FindFiringTile | b10f-p1 | CFLOW | miss |  |  |
| AiTransport_PickFootUnit | b10f-p1 | REUSE | miss |  | MenuCursor_Update's shared local was only in its commit message |
| AiFoot_BoardTransportForSite | b10f-p1 | INPLACE | partial | skill batch 7 (read the full state chain instead of a local) | same rewrite for literal order; here registers |
| AiUnit_BoardSeaTransport | b10f-p1 | INPLACE | partial | skill batch 7 | same as above |
| AiTurn_TrySiteAction | b10f-p2 | BUG | miss |  |  |
| AiUnit_HasFuelForTile | b10f-p2 | VOLATILE | partial | skill batch 7 | tip says reread the state; plain rereads were CSEd so volatile was needed |
| Ai_AddOoziumThreat | b10f-p2 | CFLOW | miss |  |  |
| AiSite_ScorePiperunnerNeed | b10f-p2 | INPLACE | partial | skill batch 7 |  |
| AiUnit_TeammateOutdamages | b10f-p2 | CFLOW | miss |  | same job (Ai_AddOoziumThreat recipe) |
| AiSeaTransport_MoveLoaded | b10f-p2 | STMT | miss |  |  |
| AiBlackBomb_ScoreExplosionAt | b10f-p2 | CFLOW | miss |  |  |
| Player_AddPowerCharge | b10f-p3 | CFLOW | miss |  |  |
| TileMap_Fill | b10f-p3 | SPLIT | miss |  |  |
| MapView_ScrollStep | b10f-p3 | TYPE | miss |  |  |
| DigitSprite_QueuePopIn | b10f-p5 | EXPR | miss |  |  |
| VsSetup_AddMapAndCoButtons | b10f-p5 | REUSE | miss |  |  |
| CoSelect_ApplyPicks | b10f-p5 | ADDR | miss |  |  |
| MapPreview_Draw | b10f-p5 | REUSE | miss |  | same job (VsSetup) |
| func_02001f50 | b10f-p5 | TWIN | miss |  |  |
| PlayerPanels_Refresh | partials11 | BUG | miss |  |  |
| PlayerPanels_Init | partials11 | BUG | miss |  | same job |
| Ov005_SendTexturedQuad | partials11 | INLINE | miss |  |  |
| Battle_DrawTexturedQuad | partials11 | INLINE | miss |  | same job |
| Ov005_SoundRoom_AddCellButtons | partials12 | DECL | hit | partials10 MapPreview_DrawBlock (block decls to function scope) |  |
| func_ov005_0235ad60 | partials12 | BUG | miss |  |  |
| UnitPath_CountSteps | partials12 | BUG | miss |  | same job (0235ad60) |
| Battle_StartAttackerMoveSound | partials12 | BUG | hit | b10f-p2 AiTurn_TrySiteAction (live r0 = callee argument) |  |
| Ov004_FillBlock | partials12 | ADDR | miss |  |  |
| Battle_UpdateSway | partials12 | ALIAS | miss |  |  |
| func_ov001_02351538 | partials13 | INPLACE | hit | b10f-p1 (read the field again instead of a local copy) |  |
| BattleFootstepTask_Start | partials13 | CSEBREAK | miss |  |  |
| BattleFighterTask_Setup | partials13 | CSEBREAK | miss |  | same job (stop sharing) |
| Ov005_LoadCreditsPage | partials14 | ADDR | partial | partials12 Ov004_FillBlock ((base + off)[j]) | judgement: same base-then-index idea; different form |
| Window_DrawFrameMiddleRow | partials14 | TYPE | miss |  |  |
| Ov001_IsActingStealthLowOnFuel | partials14 | INLINE | partial | b10f-p2 RELOAD_STATE | volatile index read was a needed second fix |
| Ov001_HasStealthLowOnFuel | partials14 | INLINE | miss |  | same job |
| CampaignMap_CenterOnMission | partials15 | EXPR | miss |  |  |
| CampaignMap_EaseScroll | partials15 | EXPR | miss |  | same job |
| ResultStats_DrawUnitGrid | partials16 | STMT | miss |  |  |
| SpriteFile_Draw3d | partials16 | CFLOW | miss |  |  |
| PageStripTask_Draw | partials17 | CFLOW | miss |  |  |
| SurvivalSelect_Draw | partials17 | CFLOW | miss |  | same job (PageStripTask_Draw twin) |
| CombatSetup_BuyUnit | partials17 | ADDR | partial | partials14 Ov005_LoadCreditsPage ((&a[i])[1]) | judgement: index from a shifted base |
| func_0204f3b0 | partials17 | STMT | miss |  | the WIP had tried this; a header change since made it work |
| ResultPoint_DrawPanel | partials19 | EXPR | miss |  |  |
| ResultPoint_DrawPanelTally | partials19 | EXPR | miss |  | same job |
| Image4bpp_SetPixelLinear | partials20 | ADDR | miss |  |  |
| Player_HasAnyUnit | partials20 | ADDR | hit | b10f-p2 func_02099ba0 (byte-offset read keeps the mul) |  |
| Palette_GetArmyBlock | partials20 | EXPR | miss |  |  |
| PlayerInfo_ToggleLastPageView | partials20 | ALIAS | partial | partials12 Battle_UpdateSway (same symbol as the store) | judgement: a cast of the base at every access |
| CombatSetup_SetPlayerReady | partials20 | ALIAS | hit | partials12 Battle_UpdateSway (alias symbol hoisted above a store) |  |
| CombatSetup_SetPlayerUnready | partials20 | ALIAS | hit | partials12 Battle_UpdateSway |  |
| GfxQueue_AddKind4 | partials20 | TWIN | hit | b10f-p5 func_02001f50 (copy a matched sibling's shape) |  |
| MapPreview_FillTileMap | partials20 | EXPR | miss |  |  |
| DayBanner_RefreshLayer | partials20 | TYPE | partial | b10f-p5 (one variable for several jobs) | reuse was the second fix |
| Ov005Slide_StartAll | partials20 | REUSE | hit | b10f-p5 (one variable for several loops) |  |
| NameEntry_OnKeySelect | partials21 | PRAGMA | hit | b17-s12 NOTES (name entry built with optimization_level 0) | sweep |
| CombatSetup_UpdateCoPick | partials21 | PRAGMA | hit | b17-s12 | sweep |
| AiTile_IsFreeForUnitType | partials21 | PRAGMA | hit | b17-s12 | sweep |
| Save_WriteRecord | partials21 | PRAGMA | hit | b17-s12 | sweep |
| SpriteAnim_Queue3d | partials21 | PRAGMA | hit | b17-s12 | sweep |
| CampaignMap_StartMissionScroll | partials21 | PRAGMA | hit | b17-s12 | sweep |
| Camera_SetFromMapScroll | partials21 | PRAGMA | hit | b17-s12 | sweep |
| AiBuild_ShouldSaveFunds | partials21 | PRAGMA | hit | b17-s12 | sweep |
| EventChoice_DrawCursor | b22/p01 | ADDR | hit | partials14 Ov005_LoadCreditsPage ((&a[i])[1] not a[i + 1]) |  |
| MainMenu_Setup | b22/p03 | SPLIT | partial | b10f-p3 TileMap_Fill (own counter per loop) | own locals per save/restore block |
| TagGauge_PopIn | b22/p05 | CFLOW | miss |  | partials16's if != NULL wrap is a different return shape |
| MapSelect_ResetTables | b22/p05 | STMT | miss |  |  |
| LinkScreen_MovePlayerUp | b22/p06 | TWIN | hit | b10f-p5 func_02001f50 and partials21 pragmas | twin tip existed at its earlier retries; the pragma did not |
| LinkScreen_SavePanelOrder | b22/p06 | PRAGMA | hit | partials21 |  |
| EventQueue_Drain | b22/p06 | INPLACE | hit | partials13 func_ov001_02351538 (read in the loop condition) |  |
| CombatSetup_AddShopButtons | b22/p07 | PRAGMA | hit | partials21 |  |
| MapView_ClearUnitTile | b22/p11 | ADDR | hit | partials12 Ov004_FillBlock (no precomputed pointer; full sum in the store) | tip also existed at its failed partials20 retry |
| BattleCell_ApplyStep | b22/p12 | CSEBREAK | partial | partials13 BattleFootstepTask_Start (two bases stop the sharing) | signedness split is a new way to stop it |
| BattleCellSprite_Arrived | b22/p13 | INPLACE | hit | partials13 func_ov001_02351538 and b10f-p1 |  |
| BattleCells_RunImpactList | b22/p14 | CACHE | partial | b22/p07 CombatSetup_AddShopButtons (keep an index local) |  |
| Battle_UpdateHud | b22/p14 | REUSE | hit | b10f-p5 (one variable for several loops) |  |
| Battle_CopyTilemapRect | b22/p14 | ADDR | hit | partials12 Ov004_FillBlock |  |
| Ov005_LoadCoGridTextures | b22/p16 | PRAGMA | hit | partials21 |  |

</details>

### Still open

This covers ideas 5 and 6 only. The larger tools stay deferred and the issue stays open: a permuter-style source mutator (ideas 1 to 4), retrieval of past fixes by nearest assembly windows (kNN over diff windows, which the cookbook does by hand), and machine learning (idea 7).

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
