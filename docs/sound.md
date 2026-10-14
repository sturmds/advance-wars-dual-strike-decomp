# Sound data

The music and sound effects are one file in the ROM's file system, `files/data/sound_data.sdat` (18,904,864 bytes). It is a sound archive (SDAT) of NitroSystem, the sound library in Nintendo's DS SDK; the game opens it through the `nns_snd_arc_*` code in `src/game/main/sound/`. The archive is game data, so it never enters git (tenet 9). `make setup` unpacks it from your own ROM into `extracted/AWRE/assets/sound/` (gitignored), and `tools/sound.py pack` rebuilds it byte for byte. The ROM build does not use `pack` yet: it copies `sound_data.sdat` from `extract/` like every other file (issue 0050). `make music` renders every song and sound effect to MP3 ("Export" below). Issue 0085 covers how the game plays the music.

The format facts below come from public descriptions, checked against our ROM (`docs/third-party/sdat-format.md`).

## What the archive holds

| Kind | Entries | Files | What it is |
|---|---|---|---|
| `seq` (SSEQ) | 249 | 85 | Sequences: scores in a MIDI-like command stream. Names start with `BGM_` (music) or `SE_` (sound effects). |
| `seqarc` (SSAR) | 29 | 29 | Sequence archives: several short sequences in one file, each with its own name in SYMB. All are sound effects. |
| `bank` (SBNK) | 205 | 204 | Instrument banks: which sample, envelope and key range each instrument plays. Each names up to 4 wave archives. |
| `wavearc` (SWAR) | 292 | 292 | Wave archives: the samples. |
| `player` | 3 | - | Sequence players: how many sequences each plays at once and its heap size. Sequences pick one with `playerNo`: 67 use player 0 (music), 182 use players 1 and 2 (sound effects). |
| `group` | 0 | - | Groups of entries to load together. None. |
| `player2` | 1 | - | The stream player: the two hardware channels (6 and 7) a stream plays on. |
| `strm` (STRM) | 1 | 1 | Streams: recorded audio played from the cartridge. The only one is named after the staff roll. |

Every entry has a name in SYMB; the SDK can strip them, but this ROM keeps them. The names are the developers' own, so the music of a CO is filed under the Japanese name of that CO. `tools/sound.py list --grep NAME` finds a song by part of its name.

Several entries can use one file. 116 sound-effect sequences use the same 40-byte sequence, and 8 more files are shared by 2 to 15 sequences: such a sequence plays one note, and most of the entries that share it name different banks, so the bank decides the sound. Two banks share one file. So the archive has 611 files for 776 entries.

## Layout

All numbers are little-endian. Offsets inside a block count from the start of that block, except the FAT, which gives offsets from the start of the archive.

| At | Size | What |
|---|---|---|
| 0x0 | 0x40 | Header: `SDAT`, byte order mark 0xfeff, version 0x100, archive size, header size 0x40, block count 4, then offset and size of SYMB, INFO, FAT and FILE, then 16 zero bytes. |
| 0x40 | 0x4b84 | SYMB: names. |
| 0x4bc4 | 0x2718 | INFO: one entry per sequence, bank, player and so on. |
| 0x72dc | 0x263c | FAT: 611 file entries. |
| 0x9918 | 0x11fde08 | FILE: the files. |

- **SYMB and INFO** start alike: magic, block size, 8 record offsets (seq, seqarc, bank, wavearc, player, group, player2, strm), 24 zero bytes. A record is a count and that many offsets; offset 0 means no entry. A seqarc record in SYMB has pairs instead: the archive's name and the offset of a sub-record with the names of the sequences inside it.
- **SYMB** holds the 8 records in order, the seqarc sub-records right after the seqarc record, then the strings, each ended by a zero byte, in the order the records name them (an archive's name before its sequences' names). The header gives SYMB's size without padding (0x4b81); the block's own size field rounds it up to 4 (0x4b84), and 3 zero bytes follow.
- **INFO** holds each record followed by its entries, in kind order. The entry layouts are the structs in `include/game/sound_arc.h`: `SoundArcSeqInfo` (12 bytes), `SoundArcBankInfo` (12), `SoundArcPlayerGroupInfo` (8), `SoundArcStrmInfo` (12, with 4 zero bytes after it); a seqarc entry is a file id (4 bytes), a wave archive entry a 24-bit file id with a flags byte on top (all 0 here), a player2 entry a channel count, 16 channel bytes (0xff when unused) and 7 zero bytes. In `SoundArcInfo` the record offset at INFO 0x20 is `strmPlayerOffset` (the stream player entry) and the one at 0x24 is `strmInfoOffset` (the streams).
- **FAT**: `FAT `, block size, count, then per file its offset from the start of the archive, its size and 8 zero bytes. The files are in id order: sequences, sequence archives, banks, wave archives, the stream.
- **FILE**: `FILE`, block size, file count, a zero word, then the files in FAT order. Each file starts on a 32-byte boundary of the archive, after zero bytes; zero bytes after the last file round the archive up to 32.

## Files

```
extracted/AWRE/assets/sound/    gitignored, written by `make setup` (or `make sound`)
  sound.yaml                    every INFO entry, in id order
  seq/NNN_NAME.sseq
  seqarc/NN_NAME.ssar
  bank/NNN_NAME.sbnk
  wavearc/NNN_NAME.swar
  strm/NN_NAME.strm
  other/NNN.bin                 a file no entry uses (none in this ROM)
```

