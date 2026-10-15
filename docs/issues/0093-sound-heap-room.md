# 0093: The room an added sound entry takes in the sound heap is not known

- **Status:** open
- **Area:** emulation
- **Opened:** 2026-10-15
- **Milestone:** later
- **Related:** 0023 (mod build), 0085 (music)

## Problem

`docs/sound.md`, "Adding a song", says how a mod adds a sequence, bank or wave archive to the sound archive. `Sound_Init` makes one sound heap of 0xf8000 bytes, `SoundArc_ReadTables` loads the archive's INFO and FAT blocks into it, and `Sound_CreatePlayerHeaps` takes each player's heap (`heapSize` in `sound.yaml`) from what is left. Each added entry makes INFO and FAT larger. How many bytes of the sound heap are free after start-up is not measured, so it is not known how many entries a mod can add before a heap allocation fails, or what the game does then.

A song also has to fit, with its bank and wave archives, in its player's heap; a song larger than the largest the ROM has was not tried.

## Notes

- Tested: one added sequence (a copy of a CO theme, 11,692 bytes, on an existing bank) starts and plays on the press-start screen in melonDS (`docs/sound.md`, "Adding a song").
- To measure: read the sound heap's free size in melonDS after `Sound_Init` (`tools/emu.py`, `expect mem`), on retail and on a ROM with many added entries.

## Done when

`docs/sound.md` gives the free room and what fails first, checked in melonDS.
