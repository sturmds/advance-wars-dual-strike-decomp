# Descriptions of the NitroSystem sound formats

`docs/sound.md` and `tools/sound.py` follow public descriptions of the SDAT archive and of the formats inside it. Every fact we use was checked against our ROM: `tools/sound.py check` rebuilds `sound_data.sdat` byte for byte, and `make music` renders every sequence, including the sound effects cut out of sequence archives. Where our ROM or our C (`include/game/sound_arc.h`) disagrees with a description, `docs/sound.md` follows the ROM and says so. We take facts from these sources, not text or code.

## ndspy documentation

- **Project:** ndspy, a Python library for DS files, by RoadrunnerWMC.
- **URL:** https://ndspy.readthedocs.io/en/latest/ (the `soundArchive`, `soundSequence` and `soundSequenceArchive` pages, and the SDAT appendix)
- **Read:** 2026-10-14.
- **License:** GPL-3.0-or-later for the library (PyPI). We use neither its code nor its package.
- **What we took:** a sequence archive's sequences share one block of events and differ only in their start offset, bank, volume, priorities and player; an offset of 0xffffffff marks a sequence without events.

## Nitro Composer file specification (kiwi.ds)

- **Source:** "Nitro Composer File Specification", first published by kiwi.ds; the description most DS sound tools cite. Incomplete by its own note.
- **URL:** https://retrocdn.net/Nitro_Composer_File_Specification ; the original through the Internet Archive: http://web.archive.org/web/20210614044355/https://sites.google.com/site/kiwids/
- **Read:** not opened by us on 2026-10-14 (RetroCDN refused automated readers, and the Internet Archive was not reachable). The SDAT block layout and the sequence command lengths in `tools/sound.py` follow the layout this description gives, as summarised by the tools that cite it; our ROM confirmed each of them.
- **License:** none known.
