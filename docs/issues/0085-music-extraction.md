# 0085: Investigate how the music is stored and played, to extract it like the maps

- **Status:** closed
- **Area:** tools
- **Opened:** 2026-10-14
- **Milestone:** later
- **Closed:** 2026-10-14. `tools/sound.py` unpacks the archive in `make setup` and packs it back byte for byte; `make music` renders every song, sound effect and the stream to MP3 with zxtune123 (`tools.lock`) and ffmpeg. Format and tools in `docs/sound.md`; the report is below.
- **Related:** 0050 (asset structure), 0082 (data linking; music is not part of it)

## Problem

The music and sound effects are in one file, `files/data/sound_data.sdat` (18.9 MB), which the build copies from the ROM unchanged. Nobody has looked at its contents or at how the game picks and plays a song. The goal: extract the music from the builder's ROM during `make setup`, the way `tools/maps.py` extracts the maps, into forms a person can play, edit and pack back; and find tooling that plays it or converts it to a portable format (MIDI, WAV, MP3).

This is an investigation. Its result is a short report in this file and a plan for a tool, not the tool itself.

## What we know

- `sound_data.sdat` is a sound archive of the Nintendo SDK's NitroSystem sound library (the `nns_snd_*` code). An SDAT holds sequences (SSEQ, the scores, MIDI-like), sequence archives (SSAR, mostly sound effects), instrument banks (SBNK), wave archives (SWAR, the samples), streams (STRM, recorded audio) and a symbol and info block that names and links them. Treat this as the format to confirm against our ROM, not as fact.
- Code already in C:
  - `src/game/main/sound/` (179 files): the NitroSystem layer (`nns_snd_arc_*`, `nns_snd_player_*`, `nns_snd_heap_*`, `nns_snd_strm_*`, `nns_snd_main.c`) and the game's own layer on top (`song_request_start.c`, `song_switch_task_update.c`, `song_fade_out_*.c`, `sound_start_music_seq.c`, `sound_stop_music.c`, `sound_set_music_volume.c`, `sound_play_se_at_volume.c`, ...).
  - `src/arm7/sdk/snd/` (130 files): the ARM7 side, where the sequencer and mixer run.
  - Headers: `include/game/sound.h`, `sound_arc.h`, `sound_bank.h`, `sound_handle.h`, `sound_heap.h`, `sound_stream.h`, `sound_strm.h`, `sound_wave_arc.h`.
  - The archive's path is the string `gSoundArcPath` in main `.data`.

## Questions

1. **Format.** Does `sound_data.sdat` follow the documented SDAT layout? How many sequences, banks, wave archives, streams? Which are music, which sound effects, which CO themes and battle themes? Are the names in the symbol block present (the SDK can strip them)?
2. **High-level flow.** From the game's side: how is a song chosen (CO theme, map, menu), started (`song_request_start` → `sound_start_music_seq` → `nns_snd_arc_player_start_seq`), switched and faded? Which ids does the game use, and where do they come from (tables in `.data`, CO data, map headers)? Write the call chain and the tables down, with the function names.
3. **Extraction.** What form should `make setup` write into `extracted/` (gitignored, never committed; tenet 9)? Options to compare: one file per sequence, bank and wave archive as in the archive; sequences as MIDI plus banks as SoundFont (SF2); streams as WAV. What is needed to pack it back byte for byte, so that the built ROM still matches (as `maps.py pack` does)?
4. **Tooling.** Find existing open-source tools for SDAT, and check each against our rules (Linux native, pinned in `tools.lock` with a SHA-256, no wine, licence that allows use, credited in `docs/third-party/`):
   - unpackers and repackers for SDAT, SSEQ, SBNK, SWAR, STRM;
   - converters: SSEQ to MIDI, SBNK plus SWAR to SF2, STRM to WAV;
   - players: a 2SF/NCSF-style rip played by an emulated DS sound core, or MIDI plus SF2 through a software synth such as FluidSynth;
   - encoding to MP3 or Ogg from WAV (ffmpeg, LAME).
   Name candidates, their versions, licences and what each would cost to pin. Prefer tools that run without a GUI.
5. **Fidelity.** MIDI plus SF2 loses the DS's own effects (its envelope, pitch and modulation commands); an emulated sound core reproduces them. Which path gives music that sounds like the game, and which gives music people can edit?
6. **Rules.** Audio extracted from the builder's ROM is game data: it stays in `extracted/` or `build/`, never in git, never published. A converted MP3 is the same data in another form. Say how the tooling keeps that.

## Done when

This file answers questions 1-6, with the call chain of question 2 written against our function names, a recommended extraction layout, and a recommended tool list for `tools.lock`. A follow-up issue (or issue 0050) holds the plan to build `tools/sound.py` (`unpack` in `make setup`, `pack` in the build) and a playback or conversion command.

## Report

### 1. Format

`sound_data.sdat` is a standard SDAT, version 1.0, with all four blocks. Its names were not stripped. `docs/sound.md` gives the layout as confirmed by `sound.py check`.

| Kind | Entries |
|---|---|
| Sequences | 249: 67 songs on player 0, 182 sound effects on players 1 and 2 |
| Sequence archives | 29, holding 113 more sound effects |
| Banks | 205 |
| Wave archives | 292 |
| Players | 3 |
| Stream players | 1 |
| Streams | 1, the staff roll |

