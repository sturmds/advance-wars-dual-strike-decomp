# Decompiling one function

Everything runs locally. The examples use `PackNibblePairs` (main, 0x020803fc), one of the three M0 proof functions.

## 1. Pick and look

```sh
make                              # builds once, so build/delinks/ exists
python3 tools/fn.py show 0x020803fc
```

`show` prints the module, ARM or Thumb, the size, the source file that covers the function (if any), its callers and other references (`refs:`: pointer tables such as `data_021585c0+0x14c`, and functions that load its address as a callback), and the original disassembly with relocations.

Good first picks: leaf functions (no `bl`), and callers of functions that already match.

### Structs and data

```sh
python3 tools/ds.py show Task                 # layout of a struct in include/
python3 tools/ds.py field Task.unk_64          # functions that use a field, and how the original loads it
python3 tools/ds.py show data_02153634         # a data symbol: type, references, bytes
```

`ds.py` does for data what `show` does for functions (issue 0041).

- `show STRUCT` prints each field's offset, size, type and name, bit fields as `:lsb+width`, gaps between fields, the header, the data symbols declared with the type, and how many C files use each field (`--uses` lists the functions).
- `show data_ADDR|gName|0xADDR` prints the module, kind, size to the next symbol, declared type, the functions and tables that reference it, and its bytes decoded with the declared type (`--count N` table entries). If the symbol lies inside another symbol's declared size, it says so (`inside: gCoTable+0x28`). A symbol with a declared size lists the later symbols it covers.
- `dupes` lists likely duplicates for renames jobs: a type defined twice under one name, header structs with the same size and field offsets, types in C files whose fields line up with a header struct, union views that lie within another, symbols declared with two types, and `#define`s repeated in C files or matching a header constant's value. They are candidates: small structs share layouts by chance. `--kind K` shows one list, `--all` every row.
- `field STRUCT.FIELD` lists each function in `src/` that names the field. For each, it shows the original instructions at the field's offset (`ldrsh`, `ldrh`, `strh`, ...) and any `(s16 *)&` casts in our C. A closing `check:` line says when the declared signedness disagrees with every load. Fix the type in the header rather than adding a cast.

Layouts come from the compiler: `tools/dsindex.py` compiles a small probe for each header with `-g` and reads the DWARF debug data, so unions, bit fields and padding are exact. C files that define their own types are probed the same way with their function bodies cut out. It writes `build/dsindex.json` in about three seconds and then reprobes only the files that changed; `make dsindex` runs it. Field uses are found by name, in the C files that include the struct's header, so two structs with a field of the same name in one file both show up. Loads are read only for functions that already have C, and only at the field's offset in that struct.

## 2. Give it a file

Add an entry to the module's `delinks.txt` (main: `config/AWRE/arm9/delinks.txt`; overlays: `config/AWRE/arm9/overlays/ovNNN/delinks.txt`). Leave out `complete` for now:

```
src/game/pack_nibbles.c:
    .text       start:0x020803fc end:0x02080440
```

`end` is the next function's address, so alignment padding stays inside the file. A file can hold several neighbouring functions; start with one, and merge files as evidence appears (shared static data, functions that always sit together).

Rename the function in `symbols.txt` once you know what it does (TENETS 4). Rename data symbols it uses the same way. C code refers to symbols by these names.

## 3. Write C

Put the file at the path from step 2. Shared types go in `include/`; SDK declarations in `include/sdk/`.

- Compiler: mwccarm 2.0-72, `-O4,p -proc arm946e -interworking`. `-ipa` does not exist in this version, so each function compiles on its own.
- Thumb SDK assembly: write it as `asm void` under `#pragma thumb on` (see `src/sdk/fx/fx_mtx43.c`). The game code itself has no Thumb.
- Shape matters more than meaning at first: declaration order, `for` vs `while`, a one-case `switch` instead of an `if`, temporaries. The compiler is old and literal.

## 4. Diff until it matches

```sh
python3 tools/fn.py diff PackNibblePairs
```