Each file is the bytes of one FAT entry, as the archive holds it. It is named after the first INFO entry that uses it, with that entry's id. A shared file is written once; the other entries name the same file in `sound.yaml`.

`sound.yaml` has one list per kind, one line per entry:

```yaml
seq:
- {id: 0, name: BGM_EXAMPLE, file: seq/000_BGM_EXAMPLE.sseq, fileId: 0, bankNo: 3, volume: 127, channelPrio: 64, playerPrio: 64, playerNo: 0}
seqarc:
- {id: 0, name: SEQARC_SE_EXAMPLE, file: seqarc/00_SEQARC_SE_EXAMPLE.ssar, fileId: 85, sequences: [SE_ONE, SE_TWO]}
bank:
- {id: 0, name: BANK_EXAMPLE, file: bank/000_BANK_EXAMPLE.sbnk, fileId: 114, waveArc: [0, 55]}
```

(Made-up names and values.) The field names are the C names in `include/game/sound_arc.h`. `pad_` fields and `flags` are left out when they are 0. `waveArc` leaves out unused slots at the end and writes `null` for one in the middle. `fileId` decides the file's place in the FAT, so two entries with one file id must name the same file.

## Commands

- `tools/sound.py unpack` reads `extract/files/data/sound_data.sdat` and rewrites `extracted/AWRE/assets/sound/`. It removes the kind directories first, so edits there are lost.
- `tools/sound.py pack` writes `build/sound/sound_data.sdat` from those files and `sound.yaml`, with the layout above. With unchanged files it gives the ROM's archive byte for byte. It stops on a field an entry does not have, a file id with two files, or a file id no entry uses.
- `tools/sound.py check` unpacks into a temporary directory, packs, and compares the SHA-1 with the ROM's archive. `tools/tests/test_sound.py` (part of `make test`) runs it when `extract/` exists, and a round trip of a made-up archive in any case.
- `tools/sound.py list [--kind seq] [--grep REGEX]` prints each entry's id, name, bank, wave archives and file size, straight from the archive in `extract/`.

A changed file changes the archive's size and moves the files after it, so the ROM no longer matches retail. Such a build belongs in the mod build (issue 0023), not in a commit here.

## Export

`make music` (or `tools/sound.py export`) writes every sequence and the stream as an MP3 file:

```
extracted/AWRE/music/           gitignored
  bgm/NN Title.mp3              the 67 songs on player 0 and the stream, titled as in the Sound Room
  se/NNN_NAME.mp3               the 182 sound effects that are sequences, under their SYMB names
  se/NN_ARCHIVE/NNN_NAME.mp3    the 113 sound effects inside sequence archives, under their SYMB names
```

How it works:

- zxtune123 (`tools.lock`, `docs/third-party/zxtune.md`) renders WAV. It plays NCSF files: PSF files of version 0x25 whose program is an SDAT and whose reserved area holds a sequence number. `export` writes one library file, `build/sound/ncsf/sound.ncsflib`, with the archive in it, and a 64-byte `NNN.minincsf` per sequence that names the library, the sequence number, the length and the fade. The stream is rendered from its own STRM file. ffmpeg (a system package, `sudo apt install ffmpeg`) encodes the WAV; `--format ogg|flac|wav` picks another format.
- An NCSF file names a sequence by number only, so a sound effect inside a sequence archive cannot be named directly. The library is our archive with one more sequence per such sound effect: an SSEQ holding the archive's events from that sound effect's start. That is only right when nothing it runs jumps by offset (open track, jump, call). In this ROM nothing does, and `export` stops if one does.
- Lengths are fixed per kind: songs 2:30 with a 10-second fade, sound effects 12 seconds with a 2-second fade. Every sound effect that ends does so within 9.1 seconds; the 42 that loop (engines, unit moves, quakes) fade out. 4 sound effects render as silence (counter and setup cues). The files are what the sound engine plays: `export` does not look for loop points or trim silence.
- Song titles come from the Sound Room list in ov005 (`data_ov005_0236b10c`, 67 cells) and the game text it points to (`gTextBanks` in ov000). Song 55 has no cell and keeps its SYMB name.
- Rendering runs on half the CPUs, two per zxtune123 process; on 16 cores the 363 files take about 100 seconds and 234 MB. `extracted/AWRE/music/.stamp` holds a hash of the archive, the two overlays, zxtune's SHA-256 and the settings; while it matches, `export` does nothing. `--force` renders again.
- `make assets` (part of `make setup` and `make release`) runs `export --auto`, which skips in a linked worktree and when ffmpeg is missing, so a first build needs neither. `make music` exports in any checkout.

## Keeping it out of git

Audio from the ROM stays in `extracted/` or `build/`, in any form: the archive's files, `sound.yaml` (it lists the names), and anything converted from them (MIDI, WAV, MP3, Ogg, FLAC, SoundFont, NCSF; an NCSF library holds the whole archive). `.gitignore` ignores those extensions anywhere, and `hooks/pre-commit` refuses `.sdat`, `.sseq`, `.ssar`, `.sbnk`, `.swar`, `.strm`, `.wav`, `.mid`, `.mp3`, `.ogg`, `.flac`, `.sf2` and NCSF (`.ncsf`, `.minincsf`, `.ncsflib`) files as well as anything under `extracted/` and `build/`. Never paste names or numbers from `sound.yaml` into a committed file beyond what this page gives as facts about the format.

## Python packages

`tools/sound.py` needs PyYAML (`python3-yaml` on Ubuntu), like `tools/maps.py`.
