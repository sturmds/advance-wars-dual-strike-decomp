# 0083: `.bss` aliases can hide separate variables

- **Status:** in progress
- **Area:** tools
- **Opened:** 2026-10-13
- **Milestone:** M2

## Problem

`tools/bss.py` makes every symbol inside the declared size of the variable before it an alias: `symbols.txt` gives it size 0, the linker script defines it as `NAME = VARIABLE + OFFSET;`, and its C users keep only an `extern`. The tool never asks whether the declared size is right. When a type is too large, two unrelated variables end up sharing memory in the source, and nothing reports it: the ROM still matches, and objdiff scores the unit 100% because the alias has size 0.

## Evidence

`SoundWaveArc` (`include/game/sound_wave_arc.h`) ends in `u32 table[1]`, so `sizeof(SoundWaveArc)` is 0x40. The wave table has `count` entries; the `[1]` only stands for "a table follows". `SoundArc_LoadWaveArcTable` (`src/game/main/sound/sound_arc_load_wave_arc_table.c`) reads `SOUND_WAVE_ARC_HEADER_SIZE` (0x3c) bytes into the global `gWaveArcHeader` and never reads or writes its `table`.

`config/AWRE/arm9/symbols.txt` has:

```
gWaveArcHeader kind:bss(size=0x40) addr:0x022a8a64
gSoundStreamReady kind:bss(size=0x0) addr:0x022a8aa0
```

so the linker script gets `gSoundStreamReady = gWaveArcHeader + 0x3c;`, and `src/game/main/sound/sound_stream_init.c` only declares `extern BOOL gSoundStreamReady;`. The flag set by `SoundStream_Init` has nothing to do with the wave archive header, but the source places it on the first entry of the header's table. If `SoundWaveArc` changes size, the flag stays at header + 0x3c while the variables after the header move: the ROM check fails, and nothing in the source points at the flag.

There are 200 aliases of size 0: 164 in main, 26 in ARM7, 10 in ov003. Some are real addresses inside a variable (`&array[1]`, a field), some may be hidden variables like this one.

### Shift pass (2026-10-14)

`aliases.txt` has 310 aliases, all `real`. The 141 not checked above (139 that `data.py` added for .data/.rodata, plus `data_02175180` and `data_02175198`) were compared with their base's C type: no hidden variable. Two notes for issue 0086: `data_02175180`/`data_02175198` are crt0's autoload list, which lands inside `data_02173788` by chance; and an alias on element 1 or later of an array goes wrong when the element size changes, so a mod-safe alias needs its offset computed from the C type.

## Fix

1. This case: give `gWaveArcHeader` a type of 0x3c bytes (`u32 table[]`, if mwccarm 2.0-72 accepts it; otherwise a separate header struct), set `gWaveArcHeader` to `size=0x3c` and `gSoundStreamReady` to `size=0x4` in `symbols.txt`, and define the flag in C. Every function that uses `SoundWaveArc` must still score 100% in `fn.py diff`.
2. `bss.py plan` refuses a new alias that sits on the last element of a `[1]` array that ends its variable's type, as it already refuses an alias past the declared size.
3. A ratchet: `config/AWRE/aliases.txt` lists every alias as `<symbol> = <base>+<offset>` with a group. It starts with the 200 existing aliases marked `unsorted`. `bss.py plan` refuses an alias that is not listed, and the decomp.dev report step fails when the list and `symbols.txt` disagree. The format and the groups go in `docs/bss-tool.md`.

## Audit

The 200 aliases each go into one group:

- `real <field>`: a real address inside the variable, which code takes on purpose (an element or a field).
- `hidden`: a separate variable that the variable before it swallows. Fix as for `gSoundStreamReady`: shrink the type, give the variable a size and a definition.
- `unsorted`: not checked yet.