This compiles the file, rebuilds the original's delinked object, and runs `objdiff-cli`. Lines marked `|` differ; relocations are compared by symbol name. The exit code is 0 only at 100%.

Some library code tests link-time constants (`SDK_SYS_STACKSIZE`, `SDK_IRQ_STACKSIZE`, `SDK_SECTION_ARENA_DTCM_START`). The ROM holds their values in literal pools, and the C must declare them as `extern char NAME[];` and use them as numbers, or mwcc folds them. `ARM9_ABSOLUTE_SYMBOLS` in `tools/configure.py` gives each one its value, and `tools/lcf_absolute.py` writes them into `build/arm9.lcf`. Add a constant there when a function needs one. `diff` marks such a word `=` and counts it as matching when the original word holds the symbol's value; `check` does the same.

### Fast check

```sh
python3 tools/fn.py check src/game/pack_nibbles.c PackNibblePairs
```

`check` compiles one file with the build's compiler command into a temporary object and compares one function with `build/fnindex.json`. It needs no `delinks.txt` entry, and runs neither ninja nor dsd: about 3 ms per function in-process for a small file and 100 ms for the largest, plus 0.1 s of Python start-up from the command line. Exit 0 on a match; otherwise it prints how many instructions differ, position by position (an inserted instruction makes everything after it differ, where `diff` lines them up).

It compares the bytes outside relocations, the relocation offsets and kinds, and the relocation targets as addresses, so `Table+0x1c` equals dsd's symbol at that address (issue 0017). A relocation to a symbol it cannot place (a string or `static` data the object defines, missing from `symbols.txt`) counts as a difference. `tools/tests/test_fncheck.py` checks that it never says yes where `diff` says no. Use it to try many variants, then run `diff` and `make release` on the one that matches.

`build/fnindex.json` lists every function: module, mode, address, size, original bytes, relocations, a masked hash (equal for functions that differ only in what they call or load), the mnemonics before the literal pool, and whether a C file covers it. `make fnindex` builds it in about a second; `check` rebuilds it when `symbols.txt`, `relocs.txt` or `extract/` change. It does not watch `delinks.txt`, so `status` can lag behind `fn.py add` until the next `make fnindex`. Tools can `import fnindex` and call `fnindex.load()` once, then `fnindex.check(index, path, name)` per variant.

## Tools that match many functions

All three work from `build/fnindex.json`, check every candidate with the fast check, and `fn.py diff` each before `fn.py finish`. They write `src/game/<module>/func_XXXXXXXX.c`, `delinks.txt` entries and nothing else. Run them on `mainline` with a clean tree, build and commit the result in chunks (each commit must pass `make release`). All are based on khdays-decomp (`docs/third-party/khdays-decomp.md`).

- `python3 tools/propagate.py [--dry-run] [--identical-only]`: copies the C of a matched function to unmatched functions with the same masked hash (identical twins) or the same shape with other immediates (near twins). `fn.py pick --families` ranks the unmatched families; match a head by hand, then run this.
- `python3 tools/trivial.py [--pattern P] [--dry-run]`: writes C for functions of up to nine instructions: empty, constants, getters, stores, load-modify-store, tail calls, wrappers, comparisons. Patterns: empty, const, getter, tailcall, wrapper, store, modify, addr, compare, other.
- `python3 tools/permute.py FILE.c NAME [--budget-seconds 120] [--apply]`: for a started function below 100%, tries declaration orders, statement swaps and local types, and prints the best variants. `--apply` replaces the source when the best one matches.

What they leave behind is in `docs/issues/0024-brute-force-tool-leftovers.md`.

### Pragmas

Some original files were built with other optimisation settings: the name entry screen only matches with `#pragma optimization_level 0`, and `NameEntry_MoveFromKey` only with `#pragma opt_common_subs off`. permute.py changes the C, never these settings.

