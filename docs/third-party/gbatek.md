# GBATEK

- **Project:** GBATEK, the GBA and Nintendo DS hardware reference.
- **Author:** Martin Korth (nocash).
- **URL:** https://problemkaputt.de/gbatek.htm
- **Version we read:** the single-page HTML edition, fetched 2026-10-12.
- **License:** none stated; the page carries a copyright notice by its author.

## What we take

Facts and names only, each checked against our ROM:

- Hardware register names and addresses (for example `DISPCNT`, `BG0CNT`, `MTX_SCALE`), used in `REG_` macros and comments.
- How the hardware behaves (the 3D geometry commands, DMA, the BIOS calls, the wireless registers), described in our own words in comments.

No text, tables or code are copied from GBATEK. Commits whose names come from it say "Register names from GBATEK".
