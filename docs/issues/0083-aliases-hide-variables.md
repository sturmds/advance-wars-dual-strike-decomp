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

## Open items

These need a decision from sturmds and are not part of this issue's changes yet:

- A tenet 11 in `TENETS.md` about sizes and aliases, and a matching check in the tenets-review skill.
- The audit of all 200 aliases.
- A `make shift` run with a grown struct, to see whether the shiftable build catches a hidden alias that moves.

## Done when

`gSoundStreamReady` is a variable of its own, `bss.py plan` refuses an alias on a trailing `[1]` element and any alias not in `aliases.txt`, the report step checks `aliases.txt` against `symbols.txt`, and every alias in `aliases.txt` has a group other than `unsorted`.
