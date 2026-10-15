#!/usr/bin/env python3
"""Read and edit an AWRE save file (.sav), field by field (docs/save.md).

    savedit.py slots SAV                       the valid slots: record id, part, save counter
    savedit.py show SAV [PATH]                 fields of a record, or of the part under PATH
    savedit.py set SAV PATH=VALUE ... -o OUT   write fields and fix the slot checksums

A .sav file holds a raw copy of the backup chip: SAVE_SLOT_COUNT slots of
sizeof(SaveBuffer) bytes, slot i at i * sizeof(SaveBuffer). The newest
record 0 lists in SaveBuffer.slotIds which record each slot holds, as
Save_LoadRecord reads it.

Every layout comes from build/dsindex.json (tools/dsindex.py), which the
compiler builds from our headers: the slot header from SaveBuffer, the
record from the struct in RECORD_TYPES (or --type). A field added, renamed
or moved in a header needs no change here. A record whose length differs
from the struct's size is refused.

PATH starts inside the record's struct. Indexes take numbers, enum and
#define names, and sums of those:
    playRecords.coRecords[CO_SONJA].experience
    playRecords.warRoomRecords[0][0x8b - WAR_ROOM_FIRST_MAP][0].score
A path that stops at an array prefix (warRoomRecords[0][0]) shows the rest.

`set` rewrites the record's data inside the slots that hold it and fixes
each slot's checksum. Counters and slot ids stay as they are, so the
record cannot change length.
"""

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ds  # noqa: E402

# Struct of each record id, as the game packs it (include/game/save_progress.h).
RECORD_TYPES = {0: "SaveProgressBlocks"}


class Save:
    """The slots of a .sav file and the layout of their header."""

    def __init__(self, data: bytes):
        self.data = bytearray(data)
        self.consts = ds.constants()
        self.slot_size = struct_record("SaveBuffer")["size"]
        self.slot_count = self.consts["SAVE_SLOT_COUNT"]
        if len(self.data) < self.slot_size * self.slot_count:
            sys.exit(f"save is {len(self.data):#x} bytes; "
                     f"{self.slot_count} slots need {self.slot_size * self.slot_count:#x}")
        self.header = {f["name"]: f for f in struct_record("SaveBuffer")["fields"]}

    def field(self, slot: int, name: str) -> int:
        f = self.header[name]
        start = slot * self.slot_size + f["offset"]
        return int.from_bytes(self.data[start:start + f["size"]], "little")

    def bytes_of(self, slot: int, name: str) -> bytearray:
        f = self.header[name]
        start = slot * self.slot_size + f["offset"]
        return self.data[start:start + f["size"]]

    def valid(self, slot: int) -> bool:
        """Save_CheckBuffer: magic, version, the byte sum and its inverse.
        The sum covers the stored checksum bytes, which add up to 0xff, as
        the 0 and 0xff they hold while Save_WriteRecord sums the slot."""
        chunk = self.data[slot * self.slot_size:(slot + 1) * self.slot_size]
        checksum = self.field(slot, "checksum")
        return (self.field(slot, "magic") == self.consts["SAVE_MAGIC"]
                and self.field(slot, "version") == self.consts["SAVE_VERSION"]
                and sum(chunk) & 0xFF == checksum
                and self.field(slot, "checksumInv") == ~checksum & 0xFF)

    def valid_slots(self) -> list[int]:
        return [i for i in range(self.slot_count) if self.valid(i)]

    def slot_ids(self) -> bytearray:
        """slotIds of the newest record 0, from its first part."""
        firsts = [i for i in self.valid_slots()
                  if self.field(i, "recordId") == 0 and self.field(i, "part") & 0xF0 == 0]
        if not firsts:
            sys.exit("no valid slot holds record 0")
        return self.bytes_of(max(firsts, key=lambda i: self.field(i, "counter")), "slotIds")

    def record_slots(self, record: int) -> list[int]:
        ids = self.slot_ids()
        slots = [i for i in self.valid_slots()
                 if ids[i] == record and self.field(i, "recordId") == record]
        if not slots:
            sys.exit(f"the save holds no record {record}")
        parts = (self.field(slots[0], "part") & 0x0F) + 1
        if len(slots) != parts:
            sys.exit(f"record {record}: {len(slots)} valid slots of {parts} parts")
        return sorted(slots, key=lambda i: self.field(i, "destOffset"))

    def read_record(self, slots: list[int]) -> bytearray:
        out = bytearray()
        for i in slots:
            dest, length = self.field(i, "destOffset"), self.field(i, "dataLength")
            out[dest:dest + length] = self.bytes_of(i, "data")[:length]
        return out

    def write_record(self, slots: list[int], record: bytes) -> None:
        data_offset = self.header["data"]["offset"]
        for i in slots:
            dest, length = self.field(i, "destOffset"), self.field(i, "dataLength")
            start = i * self.slot_size + data_offset
            self.data[start:start + length] = record[dest:dest + length]
            self.set_checksum(i)

    def set_checksum(self, slot: int) -> None:
        """As Save_WriteRecord: sum the slot with 0 and 0xff in the two
        checksum bytes, then store the sum and its inverse."""
        base = slot * self.slot_size
        checksum = base + self.header["checksum"]["offset"]
        inverse = base + self.header["checksumInv"]["offset"]
        self.data[checksum], self.data[inverse] = 0, 0xFF
        total = sum(self.data[base:base + self.slot_size]) & 0xFF
        self.data[checksum], self.data[inverse] = total, ~total & 0xFF


