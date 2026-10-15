# 0094: Parts of a GBA song that tools/m4a.py converts loosely

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-15
- **Milestone:** later
- **Related:** 0093 (sound heap)

## Problem

`tools/m4a.py import` (`docs/m4a.md`) converts a MusicPlayer2000 song into DS sound files. Some parts have no exact DS counterpart, and these were not measured against the GBA:

- The level of a GB channel (square, wave, noise) against a DirectSound channel: notes on GB voices get a quarter of their level (`CGB_LEVEL`), a rough figure.
- The noise channel's pitch and 7-bit mode, and square 1's sweep (dropped).
- REPT with a count, MEMACC, XCMD and PORT are dropped with a warning; PRIO is ignored.
- A TIE that lasts different times on different passes and overlaps other notes keeps its first length. Converting all of Advance Wars 2's songs gives this in 4 songs, and 12 places where a note reached twice gives different DS commands.
- LFO speed and delay use the song's first tempo; m4a's LFO runs per tick, so it follows tempo changes.
- `render` mixes without the driver's reverb or interpolation, and its GB channels are approximate, so it is not a reference for those.

## Notes

- Advance Wars 2's songs: all 209 convert; the warnings are counted by running `import` on each (`M4A_TEST_ROM` makes `test_m4a.py` convert them all).
- To measure the GB level: render a song with GB voices in a GBA emulator and with zxtune from the converted archive, and compare the levels of a GB track and a DirectSound track.

## Done when

The GB level is measured and set from the measurement, and each remaining approximation is either fixed or listed in `docs/m4a.md` as checked.
