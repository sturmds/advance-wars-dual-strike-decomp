# 0056: A ROM with changed main code has a wrong secure-area CRC

- **Status:** open
- **Area:** build
- **Opened:** 2026-10-11
- **Milestone:** M3

## Problem

The cartridge header holds a CRC of the secure area (main's first 16 KiB, ROM 0x4000-0x7fff) at 0x6C. `dsd rom build` can compute it only with an ARM7 BIOS dump, so `tools/fix_header.py` copies the retail value, after checking that the secure area did not change (docs/tool-issues.md).

Any mod that grows or shrinks main changes the secure area: it holds crt0, the module parameters and pointers to later code. In `make shift` with a 0x20-byte pad, 420 bytes of it differ. `tools/shift_build.py` therefore writes the retail CRC anyway and recomputes only the header CRC, so the shifted ROM has a CRC that does not match its secure area.

## Notes

- DeSmuME 0.9.13 boots the shifted ROM without complaint (issue 0006). Other emulators, flash carts or real hardware may check the CRC.
- Fix options: give `dsd rom build` an ARM7 BIOS dump (`--arm7-bios`; a dump from sturmds's own DS, never committed), or compute the CRC ourselves, which needs the same key.
- 2026-10-14 (issue 0086): checked with grown dtcm, itcm and ov001 builds. Everything else in the header (sizes, entry points, offsets, ROM size, header CRC) is recomputed by dsd; this CRC is the only one left.

## Done when

A shifted or modded ROM gets a correct secure-area CRC, or we check that the targets sturmds cares about ignore it.