The names are the developers' own, so a CO's theme is filed under the CO's Japanese name: Nell's is `BGM_CATHARINE`. The game's English titles are in the Sound Room list (ov005 `data_ov005_0236b10c`, 67 cells of title, song id, CO picture and kind), which `make music` uses for file names.

### 2. How the game plays music

A song id is the sequence's number in the archive; nothing maps it.

```
Sound_PlaySong(id)                 only stores the id while music is off (battle menu)
 -> Sound_RequestSong(id)          task; waits while a fade-out runs
  -> SongRequest_Start
   -> Sound_StartSong(id)          gCurrentSong = id
    -> Sound_StartMusicSeq(id)
     -> NNS_SndArcPlayerStartSeq(&gMusicHandle, id)
      -> SoundHandle_StartSeq      loads the bank, its wave archives, then the sequence
       -> SoundSeqPlayer_Start -> SND_StartSeq
```

- **ARM7:** the `SND_*` calls queue commands, and `SND_FlushCommand` sends them through the PXI FIFO. The ARM7 runs them in `SndArm7_ProcessCommands`; the sequencer is in `src/arm7/sdk/snd/`.
- **Switch, fade, stop:**
  - `Sound_StartSongSwitch` and `Sound_FadeOutSongOver` move the volume (`NNS_SndPlayerMoveVolume`) inside a task.
  - `Sound_StopSong` -> `Sound_StopMusic` -> `NNS_SndPlayerStopSeq`.
- **Sound effects:** `Sound_PlaySe` -> `Sound_PlayOnHandle`. The high 16 bits of the id pick a sequence archive, and 0 means a plain sequence.
- **Stream:** `Sound_StartStream` plays the staff roll stream.

Where the ids come from:

- **The CO table.** `gCoTable[co].theme` is read by `Player_GetCoThemeSong`. `Player_PlayTheme` plays the front CO's theme, or the side's power theme by `powerState`. The power themes are shared by side, not per CO: Allied or Black Hole, chosen by `Player_IsAlliedNations`, with one each for CO Power, Super CO Power and Tag Power.
- **Fixed ids** in the screens that play them: world map, title, CO select, results, shop, Design Room and ending.
- **Event script commands:** 71 `EventScript_PlaySong` and 72 `EventScript_PlayCurrentCoTheme`.
- **The Sound Room list** in ov005.

Map headers have no music field, and the battle animations start no song.

The two test cases:

- **Nell's theme** is sequence 15, `BGM_CATHARINE`, Sound Room "03 Nell's Theme". `gCoTable` gives Nell 15.
- **The Green Earth theme** is sequence 64, `BGM_EVENT_GREEN1`, "31 Green Earth Theme". It is an army theme played in story scenes, not a CO's theme: no CO has 64, and the campaign scripts request it with command 71.

`make music` writes both to `extracted/AWRE/music/bgm/`.

### 3. Extraction

`make setup` runs `sound.py unpack`. It writes one file per archive file, named by kind and name, plus `sound.yaml` with every INFO entry, into `extracted/AWRE/assets/sound/`. `sound.py pack` rebuilds the archive byte for byte.

MIDI and SoundFont were not chosen as the stored form: they lose the DS envelopes and commands, so they cannot be packed back. The ROM build still copies the original archive, and packing in the build belongs to issue 0050.

### 4. Tools

| Need | Choice |
|---|---|
| Unpack and pack | `tools/sound.py`, our own (standard library and PyYAML) |
| Render to WAV | zxtune123 r5112, pinned in `tools.lock` (LGPL-3.0, `docs/third-party/zxtune.md`). It plays NCSF files through SSEQPlayer, an emulation of the DS sequencer, and plays STRM directly. |
| Encode | ffmpeg from apt (MP3, Ogg, FLAC) |

Rejected:

- **ffmpeg alone:** it has no SDAT, SSEQ, SWAR or DS STRM reader and no MIDI synthesis.
- **VGMTrans:** it has SSEQ to MIDI and SoundFont export, but only macOS and Windows GUI releases.
- **sseq2mid, SDATStuff, sdatxtract, kode54's SSEQPlayer on its own:** source only, so they would have to be compiled.
- **Tinke, Nitro Studio, VGMusicStudio:** Windows GUIs.
- **NCSF123:** needs a .NET build.
- **FluidSynth with MIDI and SF2:** loses the DS's own sound.

### 5. Fidelity

The emulated sound engine renders the sequence as the game plays it, so it sounds like the game. MIDI plus SoundFont would be the editable form, but it loses the DS envelopes, LFO, sweep, volume curves and the random and conditional commands. Nobody has asked for editable music, so there is no MIDI export.

### 6. Rules

Every output stays in `extracted/` or `build/`: unpacked files, `sound.yaml`, NCSF files (a library holds the whole archive) and MP3s. `.gitignore` ignores the sound and audio extensions anywhere, and `hooks/pre-commit` refuses them.

This file holds the facts needed to explain the format, plus the two test cases. The full list of names and titles stays in `extracted/`, where `sound.py list` and `make music` produce it.