Check first the variables whose type ends in a `[1]` array, the shape that caused this case: `include/game/sound_bank.h` (`instruments[1]`), `sound_arc.h` (`seqs`, `offset`, `entries[1]`), `sound_heap.h` (`payload[1]`), `ov003_unit.h` (`spawns[1]`), `Unk_ov004_022bf158.h` (`unk_998[1]`), `BattleSideData.h`, `battle_fighter.h`, `menu_state.h` (`unk_264[1]`), `Unk_02080440.h`, `map_data.h`, and the SDK headers `wl.h`, `wm.h`, `mb.h`, `snd_seq.h` and `wmsp.h` where a global has one of their types.

### First pass: types that end in `[1]` (2026-10-13)

A read-only pass over the declared types of the 79 variables the 200 aliases point into. Apart from `gWaveArcHeader`, no such variable has a type that ends in a `[1]` array, directly or through its last field: the structs that do (`SoundBank`, `SoundHeapItem`, `MapData`, `SndWaveData`, the `Wl*` and `WM*` buffers, and others) are only reached through pointers or locals, and in `Ov003State` (`spawns[1]`), `Unk_ov004_022bf158` (`unk_998[1]`) and `MoveSearch` a pad field follows the `[1]` array. `ResultScreenWork` (`unk_264[1]`) is only laid over `gMenuState`, and no alias lies between its 0x264 and the next alias at 0x400. So this pass found no other hidden variable.

The aliases whose variable has a type from the headers listed above, with the group the pass suggests. `aliases.txt` still marks them `unsorted` until sturmds confirms:

| Alias | Variable | Offset | Suggested group | Reason |
|---|---|---:|---|---|
| `data_0229eb70` | `gMenuState` | 0x40 | real (screen work area) | `menu_state.h` says it points inside `MenuState`; `result_stats_init.c` copies a 0x5c0-byte layer map there. |
| `data_0229ed30` | `gMenuState` | 0x200 | real (`MenuStateFrom200`) | Two VS setup files read `.fundsPerProperty` through it. |
| `data_0229ef30` | `gMenuState` | 0x400 | real (`MenuStateFrom400`) | The map select files read `.prevMapId` and `.firstMapId` through it. |
| `data_0229f330` | `gMenuState` | 0x800 | real (layer 0 map) | The Tag Break code unpacks a 0x800-byte layer 0 map there and copies it to VRAM. |
| `data_0229fb30` | `gMenuState` | 0x1000 | real (`Unk_0229fb30`, palettes) | The Tag Break code copies two palettes there; combat setup reads `.unk_45a`, inside `MenuState.combat`. |
| `gTagBreakSparkPalette` | `gMenuState` | 0x1020 | real (second palette at 0x1000) | `tag_break_load_thunder.c` names it the second of the two palettes. |
| `data_0229fb70` | `gMenuState` | 0x1040 | real (gauge colours) | `tag_break_set_up_screens.c` copies 0x20 bytes of colours there. |
| `gTagChangePalette` | `gMenuState` | 0x1060 | real (saved palette) | `func_020593c8.c` and `tag_break_set_up_screens.c` use it as saved palette bytes. |
| `data_0229ff30` | `gMenuState` | 0x1400 | real (`Unk_0229ff30.playerMask`) | Combat setup reads and writes `.playerMask`. |
| `data_022a0030` | `gMenuState` | 0x1500 | real (CO EXP screen work) | `co_exp_init_screen.c` writes an `s16` at +0xae; `MenuState` reaches to 0x1630. |
| `data_0229c838` | `data_0229c768` (`MapListTables`) | 0xd0 | real `unk_0d0` | The map select code reads `TABLES.unk_0d0`. |
| `data_0229c86c` | `data_0229c768` | 0x104 | real `listOfTab` | `map_select_draw_sub_strip.c` reads `data_0229c768.listOfTab[...]`. |
| `data_022ac380` | `gWmConnectPortArg` | 0x14 | real `macAddress` | `wm_receive_fifo.c` copies 6 bytes into `.macAddress`. |
| `data_022ac390` | `gWmConnectPortArg` | 0x24 | real `ssid` | The same function copies or clears 24 bytes at `.ssid`. |
| `data_022ace68` | `data_022ace60` (`DlPlayRosterFrame`) | 0x8 | real `checksum` | `mb_beacon_send_players.c` writes `.checksum` and passes its address to `MbBeacon_Checksum`. |
| `data_022ace6e` | `data_022ace60` | 0xe | real `unk_35c` | The same function writes `.unk_35c`. |
| `data_022ace70` | `data_022ace60` | 0x10 | real `users` | The same function clears and fills `.users[]`. |
| `data_arm7_0380807c` | `data_arm7_03808074` (`SndSeqCache`) | 0x8 | real `words` | `snd_seq_read_byte.c` indexes it as the bytes of the 16-byte window. |
| `data_arm7_027d7d3c` | `data_arm7_027d7c44` (`WmspState`) | 0xf8 | real (stack boundary) | `wmsp_init.c` passes it as the top of one stack and the bottom of the next, both inside the WMSP work area. |
| `data_arm7_027d813c` | `data_arm7_027d7c44` | 0x4f8 | real (`OSMutex`) | A 0x14-byte mutex fills 0x4f8-0x50c up to `unk_50c`; the WMSP reply functions lock it. |
| `data_arm7_027d8160` | `data_arm7_027d7c44` | 0x51c | real `history` | `wmsp.h` says it is `WmspState.history`; `wmsp_push_rssi_sample.c` indexes it. |

