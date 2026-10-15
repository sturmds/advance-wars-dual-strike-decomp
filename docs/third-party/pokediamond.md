# pokediamond (Nitro sound driver)

- **Project:** pokediamond, pret's decompilation of Pokémon Diamond. `arm7/lib/src/SND_*.c` and `arm7/asm/wram2.s` hold the ARM7 half of the Nitro SDK's sound driver.
- **URL:** https://github.com/pret/pokediamond
- **Version we read:** 5bc4b1a3 (2026-09-10), on 2026-10-15.
- **License:** none stated.

## What we take

Nothing that is not also in this game's own ARM7 code. We read it to understand the DS sound driver for `tools/m4a.py` (`docs/m4a.md`), and took each value from our own decompiled driver in `src/arm7/sdk/snd/`, which agrees:

X, the attack table and the decay rate formula.
- The envelope steps, the LFO (counter, speed, depth x range, the shift to 1/64 semitone), the pitch bend (bend x range / 128 semitones), the PSG and noise timer at the root key.
- The sequencer: tie mode (0xc8) releases the track's notes and reuses the tied channel; transpose is added before the instrument is chosen; a key split region holds keys up to its bound, at most 8 regions.

No code or text is copied.
