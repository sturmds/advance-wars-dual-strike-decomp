# SSEQPlayer (CaitSith2)

- **Project:** SSEQ Player, a homebrew DS player of the SSEQ format. Not the SSEQPlayer inside zxtune (`docs/third-party/zxtune.md`).
- **URL:** https://github.com/CaitSith2/SSEQPlayer
- **Version we read:** `master` (last pushed 2011-11-24), `arm7/source/sndbase.arm.c`, `arm7/source/sseq.arm.c` and `common/sndcommon.h`, on 2026-10-15.
- **License:** none stated.

## What we take

Nothing that is not also in this game's own ARM7 code (`src/arm7/sdk/snd/`). We read its envelope, volume and modulation code to learn how the DS's attack, decay, sustain and release values turn into levels over time, and checked each fact against our driver before `tools/m4a.py` used it (`docs/m4a.md`, "Envelopes"). No code or text is copied.
