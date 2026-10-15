# Sound data

The music and sound effects are one file in the ROM's file system, `files/data/sound_data.sdat` (18,904,864 bytes). It is a sound archive (SDAT) of NitroSystem, the sound library in Nintendo's DS SDK; the game opens it through the `nns_snd_arc_*` code in `src/game/main/sound/`. The archive is game data, so it never enters git (tenet 9). `make setup` unpacks it from your own ROM into `extracted/AWRE/assets/sound/` (gitignored), and `tools/sound.py pack` rebuilds it byte for byte. `make` packs the archive from those files into the ROM ("Build" below), so an edited or added sequence, bank or wave archive reaches `build/AWRE.nds`; "Adding a song" says how to add one. `make music` renders every song and sound effect to MP3 ("Export" below). Issue 0085 covers how the game plays the music.

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

- `tools/sound.py unpack` reads `extract/files/data/sound_data.sdat` and writes `extracted/AWRE/assets/sound/`. It writes only the files that change, so it overwrites an edited file of the ROM, and leaves a file the ROM does not have. It rewrites `sound.yaml`, so edits to it and entries added to it are lost, as `make setup` does for the maps and the text.
- `tools/sound.py pack` writes `build/sound/sound_data.sdat` (`--out` names another file) from those files and `sound.yaml`, with the layout above. With unchanged files it gives the ROM's archive byte for byte. It writes only a changed archive, and replaces a symbolic link at `--out` instead of writing through it. It stops on a field an entry does not have, an id out of order, a file id with two files, or a file id no entry uses.
- `tools/sound.py check` unpacks into a temporary directory, packs, and compares the SHA-1 with the ROM's archive. `tools/tests/test_sound.py` (part of `make test`) runs it when `extract/` exists, and in any case a round trip of a made-up archive, an added sequence, and the writes of `pack` and `unpack` that leave unchanged files alone.
- `tools/sound.py list [--kind seq] [--grep REGEX]` prints each entry's id, name, bank, wave archives and file size, straight from the archive in `extract/`; `--sdat build/rom/files/data/sound_data.sdat` lists the one the last build packed.

A changed file changes the archive's size and moves the files after it, so the ROM no longer matches retail. Such a build belongs in the mod build (issue 0023), not in a commit here.

## Build

1. `make setup` runs `sound.py unpack` (above).
2. When `extracted/AWRE/assets/sound/sound.yaml` exists, `configure.py` adds a `SOUND` step, `sound.py pack --out build/rom/files/data/sound_data.sdat --quiet`. Its inputs are `sound.yaml` and every file under `extracted/AWRE/assets/sound/`, listed again on each `make`, so a new file counts as soon as it is there. The step writes only a changed archive (ninja `restat`): a file saved with no change stops there, and the ROM is not built again. Packing takes under a second.
3. `configure.py` lists `data/sound_data.sdat` in `build/rom/built.txt` with the pictures, so `tools/rom_files.py tree` leaves that path to the step instead of linking the ROM's file (`docs/gfx.md`, "Build"). `dsd rom build` takes the file system from `build/rom/files/`.
4. Unchanged files give back the ROM's archive, so `make release`, which unpacks first, still matches retail.

Without `sound.yaml` the build links the ROM's archive as it is. `tools/wt_setup.sh` does not copy the unpacked files into a worktree (19 MB); `make setup` or `make sound` there writes them. As with the pictures, the `ROM BUILD` step stops once when `sound.yaml` comes back with an old date (`mv`, `cp -p`); run `make` again.

The archive keeps its path and its file id in the ROM's file system. The game opens it by path (`gSoundArcPath` in `Sound_Init`), and `SoundArc_ReadTables` reads the header, INFO and FAT from it at start-up into the sound heap, then each file by its FAT offset. So a larger archive only moves the ROM files placed after it. INFO and FAT share the 0xf8000-byte sound heap with the player heaps, so each added entry takes a few bytes there; how much of it is free is not measured (issue 0093).

## Adding a song

The game names a song by its sequence id, the place of its line in the `seq` list of `sound.yaml`; no table maps one to the other. The game takes song ids from:

