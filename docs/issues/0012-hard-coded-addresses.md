# 0012: Hard-coded addresses would break a shifted build

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-07
- **Milestone:** M3

## Problem

A matching build can contain addresses written as numbers instead of symbols. They still match, but once code or data moves (modding), they point at the wrong place.

## Notes

- Known gap: none in our code yet. The `.rodata` range blanked for `dsd init` had 29 real pointers, now restored (issue 0007).
- Data tables are the likely problem: pointers inside `.data`/`.rodata` that dsd did not find.
- Plan M3 test: build with padding inserted in one module, then run the emulator scenario (issue 0006).
- The tenets review flags `0x02xxxxxx` literals in C.

## Done when

A build with padding inserted in main runs the M3 scenario the same as retail.
