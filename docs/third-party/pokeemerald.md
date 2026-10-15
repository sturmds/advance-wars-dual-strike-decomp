# pokeemerald (MusicPlayer2000 driver)

- **Project:** pokeemerald, pret's decompilation of Pokémon Emerald. Its `src/m4a.c`, `src/m4a_1.s`, `src/m4a_tables.c` and `include/gba/m4a_internal.h` hold the MusicPlayer2000 (m4a) sound driver that many GBA games link.
- **URL:** https://github.com/pret/pokeemerald
- **Version we read:** `master` at 731ad5bfd (2026-10-01), on 2026-10-15.
- **License:** none stated (GitHub reports none).

## What we take

Facts about how the driver behaves, used by `tools/m4a.py` and described in `docs/m4a.md`, each checked by converting and rendering the songs of a GBA ROM:

- The song header, track, voice (`ToneData`) and sample (`WaveData`) layouts and the voice type bits.
- The command bytes, their arguments, running status (repeated for commands from 0xbd up), the optional key, velocity and gate bytes of a note, and the wait and note lengths (`gClockTable`).
- That a TIE is a note of length 0 that an EOT stops by key; that a drum kit's voice plays at its own key with its own pan, and a key split's at the track's key.
- The pitch sum of `TrkVolPitSet` (BEND x BENDR, TUNE, KEYSH, the LFO), the volume and pan sum, and the LFO (a triangle stepped once per tick, MOD scaling, the delay counted in ticks and restarted by each note).
- The DirectSound envelope (attack adds, decay and release multiply each frame) and the GB channels' (one of 15 steps every n frames, `CgbSound`).
- That a sample's rate field is its rate in Hz x 1024 at key 60 (`MidiKeyToFreq`), and the GB pitch for a key (`MidiKeyToCgbFreq`).
- The track defaults at the start of a song (BENDR 2, LFO speed 22).
- The code at the start of `m4aSongNumStart`, which `tools/m4a.py` searches for, and the bits of `m4aSoundMode`'s argument.

No code or text is copied. The 30-byte search pattern comes from the machine code of `m4aSongNumStart` as GBA games hold it, checked against Advance Wars 2.