- `gCoTable[co].theme` (`include/game/co_data.h`), read by `Player_GetCoThemeSong` for the front CO's theme. Its values are main's data, written by `data_inc.py` into `extracted/AWRE/data/main/gCoTable.inc.c`.
- Fixed ids in the code that starts a screen's song: `Sound_StartSong(66)` on the press-start screen (ov004), the CO select, the title call, the Design Room. The CO Power, Super CO Power and Tag Power themes are fixed per side in `src/game/main/copowers/`, not per CO.
- Event scripts: command 71 (`EventScript_PlaySong`) takes the id as its argument, and 72 plays the current CO's theme.
- The Sound Room list in ov005 (`data_ov005_0236b10c`, 67 cells), which only lists the songs it has a cell for.

The call chain from there to the sequence player is in issue 0085, "How the game plays music".

To add a song:

1. Put the sequence file (SSEQ) under `extracted/AWRE/assets/sound/`, for example `seq/249_BGM_MY_SONG.sseq`. The name of the file is free; `pack` reads only the path `sound.yaml` gives.
2. Add a line at the end of the `seq` list in `sound.yaml`:

   ```yaml
   - {id: 249, name: BGM_MY_SONG, file: seq/249_BGM_MY_SONG.sseq, fileId: 611, bankNo: 3, volume: 127, channelPrio: 64, playerPrio: 64, playerNo: 0}
   ```

   The ROM has sequences 0 to 248 and files 0 to 610, so the next ids are 249 and 611; the other values are made up.

   - `id`: the next number; ids count up from 0 in order, and the game plays the song by this number.
   - `name`: the SYMB name. The game does not load the names (`SoundArc_Open` is called with `symbolLoad` 0); tools show them.
   - `fileId`: the next file id, one more than the highest in `sound.yaml`. `pack` puts that file last in the FAT, after the stream; the game finds every file by its id, so the order does not matter. An entry that uses an existing file gives that file's id and path instead, as the 116 sound effects that share one sequence do.
   - `bankNo`: the instrument bank the sequence was written for, `playerNo` 0 as the other songs use, `volume` and the priorities as a song beside it has them.
3. A new bank or wave archive works the same way: a line at the end of `bank` (with its `waveArc` ids) or `wavearc`, and the next file id. `SoundHandle_StartSeq` loads the sequence, its bank and the bank's wave archives into the player's heap (`heapSize` in the `player` list), so all three must fit there.
4. Point the game at the new id, for example a CO's `theme`, and run `make`. `sound.py list --sdat build/rom/files/data/sound_data.sdat --kind seq` shows the entry.

`make setup` rewrites `sound.yaml` from the ROM and drops the added lines; the sequence file stays. Keep a mod's sound changes in the mod build (issue 0023).

`tools/m4a.py import` does steps 1 to 3 for a song from a GBA game's MusicPlayer2000 music: it converts the song into a sequence, bank and wave archive and adds their lines (`docs/m4a.md`).

Tested by hand: a copy of a CO theme added as sequence 249 with a new file id packs into an archive that `sound.py unpack` reads back with the entry and the same bytes. With the press-start screen's `Sound_StartSong(66)` changed to 249 in a scratch build, melonDS (`tools/emu.py`) shows `gCurrentSong` 249 and the music player started on sequence 249 four seconds later (`SoundSeqPlayer.sourceNo` 249, `started` 1); with 250, an id the archive does not have, the music handle stays empty.

## Export

`make music` (or `tools/sound.py export`) writes every sequence and the stream of the ROM's archive as an MP3 file (`--sdat` takes another archive, such as the one the build packed):

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
- `make assets` (part of `make setup` and `make release`) runs `export --auto`, which skips in a linked worktree and when ffmpeg is missing, and turns any other failure (no zxtune123, a render error) into one line of output. `make release` never depends on the music; `make music` reports the error. `make music` exports in any checkout.

## Keeping it out of git

Audio from the ROM stays in `extracted/` or `build/`, in any form: the archive's files, `sound.yaml` (it lists the names), and anything converted from them (MIDI, WAV, MP3, Ogg, FLAC, SoundFont, NCSF; an NCSF library holds the whole archive). `.gitignore` ignores those extensions anywhere, and `hooks/pre-commit` refuses `.sdat`, `.sseq`, `.ssar`, `.sbnk`, `.swar`, `.strm`, `.wav`, `.mid`, `.mp3`, `.ogg`, `.flac`, `.sf2` and NCSF (`.ncsf`, `.minincsf`, `.ncsflib`) files as well as anything under `extracted/` and `build/`. Never paste names or numbers from `sound.yaml` into a committed file beyond what this page gives as facts about the format.

## Python packages

`tools/sound.py` needs PyYAML (`python3-yaml` on Ubuntu), like `tools/maps.py`.
