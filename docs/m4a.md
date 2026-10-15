# GBA music (m4a) into the sound archive

`tools/m4a.py` reads the music of a GBA game that uses Nintendo's MusicPlayer2000 sound driver (also known as m4a or Sappy) and converts one song into the DS sound formats this game plays: a sequence (SSEQ), a bank (SBNK) and a wave archive (SWAR), added to the unpacked sound archive (`docs/sound.md`). The build then packs it into the ROM like any other entry. The GBA ROM belongs to the builder and is read where it lies; nothing read from it enters git (tenet 9).

```
tools/m4a.py list ROM                    every song: player, tracks, priority, voicegroup, bytes of commands
tools/m4a.py render ROM SONG [--out F.wav] [--loops 2] [--fade 8]
tools/m4a.py import ROM SONG --name NAME [--like SEQ] [--volume N] [--sound DIR]
```

- `list` prints the song table. Songs with no tracks (placeholders) are left out unless `--all`.
- `render` writes a WAV file with a simple software mix of the song, to tell songs apart by ear. It follows the driver's rules for timing, pitch, volume, pan, bend, vibrato and envelopes, without its reverb and interpolation, so it sounds drier than the game.
- `import` converts song SONG and writes `seq/`, `bank/` and `wavearc/` files under `--sound` (default `extracted/AWRE/assets/sound/`), with three lines in `sound.yaml`: the sequence `NAME`, the bank `BANK_NAME` and the wave archive `WAVE_NAME`, with the next ids and file ids ("Adding a song" in `docs/sound.md`). The sequence takes its volume, priorities and player from `--like` (a sequence id or name), and `--volume` overrides the volume. An import of a name that is already in `sound.yaml` replaces those entries in place and keeps their ids, so running it again gives the same files. It prints the three sizes against the player's heap, stops when they do not fit, and lists what it could not convert exactly.

`make setup` rewrites `sound.yaml` from the ROM and drops imported entries, as it does for any added song; run the import again after it.

## Finding the song table

The driver's song select function, `m4aSongNumStart`, starts with the same 30 bytes of Thumb code in every game that uses the driver as a library (`push {lr}; lsl r0, r0, 16; ldr r2, =gMPlayTable; ldr r1, =gSongTable; ...; bl MPlayStart`). `m4a.py` finds those bytes, and the word 40 bytes after their start is `gSongTable` in the function's literal pool. The table has an 8-byte entry per song (header address, music player, a second player number) and ends at the first entry whose header is not a ROM address.

In the games we looked at, the word just before the function holds the constant `m4aSoundInit` passes to `m4aSoundMode`: DA bits at bit 20, the mixing rate's index at bit 16 (13379 Hz is index 4), the master volume at bit 12 and the number of DirectSound channels at bit 8. `m4a.py` reads the mixing rate from it, and falls back to 13379 Hz with a warning when a song needs it and the constant is not there.

## Formats

All numbers are little-endian; addresses are GBA bus addresses (the ROM at 0x08000000).

- **Song header:** track count, block count, priority, reverb (bit 7 set: the song sets the reverb), voicegroup address, then one address per track.
- **Track:** a byte stream of commands, 24 ticks to the quarter note.

| Byte | Command | Arguments |
|---|---|---|
| 0x80-0xb0 | wait | none; the length is `CLOCKS[byte - 0x80]` (0 to 96 ticks) |
| 0xb1 | FINE | none: the track ends |
| 0xb2 | GOTO | address |
| 0xb3 | PATT | address: call, up to 3 deep |
| 0xb4 | PEND | none: return from PATT |
| 0xb5 | REPT | count, address |
| 0xb9 | MEMACC | 3 bytes |
| 0xba-0xc8 | PRIO, TEMPO (BPM / 2), KEYSH, VOICE, VOL, PAN, BEND, BENDR, LFOS, LFODL, MOD, MODT, TUNE | one byte; PAN, BEND and TUNE are 0x40 for 0 |
| 0xcd | XCMD | a sub-command and 1 or 4 bytes |
| 0xce | EOT | optional key: ends the TIE on that key |
| 0xcf | TIE | optional key and velocity: a note held until its EOT |
| 0xd0-0xff | note | optional key, velocity and extra gate ticks; the length is `CLOCKS[byte - 0xcf]` |

