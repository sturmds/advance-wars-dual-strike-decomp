#!/usr/bin/env python3
"""Check tools/m4a.py on a made-up GBA ROM; no game ROM needed.

    python3 tools/tests/test_m4a.py

- The song table is found through the driver's song select code, and the
  mixing rate through the constant before it.
- A two-track song (tempo, voice, volume, pan, a note repeated by running
  status with its velocity left out, waits, a TIE ended by an EOT after a
  pattern call, a GOTO loop, a bend, a GB square note, FINE) converts to the
  SSEQ bytes worked out by hand below: waits and lengths doubled, the track
  stubs, call, return and jump offsets.
- The bank: a DirectSound voice as a PCM instrument, a 50 % square as a PSG
  instrument with DS duty 3. The wave archive: a loop start off a word gets
  zero samples before it, and a 5-sample loop is repeated 4 times.
- A TIE in a pattern that two callers end at different times plays in the
  DS's tie mode (0xc8 1 ... 0xc8 0).
- import adds a sequence, bank and wave archive to a made-up sound.yaml with
  the next ids and file ids, and a second import of the same name keeps them;
  an entry of the name that shares another entry's file gets a file id of
  its own.
- The volume curve: linear 1 is 127, 0 is 0, and it never goes down.
- With M4A_TEST_ROM naming a GBA ROM: list and convert every song of it
  (skipped with a message when unset).
"""

import argparse
import contextlib
import io
import os
import struct
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import m4a  # noqa: E402

failures = []
BASE = m4a.GBA_ROM_BASE


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if got == want else 'FAIL'}")
    if got != want:
        print(f"  got:  {got.hex(' ') if isinstance(got, bytes) else got!r}\n"
              f"  want: {want.hex(' ') if isinstance(want, bytes) else want!r}")
        failures.append(what)


def ptr(off: int) -> bytes:
    return struct.pack("<I", BASE + off)


def made_up_rom() -> bytes:
    rom = bytearray(0x1000)

    def put(off: int, data: bytes) -> None:
        rom[off:off + len(data)] = data

    put(0xa0, b"M4ATESTROM")
    rom[0xb2] = 0x96
    put(0x1fc, struct.pack("<I", 0x0094db00))          # m4aSoundMode: 13379 Hz
    put(0x200, m4a.SELECT_SONG)
    put(0x200 + m4a.SONG_TABLE_POINTER, ptr(0x300))
    put(0x300, ptr(0x400) + bytes(4) + ptr(0x480) + bytes(4))   # two songs, then 0
    # Song 0: two tracks on the voicegroup at 0x500.
    put(0x400, bytes((2, 0, 0, 0)) + ptr(0x500) + ptr(0x600) + ptr(0x700))
    # Song 1: one track, a TIE in a pattern ended at two different times.
    put(0x480, bytes((1, 0, 0, 0)) + ptr(0x500) + ptr(0x900))
    # Voice 0: DirectSound, instant attack, full sustain; voice 1: square 2 at 50 %.
    put(0x500, bytes((0, 60, 0, 0)) + ptr(0x800) + bytes((255, 0, 255, 0)))
    put(0x50c, bytes((2, 60, 0, 0)) + struct.pack("<I", 2) + bytes((0, 0, 15, 0)))
    # Sample: looped from 2, 7 samples (and the driver's one extra).
    put(0x800, struct.pack("<HHIII", 0, 0x4000, 13379 * 1024, 2, 7) + bytes(range(1, 9)))
    put(0x600, bytes((0xbb, 0x3c,                      # TEMPO 120
                      0xbd, 0x00,                      # VOICE 0
                      0xbe, 0x64,                      # VOL 100
                      0xbf, 0x50,                      # PAN +16
                      0xd7, 0x3c, 0x7f,                # N08 key 60 velocity 127
                      0x81,                            # W01
                      0x3e,                            # N08 again (running status), key 62
                      0x98,                            # W24
                      0xcf, 0x40, 0x64,                # TIE key 64 velocity 100
                      0xb3)) + ptr(0x680)              # PATT
        + bytes((0xce, 0x40, 0xb2)) + ptr(0x600))      # EOT 64, GOTO the start
    put(0x680, bytes((0x8c, 0xb4)))                    # W12, PEND
    # VOICE 1, VOL 127, BEND +16, N04 key 60 velocity 80, W04, FINE.
    put(0x700, bytes((0xbd, 0x01, 0xbe, 0x7f, 0xc0, 0x50, 0xd3, 0x3c, 0x50, 0x84, 0xb1)))
    # VOICE 0, PATT, W04, EOT 60, PATT, W08, EOT 60, FINE; the pattern: TIE 60, W04, PEND.
    put(0x900, bytes((0xbd, 0x00, 0xb3)) + ptr(0x980) + bytes((0x84, 0xce, 0x3c, 0xb3)) + ptr(0x980)
        + bytes((0x88, 0xce, 0x3c, 0xb1)))
    put(0x980, bytes((0xcf, 0x3c, 0x7f, 0x84, 0xb4)))
    return bytes(rom)


