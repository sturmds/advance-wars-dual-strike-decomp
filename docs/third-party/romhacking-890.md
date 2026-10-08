# romhacking.net document 890

- **Title:** *Advance Wars Dual Strike - Disassembly (USA)*
- **Author:** RadioTails.
- **Version:** 0.3, released 2022-07-11, last modified 2023-05-24.
- **URL:** https://www.romhacking.net/documents/890/
- **License:** no license stated.
- **Archive we read:** `awds-us-disassembly.7z`, SHA-256 `b6b3d7b579a8532e9308133ef61b75adf26a667a6ce52719ef216806fda7c367`. It stays outside the repo.

It is an armips ROM-hacking project for the US cartridge, not a full disassembly. We checked it against our `AWRE` extract before using it: their copies of original code and their data tables match our bytes.

## What we take

Facts only, each one checked against our ROM:

- Function and data names. `config/AWRE/names-provenance.csv` lists each imported name with `source` = `rh890:<file>:<line>` (paths inside their archive).
- The CO record layout (0x220 bytes: header and three power blocks) and the unit stat-change row, in `include/game/co_data.h`.
- The map header layout (0xa0 bytes per map, table in ov000), in `include/game/map_header.h`.

## What we do not take

- Their assembly, comments or files. Nothing from the archive is copied into this repo.
- Values from their `include/` files and modified data, which they renumbered or changed for their mod.

Each struct taken from the document carries one comment at its definition that credits it. Commits that import names carry the line `Names from romhacking.net document 890 (RadioTails)`.