A byte below 0x80 where a command is expected repeats the last command from 0xbd up (running status), with that byte as its first argument. A note's key and velocity may be left out: the track's last key and velocity are used. Notes do not take time; waits do.

- **Voice** (12 bytes): type, key, length, pan or sweep, a 4-byte field, attack, decay, sustain, release. The type's low 3 bits pick DirectSound (0) or a GB channel (1 and 2 square, 3 wave, 4 noise); 0x08 is a DirectSound voice that plays at the mixing rate whatever the key; 0x40 is a key split (the 4-byte field points at a voicegroup and the last 4 bytes at a 128-byte table of key to voice); 0x80 is a drum kit (the field points at a voicegroup of 128 voices, one per key). The 4-byte field is a sample's address (DirectSound), the duty 0-3 (squares), the address of 16 bytes of 4-bit steps (wave) or the noise mode.
- **Sample:** type (0: 8-bit PCM), flags (0x4000: loops), rate x 1024 at key 60, loop start, sample count, then the signed samples.

## How a song converts

### Sequence

Each track is run as the driver runs it, twice through its loop. Every command it reaches becomes DS commands at the place it has in the ROM, so the SSEQ keeps the song's shape: PATT and PEND become call (0x95) and return (0xfd), GOTO a jump (0x94), FINE the end of the track (0xff). The SSEQ starts by allocating the tracks and opening tracks 1 and up, each at a short stub that turns off note waits (0xc7 0, so notes do not take time, as in m4a), sets the LFO speed to m4a's default and the modulation range, and jumps to the track's commands.

| m4a | DS | How |
|---|---|---|
| wait, note length | rest 0x80, note length | doubled: the SSEQ counts 48 ticks to the quarter note |
| note | note 0x00-0x7f | key as is; velocity through the volume curve below |
| TIE ... EOT | a note | it lasts from the TIE to its EOT; see below |
| TEMPO | 0xe1 | BPM, as a 16-bit value |
| VOICE | program 0x81 | the voice's number; the bank has an instrument at that number |
| VOL | volume 0xc1 | through the volume curve |
| PAN | pan 0xc0 | as is (64 is the middle) |
| BEND, BENDR, TUNE | pitch bend 0xc4, bend range 0xc5 | m4a bends by (BEND x BENDR + TUNE) / 64 semitones, the DS by bend x range / 128: the DS range is BENDR (wider when TUNE would take the bend past 127) and the bend is 2 x (BEND x BENDR + TUNE) / range |
| KEYSH | transpose 0xc3 | as is |
| MOD | modulation depth 0xca | as is, with range 8 for vibrato: m4a's MOD gives MOD / 16 semitones, the DS's depth x range / 128 |
| MODT | modulation type 0xcc | 0 vibrato, 1 tremolo, 2 auto-pan as is; range 8 for vibrato, 1 for the others |
| LFOS | modulation speed 0xcb | m4a steps LFOS / 256 of a cycle per tick, the DS speed / 512 per sound frame (2728 x 64 / 33513982 s), at the song's first tempo |
| LFODL | modulation delay 0xe0 | ticks to sound frames at the song's first tempo |
| PRIO | none | the DS's channel priorities stay as the player sets them |

A command reached more than once must give the same DS commands each time, since it has one place in the SSEQ. When it does not (a bend that depends on a BENDR set elsewhere, a note in a pattern that two voices play), the first is used and `import` warns.

A TIE is one place in the ROM, but its EOT can come at different times: a TIE inside a pattern can be ended by each caller. When one TIE lasts different times on different passes, it plays in the DS's tie mode instead: 0xc8 1 before the note and 0xc8 0 at each EOT that ends it. Tie mode releases the track's other notes when it starts and plays the track's next notes on the tied note's channel, so it is used only when the track plays no other note while the tie lasts; otherwise the first length is used with a warning. A TIE with no EOT lasts to the end of the second pass.

### Volume curve

m4a's volume, velocity and sustain are linear. The DS turns each value 0-127 into decibels with a square law (40 x log10(v / 127), the table `gSndVolumeDecibels` in our ARM7 code, `src/arm7/sdk/snd/`). Each m4a value is converted to the DS value whose level is nearest: m4a 64 (half) becomes DS 90 (-6.0 dB).

