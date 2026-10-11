# 0053: Two data symbols with the same name break the arm9 checksum during a replay

- **Status:** open
- **Area:** loop
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

Batch 21 group `s24` added a data symbol for a `/bmap` path string while mainline already had another symbol with the same name for a second `/bmap` string. The replay carried both names into `symbols.txt`, the build still linked, and the arm9 module checksum failed in the middle of the replay. The agent found it by hand and renamed one symbol to `gPowerHighlightDir`.

Nothing in `apply_commit.py` or `fn.py rename` checks that a new name is unused in `symbols.txt`.

## Workaround

When the checksum fails after a replay step that adds data symbols, look for two entries with one name in `config/AWRE/arm9/symbols.txt` and rename one.

## Ideas

- Make `fn.py rename` and the replay refuse a name that another symbol already has (in any module).
- Run the same check as a line in `make check`, before the hashes, so the message names the symbol.
- Related: 0025 (landing tool faults), 0016 (renames not replayed).
