# GBA Mus Riper and Sappy

- **Projects:** Sappy, a Windows tool of the GBA homebrew scene that plays and edits MusicPlayer2000 songs, and GBA Mus Riper, a ripper of GBA music to MIDI and SoundFont by Bregalad, built on his Sappy engine detector.
- **URL:** https://battleofthebits.com/lyceum/View/GBAMusRiper (a description; the tool is distributed through Romhacking.net)
- **Version we read:** none; we did not open either tool or its source.
- **License:** not checked.

## What we take

An idea, widely described for these tools: a GBA game's song table can be found by searching for the machine code of the driver's song select function, `m4aSongNumStart`, and reading the table's address from that function's literal pool. `tools/m4a.py` does this with the code as pokeemerald gives it (`docs/third-party/pokeemerald.md`) and the offset of the literal, both checked against Advance Wars 2. No code or text is copied.