- `python3 tools/pragmas.py FILE.c NAME [--jobs N] [--apply]`: compiles the C as it is under each pragma that changes code generation and prints the best scores. It tries `optimization_level` 0-3, `optimize_for_size` and each `opt_*` switch and `peephole` both on and off, alone, then in pairs and triples of the ones that changed the function. Each goes at the top of the file and around the function alone (`#pragma push` ... `#pragma pop`). A few hundred compiles, one to a few minutes. `--apply` writes the matching variant; move the pragma below the file comment, say why it is there, and confirm with `fn.py diff`. `--accepted` checks that mwccarm 2.0-72 still accepts every pragma in the list (`scheduling`, `register_coloring` and `opt_cse` do not exist in this version; an unknown pragma is only a warning). `make test` runs `tools/tests/test_pragmas.py` (no ROM).
- `python3 tools/triage_partials.py --out FILE.md [--jobs 3] [NAME ...]`: for every started, unlinked function, sorts the `fn.py diff` result into register-only, order, missing/extra and structural, runs the pragma sweep, and names the nearest file in the same module that uses a pragma. A pragma'd neighbour hints that the original object was built with that setting; a sweep that gets closer but not to 100% can mean the C was shaped for the wrong flags. The `proto` column counts what `protocheck.py` reports for the file.

Run the sweep on a partial before trying C variants by hand or with permute.py.

## Near misses: the cookbook

A *partial* is a function whose C compiles but does not match yet: a `WIP` commit, listed under "Started, not linked" in `PROGRESS.md`. This section lists the source rewrites that matched partials before, grouped by what the diff shows, and when the loop stops retrying one. The numbers behind it are in `docs/issues/0052-partials-strategy.md` ("Measured").

Terms: *registers swap* means the same instructions with two register numbers exchanged; a *spill slot* is the stack offset where mwcc keeps a value it has no register for; *CSE* (common subexpression elimination) is mwcc computing a value once and reusing it; *hoisting* is moving a load or computation out of a loop or above other code.

### Order of work

1. Rule out a bug. 7 of the 74 retry matches were not about C shape at all:
   - a callee prototype missing an argument (a temp in r1 where ours uses r0 means r0 is still a live argument of the next call; AiTurn_TrySiteAction, Battle_StartAttackerMoveSound);
   - a local declaration that disagrees with the callee's later definition: fewer arguments (UnitAction_Capture, AiUnit_JoinDamagedUnit, PlayerInfo_DrawRankAndExperience, FS_LoadOverlayInfo, func_ov002_02358c40), or an `int` where the definition takes `u8` or `s16`, which changes how the caller narrows the value. Run `python3 tools/protocheck.py FILE.c`; it lists these by kind (`count`, `call`, `width`, `ret`). A `count` row means the original passes a value the C does not: look for the register the original keeps live into the call;
   - one trailing `bx lr` too many: dsd split the function's 4-byte tail off as its own symbol. Look for a size 0x4 function right after yours in `symbols.txt` and merge it back (UnitPath_CountSteps);
   - a pointer walk whose registers swap with the `i * sizeof` induction: the array may start one entry before its symbol (PlayerPanels_Init);
   - a wrong argument value or slot (Player_GetCoDefense, func_ov002_023571d0);
   - a header that lacks something the siblings use, such as `volatile` fields (GfxQueue_AddKind4).
2. If a sibling of the same shape matched (a twin, an Up/Down or Ready/Unready pair), copy its shape first (func_02001f50, GfxQueue_AddKind4, LinkScreen_MovePlayerUp).
3. Run `pragmas.py`. A pragma was part of 13 partial matches, and matched 5 of them with no change to the C.
4. Find the symptom below and try its rewrites, before inventing new ones.
5. Run `permute.py` for declaration order once the instructions agree. It finished many matches, but on a register-only diff it never found one alone.

### Rewrites by symptom

Each line: the rewrite, then the functions it matched.

**The original reloads a value at each use; ours keeps it in a register**