def struct_record(name: str) -> dict:
    hit = ds.find_struct(name)
    if hit is None:
        sys.exit(f"no struct {name} in build/dsindex.json")
    return hit[1]


def number(text: str, consts: dict[str, int]) -> int:
    """A number, a constant name, or a sum of those."""
    terms = re.findall(r"\s*([+-]?)\s*([A-Za-z_]\w*|0x[0-9a-fA-F]+|\d+)", text)
    if not terms or "".join(s + t for s, t in terms) != re.sub(r"\s", "", text):
        sys.exit(f"cannot read {text!r} as a number")
    total = 0
    for sign, term in terms:
        if term[0].isdigit():
            value = int(term, 0)
        elif term in consts:
            value = consts[term]
        else:
            sys.exit(f"unknown name {term}")
        total += -value if sign == "-" else value
    return total


def resolve(type_name: str, path: str, consts: dict[str, int]) -> dict:
    """The field at path, with its offset from the start of the record."""
    current = type_name
    field = None
    offset = 0
    steps = re.findall(r"\.?(\w+)((?:\[[^\]]+\])*)", path)
    if "".join(n + i for n, i in steps) != path.replace(".", ""):
        sys.exit(f"cannot read the path {path!r}")
    for name, index_text in steps:
        if current is None:
            sys.exit(f"{path}: {field['type']} has no field {name}")
        field = next((f for f in struct_record(current)["fields"] if f["name"] == name), None)
        if field is None:
            sys.exit(f"{current} has no field {name}")
        offset += field["offset"]
        indexes = [number(i, consts) for i in re.findall(r"\[([^\]]+)\]", index_text)]
        if indexes:
            shape = [int(n) for n in re.findall(r"\[(\d+)\]", field["type"])]
            if len(indexes) > len(shape) or any(not 0 <= i < n for i, n in zip(indexes, shape)):
                sys.exit(f"{name}{index_text} is outside {field['type']}")
            rest = shape[len(indexes):]
            count = 1
            for n in rest:
                count *= n
            flat = 0
            for i, n in zip(indexes, shape):
                flat = flat * n + i
            offset += flat * count * field["elem_size"]
            element = field["type"].split("[")[0]
            field = dict(field, size=count * field["elem_size"], count=count,
                         type=element + "".join(f"[{n}]" for n in rest))
        whole = field.get("count", 1) == 1
        current = field.get("struct") if whole else None
    return dict(field, offset=offset, path=path)


def scalar(f: dict) -> bool:
    return f.get("count", 1) == 1 and not f.get("struct") and f["size"] in (1, 2, 4)