Next places to look: eight variables whose size comes from "up to the next symbol" (`gBuildMenuEntries`, `data_0229e5bc`, `data_022a6c84`, `gOsIrqSlots`, `data_022a98ec`, `data_arm7_038078bc`, `gEventTextMap`, `gSlotControllers`) hold 16 aliases at small offsets, and 23 variables are plain `u8`, `u16` or `char` arrays with no layout to check against.

### Second pass: every alias (2026-10-13)

Each alias was checked against its variable's type and the code that loads it (`ds.py show`, `fn.py show`, the relocations). Of the 198 aliases:

- 169 are `real`. 160 lie on a field or element. Nine are bases the compiler picked because a load or store cannot reach the field with its immediate offset (`data_022aaf40`, `data_02290a18`, `data_0229a050`, the four at +0xd00 and +0xe00 in ov003's two tile maps, `data_arm7_027d82b0`, `data_arm7_03809eb0`); their lines say "compiler base". The eight variables whose size runs up to the next symbol hold only real fields.
- 2 hid something. `gTouch` (0x10 bytes) sat at `gTouchDriverState`+0x8, whose real type is an 8-byte `TPCalibrateParam`. `data_02290256` sat on `data_02290250`, a 0x48-byte placeholder with no user that was really the tail of `data_02290118`'s 0x180-byte profile block; `data_02290256` is now `data_02290118`+0x13e, real.
- 11 had no relocation and no user, and are deleted.
- 17 stay `unsorted`: each is reached only by a relocation from main's `.rodata` or `.data`, or ov000's `.rodata` or ov002's `.data`, where the word is not a pointer (u16 tables such as 0x0216/0x0218, packed graphics, a script opcode and argument). They are false symbols: `data_0217f7ff`, `data_02180216`, `data_021a01ca`, `data_021c000e`, `data_021c021a`, `data_021fffff`, `data_022001ee`, `data_0220ee27`, `data_02220202`, `data_02222222`, `data_0222491f`, `data_022401e6`, `data_022610df`, `data_02270226`, `data_02270fdb`, `data_0227f025`, `data_0228443d`. The fix is to delete each relocation and then the symbol, as `7009a8151` and `95e17cfef` did for ov003; that belongs with the false-relocation work of issue 0082.

`WmspState` (`data_arm7_027d7c44`) is marked real but does not fit its type well: `Wmsp_Init` passes `data_arm7_027d7d3c` (+0xf8) as the top of a 0x1000-byte stack that starts 0xf08 bytes before the variable, over the three placeholders before it, and +0x4f8 as the top of a 0x400-byte stack and the reply mutex. The real layout of the WMSP work area around it is still to find.

## Progress

- `gSoundStreamReady` is a variable of its own: `SoundWaveArc` ends in `u32 table[]` (mwccarm 2.0-72 accepts it; `sizeof` is 0x3c), `gWaveArcHeader` has size 0x3c and `gSoundStreamReady` size 4 in `symbols.txt`, and `sound_stream_init.c` defines it as `static BOOL` and takes 0x022a8aa0 into its `.bss` range, by hand: `plan` gives a slice only to a file with no `.bss` yet, so it would have put the flag in a new file. `SOUND_WAVE_ARC_HEADER_SIZE` is now `((int)sizeof(SoundWaveArc))`; the unsigned `sizeof` alone changes the code of `SoundArc_LoadWaveArcTable`. Every function that uses `SoundWaveArc` still scores 100%. 199 aliases are left.
- `bss.py plan` refuses an alias on a trailing `[1]` element and any alias not in `config/AWRE/aliases.txt`; `bss.py aliases` and the report step check the list (`docs/bss-tool.md`, "Aliases").
- `gTouch` is a variable of its own: `gTouchDriverState` has size 0x8 (was 0x18) and `gTouch` size 0x10 in `symbols.txt`; the generated `bss_0216e4b8.inc.c` defines `gTouch` as `TouchState`, and `touch_init.c` declares the calibration as `TPCalibrateParam`.
- `data_02290118` has size 0x180 (was 0x138): `Unk_02290118` ends with `pad_136`, the placeholder `data_02290250` is gone, `bss_022900b8.c` ends at 0x02290298 and `bss_02290250.c` became `bss_02290298.c`.
- 11 aliases with no relocation and no user are deleted. `aliases.txt` holds 186 aliases: 169 real, 17 unsorted (the false symbols above).
- The 17 false symbols are deleted, with their lines in `aliases.txt`. Twelve had lost their last relocation in issue 0082's work on main and ov002. The other nine relocations came from ov000 `.rodata` and are deleted too: four sit inside the LZ77 streams at 0x022cf9b0, 0x022d6af0, 0x022d816c and 0x022d9914 (pictures that `Data_Unpack` reads through `gNumberFontByLanguage` and the picture columns of `Ov000_gTerrainStats` and `data_ov000_023501e8`/`02350278`), and five, all `0x02222222`, sit in the 0x800 bytes of tiles at 0x022dabfc that `PlayerPanels_Init` copies to VRAM with `Mem_CopyAuto`. `aliases.txt` holds 169 aliases, all `real`.
- The screen context buffers have their full sizes: `data_0216ef94` and `data_0216f2c4` are `IconBank[ICON_BANK_COUNT]` (0x330), `data_021700a8` the sub screen's 0x400-byte palette buffer, `data_02172108` and `data_02172c48` `ScriptSlot[SCRIPT_SLOT_COUNT]` (0xb40), and `data_02173788` and `data_02175948` the two `Task[TASK_COUNT]` pools (0x21c0). The 66 symbols dsd had cut them into, with no user and no relocation, are gone. `data_02175180` and `data_02175198`, the bounds of crt0's autoload list that `BuildInfo` names, lie inside the main task pool (the list is in the load image, and crt0 clears `.bss` after the autoload) and are listed as aliases of it.
- `data_02291774`, the save record buffer, is a `SaveRecordBuffer` union of 0x6000 bytes (was `SaveProgress`, 0x1590): `SaveProgressBlocks` (0x1684) and `BattleSaveRecord` (0x2238) use it past 0x1590, so `data_02292d04`, the 0x4a70 bytes after it with no user and no relocation, was its tail and is gone.

## Open items

These need a decision from sturmds and are not part of this issue's changes yet:

- A tenet 11 in `TENETS.md` about sizes and aliases, and a matching check in the tenets-review skill.
- A check of the groups of the second pass by sturmds.
- A `make shift` run with a grown struct, to see whether the shiftable build catches a hidden alias that moves.

## Done when

`gSoundStreamReady` is a variable of its own, `bss.py plan` refuses an alias on a trailing `[1]` element and any alias not in `aliases.txt`, the report step checks `aliases.txt` against `symbols.txt`, and every alias in `aliases.txt` has a group other than `unsorted`.
