# 0019: Relocations marked `overlays(0,4)` that can only be ov000

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-08
- **Milestone:** M3

## Problem

ov000 and ov004 load at the same address (`0x022ad560`). dsd marks 804 relocations in main `module:overlays(0,4)` because it cannot tell which one a pointer means. ov004 ends at `0x022bf140`, so every target above that is in ov000: the map header table (`Ov000_gMapHeaders`), unit stats (`Ov000_gUnitStats`), terrain and movement tables and text all sit there. The matching build does not care, but a shifted build (issue 0012) would resolve these wrong.

## Notes

- A small script can rewrite the `module:` field of those lines in `config/AWRE/arm9/relocs.txt` to `overlay(0)` when the target is past ov004's end, then `make release` checks it.
- The ov000 symbols near these targets are marked `ambiguous` in `symbols.txt` for the same reason.

## Done when

No relocation with a target above ov004's end is marked `overlays(0,4)`.