def get(record: bytes, f: dict) -> int:
    value = int.from_bytes(record[f["offset"]:f["offset"] + f["size"]], "little")
    low, width = f.get("bits", (0, f["size"] * 8))
    value = value >> low & ((1 << width) - 1)
    if f.get("signed") and value >> (width - 1):
        value -= 1 << width
    return value


def put(record: bytearray, f: dict, value: int) -> None:
    if not scalar(f):
        sys.exit(f"{f['path']} is a {f['type']}; set one element or member of it")
    low, width = f.get("bits", (0, f["size"] * 8))
    lowest, highest = (-(1 << (width - 1)), (1 << (width - 1)) - 1) if f.get("signed") \
        else (0, (1 << width) - 1)
    if not lowest <= value <= highest:
        sys.exit(f"{f['path']}: {value} is outside {lowest}..{highest}")
    mask = (1 << width) - 1
    start, end = f["offset"], f["offset"] + f["size"]
    word = int.from_bytes(record[start:end], "little")
    word = word & ~(mask << low) | (value & mask) << low
    record[start:end] = word.to_bytes(f["size"], "little")


def show(record: bytes, type_name: str, path: str, consts: dict[str, int]) -> None:
    if path:
        top = resolve(type_name, path, consts)
        if top.get("struct") and top.get("count", 1) == 1:
            fields = ds.flatten(struct_record(top["struct"])["fields"], top["offset"], path + ".")
        else:
            fields = [top]
    else:
        fields = ds.flatten(struct_record(type_name)["fields"])
    for f in fields:
        if f.get("struct") and f.get("count", 1) == 1:
            continue
        if scalar(f):
            value = str(get(record, f))
        else:
            raw = bytes(record[f["offset"]:f["offset"] + f["size"]])
            value = raw[:16].hex() + ("..." if len(raw) > 16 else "")
        print(f"{f['offset']:#06x}  {f['path']:56} {f['type']:24} {value}")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name in ("slots", "show", "set"):
        p = sub.add_parser(name)
        p.add_argument("sav", type=Path)
        if name == "slots":
            continue
        p.add_argument("--record", type=int, default=0, help="record id (default 0, the progress data)")
        p.add_argument("--type", help="struct of the record (default: from RECORD_TYPES)")
        if name == "show":
            p.add_argument("path", nargs="?", default="")
        else:
            p.add_argument("assignments", nargs="+", metavar="PATH=VALUE")
            p.add_argument("-o", "--out", type=Path, required=True,
                           help="file to write (may be SAV itself)")
    args = ap.parse_args()

    save = Save(args.sav.read_bytes())
    if args.cmd == "slots":
        ids = save.slot_ids()
        for i in save.valid_slots():
            part = save.field(i, "part")
            state = "" if ids[i] == save.field(i, "recordId") else "  (old copy)"
            print(f"slot {i:2}  record {save.field(i, 'recordId'):3}  part {part >> 4} of "
                  f"{(part & 0x0F) + 1}  counter {save.field(i, 'counter')}  "
                  f"{save.field(i, 'dataLength'):#06x} bytes at {save.field(i, 'destOffset'):#06x}{state}")
        return 0

    type_name = args.type or RECORD_TYPES.get(args.record)
    if type_name is None:
        sys.exit(f"no struct known for record {args.record}; pass --type")
    slots = save.record_slots(args.record)
    record = save.read_record(slots)
    size = struct_record(type_name)["size"]
    if len(record) != size:
        sys.exit(f"record {args.record} is {len(record):#x} bytes and {type_name} {size:#x}: "
                 "the header and the save disagree")

    if args.cmd == "show":
        show(record, type_name, args.path, save.consts)
        return 0

    for assignment in args.assignments:
        path, sep, text = assignment.partition("=")
        if not sep:
            sys.exit(f"{assignment}: expected PATH=VALUE")
        f = resolve(type_name, path.strip(), save.consts)
        before = get(record, f) if scalar(f) else None
        put(record, f, number(text, save.consts))
        print(f"{path.strip()}: {before} -> {get(record, f)}")
    save.write_record(slots, record)
    args.out.write_bytes(save.data)
    print(f"wrote {args.out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