- Read the field or global where it is used (in the loop condition, in each compare) instead of caching it in a local; mwcc hoists what it can and the rest stays as in the original (EventQueue_Drain, func_ov001_02351538, BattleCellSprite_Arrived, AiSite_ScorePiperunnerNeed).
- Assign from the field, not from the local copy: `boatType = transport->type;` (AiFoot_BoardTransportForSite, AiUnit_BoardSeaTransport).
- Read the game state through the whole `data_027c027c->unk_48[data_027c0284]` chain at each use; this also fixes literal-pool order. If mwcc still shares it, a volatile read, index first, forces the reload; mark it as match-only (`RELOAD_STATE` in AiUnit_HasFuelForTile, Ov001_IsActingStealthLowOnFuel).
- `#pragma opt_common_subs off` (SpriteAnim_Queue3d, EventChoice_DrawCursor).
- A store makes mwcc drop its cached copy: move the statement that needs the reload after that store (AiSeaTransport_MoveLoaded).

**The original computes a value once; ours computes it again**

- Read it once into a local, and use the local only where the original keeps it in a register (BattleCells_RunImpactList: `side` only for the side argument; Save_WriteRecord: the part count read once and stored back).
- Under `opt_common_subs off`, keep in a local the index the original holds in a register (CombatSetup_AddShopButtons).

**Part of an address is shared differently** (the original shares one product and rebuilds the rest, or the reverse)

- Stop the sharing:
  - read through two different bases, `scene + offset` and `scene->sides + offset` (BattleFootstepTask_Start);
  - read once through a cast, `*(T **)&p->field`, before the plain read (BattleFighterTask_Setup);
  - make one of the two address sums unsigned (`sizeof`); mwcc does not share across signedness (BattleCell_ApplyStep);
  - index through a byte offset, `(T *)((u8 *)base + i * sizeof(T))`, to keep a multiply per iteration instead of a moving pointer (Player_HasAnyUnit, func_02099ba0).
- Share the step: `(&a[i])[1]` instead of `a[i + 1]` (Ov005_LoadCreditsPage); `(&tiles[y][x])[32]` instead of `tiles[y][x + 32]` (EventChoice_DrawCursor); the row array seen from a field of the first row, `((Row *)&base->field)[i]` (CombatSetup_BuyUnit).
- No precomputed pointer: write the whole sum in the access, `*(map + block + colOff + rowOff) = 0;`, `*(dst + row * 32 + col)` (MapView_ClearUnitTile, Battle_CopyTilemapRect, Ov004_FillBlock).
- Other index forms that matched:
  - a 2D view, `((u8 (*)[2])grid)[c][r]` (CoSelect_ApplyPicks);
  - a pointer to the byte for a read-modify-write, `u8 *p = &a[off]; *p = (*p & m) | v;` (Image4bpp_SetPixelLinear);
  - one flat array, `counts[kind * 4 + i]` (func_0208395c, gain only);
  - `(y + y) + x` when the `add` operands are swapped (MapPreview_FillTileMap);
  - the state as base + index * sizeof + offsetof of `unk_48` when `data_027c027c` and `data_027c0284` swap registers (BattleSetup_LoadFromGame);
  - the global's address cast and a run-time offset added, when the original does not fold the offset into the relocation (`WORK_FIELD`, `SETUP_FIELD`).

**A load moves above a store that the original keeps it after**

- The load may go through an alias symbol of the stored object (`data_0229ed30` is `gMenuState + 0x200`, `data_0229fb30` is `gMenuState + 0x1000`). Read through the same symbol as the store, or as a cast from it, `(T *)((u8 *)&gMenuState + 0x1000)`; `fn.py` counts the relocation as matching (Battle_UpdateSway, CombatSetup_SetPlayerReady, CombatSetup_SetPlayerUnready, PlayerInfo_ToggleLastPageView).

**Register-only: two values swap registers**