def quiet(fn, *args):
    with contextlib.redirect_stdout(io.StringIO()):
        return fn(*args)


def main() -> int:
    with tempfile.TemporaryDirectory() as tmp:
        path = Path(tmp) / "test.gba"
        path.write_bytes(made_up_rom())
        rom = m4a.Rom(path)
        expect("song table", m4a.song_table(rom), (BASE + 0x300, 2))
        expect("mixing rate", m4a.mix_rate(rom), (13379, True))

        song = m4a.get_song(rom, 0)
        report = m4a.Report()
        sseq, sbnk, swar = m4a.convert(rom, song, report)
        v100 = m4a.ds_volume(100 / 127)
        square_vel = m4a.ds_volume(80 / 127 * m4a.CGB_LEVEL)
        lfo = m4a.ds_lfo_speed(22, 120)
        stub = bytes((0xc7, 0x00, 0xcb, lfo, 0xcd, 0x08, 0x94))
        # Offsets: header 3 + open 5 = 8; stubs of 10 bytes at 8 and 18; code from 28.
        t0 = 28
        code0 = (bytes((0xe1, 0x78, 0x00, 0x81, 0x00, 0xc1, v100, 0xc0, 0x50,
                        0x3c, 0x7f, 0x10, 0x80, 0x02, 0x3e, 0x7f, 0x10, 0x80, 0x30,
                        0x40, v100, 0x18)))                       # the TIE: 12 ticks to its EOT
        patt_at = t0 + len(code0) + 4 + 4                         # after the call and the jump
        code0 += bytes((0x95,)) + patt_at.to_bytes(3, "little") + bytes((0x94,)) + t0.to_bytes(3, "little")
        code_patt = bytes((0x80, 0x18, 0xfd))
        t1 = patt_at + len(code_patt)
        code1 = bytes((0x81, 0x01, 0xc1, 0x7f, 0xc4, 0x20, 0x3c, square_vel, 0x08, 0x80, 0x08, 0xff))
        events = (bytes((0xfe, 0x03, 0x00, 0x93, 0x01)) + (18).to_bytes(3, "little")
                  + stub + t0.to_bytes(3, "little") + stub + t1.to_bytes(3, "little")
                  + code0 + code_patt + code1)
        events += bytes(-len(events) % 4)
        size = 0x1c + len(events)
        want = b"SSEQ" + struct.pack("<HHIHH", 0xfeff, 0x100, size, 0x10, 1) + b"DATA" + struct.pack(
            "<II", size - 0x10, 0x1c) + events
        expect("sequence bytes", sseq, want)
        expect("no warnings", report.warnings, [])

        inst0 = struct.pack("<HHBBBBBB", 0, 0, 60, 127, 127, 127, 127, 64)
        inst1 = struct.pack("<HHBBBBBB", 3, 0, 60, 127, 127, 127, 127, 64)
        body = bytes(32) + struct.pack("<I", 2) + struct.pack("<BHBBHB", 1, 0x44, 0, 2, 0x4e, 0) + inst0 + inst1
        body += bytes(-(0x18 + len(body)) % 4)
        expect("bank bytes", sbnk, b"SBNK" + struct.pack("<HHIHH", 0xfeff, 0x100, 0x18 + len(body), 0x10, 1)
               + b"DATA" + struct.pack("<I", 8 + len(body)) + body)

        pcm = bytes((0, 0, 1, 2)) + bytes(range(3, 8)) * 4
        wav = struct.pack("<BBHHHI", 0, 1, 13379, 1252, 1, 5) + pcm
        body = bytes(32) + struct.pack("<II", 1, 0x40) + wav
        expect("wave archive bytes", swar, b"SWAR" + struct.pack("<HHIHH", 0xfeff, 0x100, 0x18 + len(body), 0x10, 1)
               + b"DATA" + struct.pack("<I", 8 + len(body)) + body)

        sseq1, _, _ = m4a.convert(rom, m4a.get_song(rom, 1), m4a.Report())
        ev = sseq1[0x1c:]
        expect("tie mode: the TIE", b"\xc8\x01\x3c\x7f\x00" in ev, True)
        expect("tie mode: both EOTs end it", ev.count(b"\xc8\x00"), 2)

        # import into a made-up unpacked archive.
        snd = Path(tmp) / "sound"
        (snd / "seq").mkdir(parents=True)
        (snd / "seq" / "000_BGM_A.sseq").write_bytes(b"x")
        (snd / "sound.yaml").write_text(
            "version: 0x100\nseq:\n"
            "- {id: 0, name: BGM_A, file: seq/000_BGM_A.sseq, fileId: 0, bankNo: 0, volume: 80, channelPrio: 64,"
            " playerPrio: 64, playerNo: 0}\nseqarc: []\nbank: []\nwavearc: []\n"
            "player:\n- {id: 0, name: PLAYER_BGM, maxPlayers: 1, trackChannelMask: 0, heapSize: 760000}\n"
            "group: []\nplayer2: []\nstrm: []\nunusedFiles: []\n")
        args = argparse.Namespace(rom=str(path), song=0, name="BGM_TEST", like="BGM_A", volume=None, sound=str(snd))
        quiet(m4a.cmd_import, args)
        first = (snd / "sound.yaml").read_text()
        expect("import: sequence entry", "- {id: 1, name: BGM_TEST, file: seq/001_BGM_TEST.sseq, fileId: 1, bankNo: 0,"
               " volume: 80, channelPrio: 64, playerPrio: 64, playerNo: 0}" in first, True)
        expect("import: bank entry", "- {id: 0, name: BANK_BGM_TEST, file: bank/000_BANK_BGM_TEST.sbnk, fileId: 2,"
               " waveArc: [0]}" in first, True)
        expect("import: wave archive entry", "- {id: 0, name: WAVE_BGM_TEST, file: wavearc/000_WAVE_BGM_TEST.swar,"
               " fileId: 3}" in first, True)
        expect("import: files", (snd / "seq/001_BGM_TEST.sseq").read_bytes(), sseq)
        quiet(m4a.cmd_import, args)
        expect("import again: same sound.yaml", (snd / "sound.yaml").read_text(), first)
        # An entry of the name that shares another's file (a copy of BGM_A) gets a file of its own.
        (snd / "sound.yaml").write_text(first.replace(
            "file: seq/001_BGM_TEST.sseq, fileId: 1,", "file: seq/000_BGM_A.sseq, fileId: 0,"))
        quiet(m4a.cmd_import, args)
        again = (snd / "sound.yaml").read_text()
        expect("import over a copy: own file id", "- {id: 1, name: BGM_TEST, file: seq/001_BGM_TEST.sseq, fileId: 4,"
               in again and "- {id: 0, name: BGM_A, file: seq/000_BGM_A.sseq, fileId: 0," in again, True)

    expect("volume curve ends", (m4a.ds_volume(1.0), m4a.ds_volume(0)), (127, 0))
    levels = [m4a.ds_volume(i / 100) for i in range(101)]
    expect("volume curve never goes down", levels == sorted(levels), True)

    other = os.environ.get("M4A_TEST_ROM")
    if not other:
        print("game ROM: skipped (set M4A_TEST_ROM to a GBA ROM with MusicPlayer2000 music to convert every song)")
    else:
        rom = m4a.Rom(Path(other))
        count = 0
        for s in m4a.songs(rom):
            if s.tracks:
                m4a.convert(rom, s, m4a.Report())
                count += 1
        print(f"game ROM: converted {count} songs: ok")
    if failures:
        print(f"{len(failures)} failed: {', '.join(failures)}")
        return 1
    print("all passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