### Bank and wave archive

The bank has one instrument per voice number the song plays; voices it does not play are left empty.

| m4a voice | SBNK instrument |
|---|---|
| DirectSound | PCM (type 1), base note 60, the sample in the wave archive at its rate |
| DirectSound, fixed pitch (0x08) | a drum set over the keys the song plays, each with base note = its key, the sample at the mixing rate |
| Square 1 and 2 | PSG (type 2), duty 12.5, 25, 50, 75 % as DS duty 0, 1, 3, 5; square 1's sweep is dropped |
| Wave | PCM: the 32 steps as an 8-bit looped wave, at 4186 Hz for key 60 (the GB wave channel plays an octave below its key) |
| Noise | noise (type 3), base note 60; pitch and 7-bit mode are loose |
| Key split | key split (type 17) with up to 8 ranges over the keys the song plays; more ranges make a drum set |
| Drum kit | drum set (type 16) over the keys the song plays; each drum plays at its own key (base note = key - drum key + 60) and its own pan |

A GB channel at its loudest is about a quarter of a full DirectSound channel on the GBA; the DS PSG channels are as loud as PCM, so notes on GB voices get a quarter of their velocity's level (`CGB_LEVEL`). This value is rough.

A sample becomes an 8-bit PCM wave. The DS loops on 4-byte words: zero samples go before a loop start that is not on a word, and a loop whose length is not a whole number of words is repeated (up to 4 times) until it is. The wave's timer is 16756991 / rate, rounded.

### Envelopes

m4a steps envelopes once a frame (1/60 s), the DS once a sound frame (5.21 ms).

- **DirectSound attack** adds the attack value to a 0-255 level each frame, a straight rise. The DS attack multiplies the attenuation each sound frame, a curve. `import` picks the DS attack whose curve is nearest the rise (least squares over its length); 255 (instant) is 127.
- **DirectSound decay and release** multiply the level by n/256 each frame: a fall of -20 log10(n/256) x 60 dB a second. The DS falls by a fixed number of 1/1280 dB each sound frame; `import` picks the DS value with the nearest rate. 0 (at once) is 127.
- **DirectSound sustain** n/255 goes through the volume curve.
- **GB channels** move one of 15 steps every n frames (n = 0: at once), and sustain is n/15. Attack is fit like DirectSound's over 15 x n frames; decay and release, straight lines in level, get the DS rate that reaches the same level (the sustain, or silence at -72.3 dB) in the same time.

## Limits

- Reverb, the GBA's mixing (8-bit output, master volume) and the DS's interpolation differ; the DS version sounds cleaner.
- Commands with no DS counterpart are dropped with a warning: PRIO, MEMACC, XCMD, PORT, REPT with a count (REPT 0 is a jump).
- KEYSH on a key split or drum kit: the DS picks the instrument by the transposed key, m4a by the key; `import` warns.
- LFO speed and delay use the first tempo of the song.
- A TIE that lasts different times on different passes and overlaps other notes keeps its first length.
- The GB channels' levels against DirectSound, the noise channel's pitch and square 1's sweep are approximations.
- Compressed samples (type 1, in some later games) are refused.

Checked against Advance Wars 2 (US): the table has 505 entries, 209 of them songs, and every song converts. The made-up ROM in `tools/tests/test_m4a.py` checks the bytes of each kind of output (`make test`); with `M4A_TEST_ROM` naming a GBA ROM, the test also converts every song of it.

## Sources

The m4a driver's behaviour comes from pret's decompilation of Pokémon Emerald (`docs/third-party/pokeemerald.md`). The DS driver's units and tables are this game's own, in our ARM7 code (`src/arm7/sdk/snd/`: the sound frame, `gSndVolumeDecibels`, `gSndAttackTable`, `SndExChannel_CalcDecayRate`, `SndExChannel_UpdateEnvelope`, the PSG timer), read alongside the Nitro driver in pokediamond (`docs/third-party/pokediamond.md`) and SSEQPlayer (`docs/third-party/sseqplayer-caitsith2.md`). Sappy and GBA Mus Riper find the table through `m4aSongNumStart`, as `m4a.py` does (`docs/third-party/gba-mus-ripper.md`).