- Pragmas first: `opt_loop_invariants off`, `opt_dead_assignments off` or `opt_propagation off` matched four with no C change (CombatSetup_UpdateCoPick, Save_WriteRecord, AiTile_IsFreeForUnitType, NameEntry_OnKeySelect). A dead store kept by hand to hold a register often means `opt_dead_assignments off` (Ov005_LoadCoGridTextures).
- One variable for several jobs, as the original has: the slot or origin variable as the loop counter, an earlier search's variable in a later search, one counter for two loops (VsSetup_AddMapAndCoButtons, MapPreview_Draw, AiTransport_PickFootUnit, Ov005Slide_StartAll, Battle_UpdateHud, MenuCursor_Update, DayBanner_RefreshLayer).
- The opposite: its own counter per loop, its own locals for a second save/restore block, then declaration order (TileMap_Fill, MainMenu_Setup).
- Move block-scoped locals to function scope, each subset in turn (Ov005_SoundRoom_AddCellButtons; MapPreview_DrawBlock gained).
- A stepped variable (`x += 54`) that swaps with a sum built from it: compute it from the loop counter where it is used (ResultStats_DrawUnitGrid).
- A narrowed local whose mask lands in the wrong register: update the parameter in place (`y += 8;`) and cast `(u8)y` at each use (ResultPoint_DrawPanel). A shift temporary in a parameter's register: store it in a `u16` local (Window_DrawFrameMiddleRow).
- A constant the original keeps in a callee-saved register: start a variable at the constant and update it in place, `list = 5; ... list -= d;` (DigitSprite_QueuePopIn).

**Spill slots differ** (same code, other stack offsets)

- Write each bound clamp as a ternary, an `if`, or assign-then-clamp, and try the combinations (Ai_ClearThreatInAntiAirCover, AiUnit_FindFiringTile). In the bounding-box loops: minX as assign-then-clamp, the first row as an if/else written into the loop variable (Ai_AddOoziumThreat, AiUnit_TeammateOutdamages). Distances as `dist = 0; if (x - tx > 0) dist += x - tx; else dist += tx - x;` (AiBlackBomb_ScoreExplosionAt). Then `permute.py`.

**Order: the same instructions in another order**

- Two hoisted constants loaded in the wrong order: a redundant `else pal = 6;` in the test that sets the higher one (PageStripTask_Draw, SurvivalSelect_Draw).
- Two slots set to one value with their index registers swapped: one chained assignment, `a[j] = a[i] = v;` (CampaignMap_CenterOnMission, CampaignMap_EaseScroll).
- Argument setup or a stack load before a constant store: an explicit `return;` at the end of the final `if` (TagGauge_PopIn); `if (p != NULL) { ...; return TRUE; } return FALSE;` instead of an early return (SpriteFile_Draw3d).
- A statement that runs once per outer iteration in the original sits in the inner loop in ours, or the reverse: check where it belongs (MapSelect_ResetTables, func_0204f3b0).
- A call result used in an expression: call it inside a local's initialiser (Palette_GetArmyBlock) or inline in each compare (AiUnit_HasFuelForTile). `(x = x << 9) >> 4` keeps a shift in place (Camera_SetFromMapScroll).
- A value the original copies to a scratch register instead of reusing a callee-saved one: compute the later use after the copy (SoundStream_FillBlocks, gain only).

**Conditional moves and BOOLs**

- `cmp; moveq #0; movne #1` comes from `cond ? ENUM_A : ENUM_B`; an int 0/1 ternary folds into `!=` and puts `movne` first (Player_AddPowerCharge). `if (c) return FALSE; return TRUE;` where a ternary gave `movne` first (Player_HasAnyUnit).
- A BOOL normalised twice (`cmp #0` with `movne/moveq`, then `cmp #1`): nest two inline helpers of different forms, an if-set flag around a ternary (Ov001_IsActingStealthLowOnFuel).

**Missing or extra instructions**

- A packed hardware word (TEXIMAGE_PARAM): build it in a `static inline` helper; its argument order sets the register order (Ov005_SendTexturedQuad, Battle_DrawTexturedQuad).
- An add through a signed halfword: type the field `s16` (MapView_ScrollStep).
- `add rN, rM, #0` comes from `x = 0; x += call();` (`terrain_firepower.c`).
- See also the bug checks in step 1.

