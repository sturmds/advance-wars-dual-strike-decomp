#!/usr/bin/env python3
"""Check tools/savedit.py on a save file made here, byte by byte.

    python3 tools/tests/test_savedit.py

The save has a current record 0 in slot 20, an older copy of it in slot 3
that the current slot table no longer lists, and a slot with a bad
checksum. The slot header and the field offsets below are written as
numbers, apart from the headers: the game fixes them, so a header change
that moves one is a mistake this test catches.

1. `set` changes Sonja's experience and her available bit, and one War
   Room record, and nothing else but the two checksum bytes of slot 20.
2. Each changed slot passes the check of Save_CheckBuffer.
3. A value too wide for its bit field, an index past the array and a
   record of the wrong length are refused.

Needs the compiler from `make setup` (for build/dsindex.json); skipped
without it.
"""

import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import configure  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent.parent
TOOL = ROOT / "tools/savedit.py"
SLOT = 0x3000
SLOTS = 21
RECORD = 0x1684
DATA = 0x66
SLOT_IDS = 0x2FEA
SONJA = 0x170        # playRecords.coRecords[CO_SONJA]
SPANN_ISLAND = 0x574  # playRecords.warRoomRecords[0][0][0]
failures = []


def check(what: str, ok: bool, detail=""):
    print(f"{what}: {'ok' if ok else 'FAIL'}{f' ({detail})' if detail and not ok else ''}")
    if not ok:
        failures.append(what)


def slot(record_id: int, counter: int, data: bytes, slot_ids: bytes) -> bytearray:
    b = bytearray(b"\xff" * SLOT)
    b[0:4] = (0x6A777364).to_bytes(4, "little")
    b[4], b[5] = 0xAA, 29
    b[8:12] = counter.to_bytes(4, "little")
    b[0xC], b[0xD] = 0, record_id
    b[0xE:0x10] = (0).to_bytes(2, "little")
    b[0x64:0x66] = len(data).to_bytes(2, "little")
    b[DATA:DATA + len(data)] = data
    b[SLOT_IDS:SLOT_IDS + SLOTS] = slot_ids
    b[0x2FFF] = 0x55
    b[6], b[7] = 0, 0xFF
    total = sum(b) & 0xFF
    b[6], b[7] = total, ~total & 0xFF
    return b


def passes_check(b: bytes) -> bool:
    return (b[0:4] == (0x6A777364).to_bytes(4, "little") and b[5] == 29
            and sum(b) & 0xFF == b[6] and b[7] == ~b[6] & 0xFF)


def run(*args) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, str(TOOL), *map(str, args)],
                          capture_output=True, text=True, cwd=ROOT)


def main() -> int:
    if not (ROOT / configure.CC).exists():
        print("savedit: skipped (no compiler; run make setup)")
        return 0

    record = bytearray(RECORD)
    record[SONJA:SONJA + 4] = (228 << 19).to_bytes(4, "little")  # a Force Rank field
    old = bytearray(RECORD)
    old[SONJA] = 0x42
    current_ids = bytes(0 if i == 20 else 0xFF for i in range(SLOTS))
    old_ids = bytes(0 if i == 3 else 0xFF for i in range(SLOTS))
    save = bytearray(b"\xff" * (SLOT * SLOTS)) + b"\xff" * 0x1000
    save[20 * SLOT:21 * SLOT] = slot(0, 5, record, current_ids)
    save[3 * SLOT:4 * SLOT] = slot(0, 4, old, old_ids)
    bad = slot(0, 9, old, old_ids)
    bad[6] ^= 1
    save[7 * SLOT:8 * SLOT] = bad

    with tempfile.TemporaryDirectory() as tmp:
        src, out = Path(tmp) / "in.sav", Path(tmp) / "out.sav"
        src.write_bytes(save)
        spann = "playRecords.warRoomRecords[0][0x8b - WAR_ROOM_FIRST_MAP][0]"
        r = run("set", src, "playRecords.coRecords[CO_SONJA].experience=7000",
                "playRecords.coRecords[CO_SONJA].available=1", f"{spann}.frontCo=CO_KANBEI",
                f"{spann}.days=7", f"{spann}.score=300", "-o", out)
        check("set runs", r.returncode == 0, r.stderr.strip())
        if r.returncode:
            return 1
        new = out.read_bytes()
        base = 20 * SLOT + DATA
        sonja = int.from_bytes(new[base + SONJA:base + SONJA + 4], "little")
        check("Sonja's experience and available bit", sonja == 7000 | 228 << 19 | 1 << 31, hex(sonja))
        spann_word = int.from_bytes(new[base + SPANN_ISLAND:base + SPANN_ISLAND + 4], "little")
        check("Spann Island record", spann_word == 7 | 7 << 10 | 300 << 20, hex(spann_word))
        changed = {i for i in range(len(save)) if save[i] != new[i]}
        allowed = set(range(base + SONJA, base + SONJA + 4)) | \
            set(range(base + SPANN_ISLAND, base + SPANN_ISLAND + 4)) | {20 * SLOT + 6, 20 * SLOT + 7}
        check("only those fields and the checksum change", changed <= allowed,
              sorted(hex(i) for i in changed - allowed))
        check("slot 20 passes Save_CheckBuffer", passes_check(new[20 * SLOT:21 * SLOT]))

        r = run("show", out, "playRecords.coRecords[CO_SONJA]")
        check("show reads the new values", "7000" in r.stdout and r.returncode == 0, r.stdout)

        r = run("set", src, "playRecords.coRecords[CO_SONJA].experience=0x80000", "-o", out)
        check("a value wider than the bit field is refused", r.returncode != 0)
        r = run("set", src, "playRecords.coRecords[28].experience=1", "-o", out)
        check("an index past the array is refused", r.returncode != 0)

        short = bytearray(save)
        short[20 * SLOT:21 * SLOT] = slot(0, 5, record[:-4], current_ids)
        src.write_bytes(short)
        r = run("show", src)
        check("a record of the wrong length is refused", r.returncode != 0 and "disagree" in r.stderr,
              r.stderr.strip())

    if failures:
        print(f"savedit: {len(failures)} failed")
        return 1
    print("savedit: all passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
