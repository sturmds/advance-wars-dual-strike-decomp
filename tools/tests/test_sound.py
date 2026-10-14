#!/usr/bin/env python3
"""Check tools/sound.py.

    python3 tools/tests/test_sound.py

- A made-up archive (two sequences on one file, a sequence archive with
  sub-names, a bank with an unused wave archive slot between two used ones,
  players, a stream) comes back from unpack and pack byte for byte, with the
  shared file written once.
- pack stops when two entries give one file id different files, and on a
  field the entry does not have.
- With extract/ present: unpack and pack the ROM's sound_data.sdat in a
  temporary directory and compare the SHA-1 (`sound.py check`). Skipped with a
  message when there is no extract/.
"""

import argparse
import contextlib
import io
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import sound  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent.parent
failures = []


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if got == want else 'FAIL'}")
    if got != want:
        print(f"  got:  {got!r}\n  want: {want!r}")
        failures.append(what)


def quiet(fn, *args):
    with contextlib.redirect_stdout(io.StringIO()):
        return fn(*args)


names = {"seq": ["SEQ_A", "SEQ_B"], "seqarc": ["ARC_A", "ARC_B"], "bank": ["BANK_A"],
         "wavearc": ["WAVE_A", "WAVE_B"], "player": ["PLAYER_A"], "group": [], "player2": ["PLAYER2_A"],
         "strm": ["STRM_A"]}
subnames = [["SE_ONE", "SE_TWO"], []]
seq = {"fileId": 0, "bankNo": 0, "volume": 100, "channelPrio": 64, "playerPrio": 32, "playerNo": 0}
entries = {
    "seq": [seq, dict(seq, volume=90)],
    "seqarc": [{"fileId": 1}, {"fileId": 2}],
    "bank": [{"fileId": 3, "waveArc": [0, None, 1]}],
    "wavearc": [{"fileId": 4}, {"fileId": 5, "flags": 1}],
    "player": [{"maxPlayers": 1, "trackChannelMask": 0xffff, "heapSize": 1000}],
    "group": [],
    "player2": [{"channels": [6, 7]}],
    "strm": [{"fileId": 6, "volume": 105, "priority": 64, "slotNo": 0}],
}
# Lengths that are not multiples of 32, so the padding shows.
files = [bytes([i]) * (13 + 7 * i) for i in range(7)]
made_up = sound.build(names, subnames, entries, files)

with tempfile.TemporaryDirectory() as tmp:
    tmp = Path(tmp)
    (tmp / "a.sdat").write_bytes(made_up)
    quiet(sound.cmd_unpack, argparse.Namespace(sdat=str(tmp / "a.sdat"), out=str(tmp / "out")))
    expect("made-up: pack gives the same bytes", sound.load(tmp / "out") == made_up, True)
    expect("made-up: the shared file is written once", sorted(p.name for p in (tmp / "out/seq").iterdir()),
           ["000_SEQ_A.sseq"])
    fat = sound.u32(made_up, 0x20)
    offsets = [sound.u32(made_up, fat + 12 + 16 * i) for i in range(len(files))]
    expect("made-up: files on 32-byte boundaries", [o % 32 for o in offsets] + [len(made_up) % 32], [0] * 8)
    arc = sound.Archive(made_up)
    expect("made-up: names read back", (arc.names, arc.subnames), (names, subnames))
    expect("made-up: unused bank slot kept", arc.entries["bank"][0]["waveArc"], [0, None, 1])
    expect("made-up: wave archive flags kept", arc.entries["wavearc"][1], {"fileId": 5, "flags": 1})
    manifest = tmp / "out" / sound.MANIFEST
    manifest.write_text(manifest.read_text().replace("file: seq/000_SEQ_A.sseq, fileId: 0, bankNo: 0, volume: 90",
                                                     "file: seq/001_SEQ_B.sseq, fileId: 0, bankNo: 0, volume: 90"))
    try:
        sound.load(tmp / "out")
        expect("made-up: two files for one file id stop pack", "returned", "stopped")
    except SystemExit as e:
        expect("made-up: two files for one file id stop pack", str(e.code).startswith("error:"), True)

    manifest.write_text(manifest.read_text().replace("seq/001_SEQ_B.sseq", "seq/000_SEQ_A.sseq")
                        .replace("slotNo: 0", "slot: 0"))
    try:
        sound.load(tmp / "out")
        expect("made-up: an unknown field stops pack", "returned", "stopped")
    except SystemExit as e:
        expect("made-up: an unknown field stops pack", str(e.code).startswith("error:"), True)

# Export: a sequence archive whose second sequence starts 4 bytes into the
# shared events comes out as an SSEQ holding the events from there on; one
# that calls by offset stops the export.
events = bytes([0xc1, 0x7f, 0x80, 0x10, 0x3c, 0x64, 0x81, 0x30, 0xff])   # volume, rest, note, end
ssar = (b"SSAR" + bytes(12) + b"DATA" + bytes(4) + (0x44).to_bytes(4, "little") + (3).to_bytes(4, "little")
        + bytes.fromhex("00000000" "0100" "7f404001" "0000")
        + bytes.fromhex("04000000" "0200" "64404002" "0000")
        + bytes.fromhex("ffffffff" "0000" "00000000" "0000") + events)
got = sound.seqarc_sequences(ssar, "made-up")
expect("export: sequence 1 starts at its offset", got[1][0][0x1c:], events[4:])
expect("export: sequence 1 SSEQ sizes", (got[1][0][8:12], got[1][0][0x14:0x18]),
       ((0x1c + 5).to_bytes(4, "little"), (0x0c + 5).to_bytes(4, "little")))
expect("export: sequence 1 entry", got[1][1],
       {"bankNo": 2, "volume": 0x64, "channelPrio": 0x40, "playerPrio": 0x40, "playerNo": 2})
expect("export: an unused sequence is None", got[2], None)
try:
    sound.seqarc_sequences(ssar.replace(events, bytes([0x95, 0, 0, 0, 0xff])), "made-up")
    expect("export: a call by offset stops", "returned", "stopped")
except SystemExit as e:
    expect("export: a call by offset stops", str(e.code).startswith("error:"), True)
lib = sound.psf(b"", b"SDAT-bytes", {})
mini = sound.psf((15).to_bytes(4, "little"), b"", {"_lib": "sound.ncsflib", "length": "2:30"})
expect("export: NCSF library header", (lib[:4], int.from_bytes(lib[4:8], "little")), (b"PSF\x25", 0))
expect("export: NCSF song holds its number and tags",
       (mini[16:20], mini[20:]), ((15).to_bytes(4, "little"), b"[TAG]_lib=sound.ncsflib\nlength=2:30\n"))

if (ROOT / sound.SDAT).exists():
    expect("ROM: check passes", quiet(sound.cmd_check, argparse.Namespace(sdat=str(sound.SDAT))), 0)
else:
    print(f"ROM: skipped ({sound.SDAT} not found; run make extract)")

if failures:
    sys.exit(f"test_sound: {len(failures)} failed")
print("test_sound: OK")