**Rarely helped:** `permute.py` or full declaration-order searches on a register-only diff without a rewrite first; `register`; `volatile` other than for a forced reload; a cast on one use; moving a whole block or body into an inline helper.

### When to stop: parking

A *retry* is one model job's work on a function after the group that first wrote it (a `partials<N>` job or a batch 22 style partials group). Passes that spend no model tokens (`pragmas.py`, `triage_partials.py`, `permute.py`, a future permuter) do not count.

- **Park a function after two retries that did not match it**, whether they gained or not. In past data, first retries matched 47% of the time, second retries 16%, third 10%, fourth none; this rule would have lost 2 of 74 matches and saved 13% of retry tokens.
- **Record it** in `work/partials-parked.md`: one row with the function, module and address, file, best match, triage class, the retry jobs, the date, and what would reopen it. `partials-tried.md` keeps the notes of each attempt. The parked list in `steer.md` (sturmds's) also stays parked.
- **Reopen** a parked function for one more retry (park it again if that fails) when: a pass with no model tokens brings it to 100% or within two differing instructions; a twin or a function of the same shape matches; a callee prototype, a struct field type or a symbol it uses changes on mainline; or a new cookbook line fits its diff. Half of the past matches at a second or later retry had one of these.
- **Order**: retry functions with fewer retries first, not the ones closest to 100% first. Near misses above 99.5% matched least often (20% per retry) and cost the most tokens per match.

## 5. Mark it complete and review

1. `python3 tools/fn.py finish NAME` adds `complete` under the file entry in `delinks.txt`. The link now uses your object instead of the original bytes.
2. Run the review pass (`.claude/skills/tenets-review/`): names, types, no magic offsets, comments only where the shape is odd, plain writing.
3. `make release`. It fails if any module or the whole ROM differs from retail.
4. Commit. The hooks run `make release` again, rewrite `PROGRESS.md` and stage it, and check the message.

## ARM7

ARM7 (the second CPU: sound, touch screen, wireless, power) is one module, `arm7`, in `config/AWRE/arm7/`. It has three parts with their own `.text`: the crt0 at 0x02380000, an autoload into main RAM at 0x027c0000 and one into ARM7 WRAM at 0x037f8000. Names are `func_arm7_XXXXXXXX` and `data_arm7_XXXXXXXX`. All of it is SDK/library code (`docs/awre-facts.md`, "ARM7").

- C goes under `src/arm7/sdk/` (`src/arm7/game/` only if game code ever shows up). Files there compile with the ARM7 flags, mwccarm 2.0-72 `-O4,s -proc arm7tdmi -interworking`: built for size, unlike ARM9's `-O4,p`.
- `fn.py show/add/check/diff/finish/rename` work as above. `diff` uses `build/arm7/objdiff.json` and the objects in `build/arm7/`; `add` ends a file at the end of the function's own part.
- `fn.py pick --arm7` and `pick --families --arm7` list ARM7 functions; without `--arm7` they leave ARM7 out.
- `propagate.py --module arm7` copies matched C, ARM9's included, to ARM7 twins and checks them with the ARM7 flags; `trivial.py --arm7` does the short functions. Without those flags both leave ARM7 alone. `permute.py` and `pragmas.py` pick the ARM7 flags for a file under `src/arm7/`.
- The 25 Thumb functions are BIOS call stubs (`svc N; bx lr`); write them as `asm void` under `#pragma thumb on`, as for ARM9's.
- dsd cannot delink ARM7 on its own: `tools/arm7.py` runs it on a stand-in ROM with the parts moved (`docs/tool-issues.md`). That is invisible in the loop, but `config/AWRE/arm7/` uses real addresses while `build/arm7/config/` holds the moved copy; edit only the first.
## Shifted build and pointer checks

A mod that changes code size moves every later address. The linker fixes an address only when it is a relocation (an entry in `relocs.txt` or a symbol in our C), never a plain number, so a pointer dsd missed keeps its old value and points at the wrong place. These tools look for that (issues 0012, 0019, 0023). None of them changes what `make release` checks.

- `make shift` (`tools/shift_build.py [--pad N] [--at ADDR]`) links a copy of the build with N zero bytes (default 0x20, a multiple of 32) inserted in main's .text before the first object at or after ADDR (default 0x02004000), and builds `build/shift/AWRE.nds`. It checks that every main symbol after the pad and every overlay symbol moved by N and that the rest stayed. `dsd rom build` itself rewrites the .bss bounds in main's module parameters (a table crt0 reads at 0x02000b00) and the overlay table. The secure-area CRC stays the retail value and is wrong for the shifted ROM (issue 0056).
- `make emu-smoke` (`tools/emu_smoke.py ROM...`) starts retail, `build/AWRE.nds` and the shifted ROM in DeSmuME for 20 seconds each, with no window and no sound, and compares the exit status, the log and the save file the game formats on first boot. It catches a ROM that dies early, not much more: a ROM with 16 KiB of main's code overwritten passed it. See issue 0006.
- `python3 tools/scan_pointers.py [--list high|medium|low|all] [--module M] [--tsv FILE]` lists 4-byte words in .data, .rodata and literal pools whose value falls in a module's address range and that have no relocation, ranked by confidence (the docstring has the rules). Needs `make fnindex`. Add a relocation by hand only after reading the code that uses the word: most low-confidence hits are byte tables such as `04 00 01 02`.
- `python3 tools/resolve_overlay_relocs.py [--write]` narrows relocations that dsd marked `module:overlays(...)` because several overlays share the target address. Run it after anything that regenerates `relocs.txt`, then `make release`.

## Reference

| Command | What it does |
|---|---|
| `python3 tools/fn.py show NAME\|ADDR` | Location, callers, table references and original disassembly |
| `python3 tools/fn.py diff NAME` | Compile, diff, exit 0 on a full match |
| `python3 tools/fn.py check FILE.c NAME` | Compile one file, compare one function with the index, exit 0 on a match |
| `python3 tools/pragmas.py FILE.c NAME` | Score one function under each optimisation pragma |
| `python3 tools/triage_partials.py --out FILE.md` | Classify the unlinked functions' diffs, with pragma sweep and pragma'd neighbours |
| `python3 tools/protocheck.py [FILE.c ...]` | Declarations and calls in the unlinked files that disagree with the callee's definition or header; `--all` for every file |
| `python3 tools/fn.py pick --families` | Rank unmatched same-shape families by member count |
| `python3 tools/fn.py pick --arm7` | Candidates in ARM7 (also with `--families`) |
| `make fnindex` | Build `build/fnindex.json` |
| `python3 tools/ds.py show STRUCT\|data_ADDR` | Struct layout, or a data symbol's type, references and decoded bytes |
| `python3 tools/ds.py field STRUCT.FIELD` | Functions that use a field, the original loads and stores, signedness check |
| `python3 tools/ds.py dupes [--kind K]` | Likely duplicate types, extern clashes and repeated constants |
| `make dsindex` | Build `build/dsindex.json` |
| `python3 tools/tests/test_fncheck.py` | Check that `check` agrees with `diff` |
| `make` | Build `build/AWRE.nds` |
| `make check` | dsd module check, per-module SHA-1, whole-ROM SHA-1 |
| `make progress` | Matched functions and bytes per CPU: game vs SDK, C vs assembly |
| `python3 tools/progress.py --write PROGRESS.md` | Same counts per module, plus C files not linked yet; the pre-commit hook runs it |
| `make shift` | Build `build/shift/AWRE.nds` with padding in main and check that symbols moved |
| `make emu-smoke` | Start retail, our ROM and the shifted ROM in DeSmuME and compare what can be seen without pixels |
| `python3 tools/scan_pointers.py` | Words that look like pointers but have no relocation |
| `python3 tools/resolve_overlay_relocs.py` | Narrow `overlays(...)` relocations to one overlay where the rules allow |
| `ninja objdiff.json` | objdiff project file, for the objdiff GUI if you install it |
