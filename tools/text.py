#!/usr/bin/env python3
"""Game text: unpack the text banks of ov000 to YAML files, pack them back.

    text.py unpack [--out DIR]                     every bank -> DIR/banks.yaml + one YAML file per bank
    text.py pack [YAML...] [--dir DIR] [--out DIR] [--c]
                                                   bank files -> their bytes in ov000, or the C the
                                                   build compiles with --c
    text.py check                                  unpack and pack every bank in memory; compare with the ROM
    text.py get ID [--rom | --built]               one string by text id (0xf0000123)
    text.py grep PATTERN [--rom | --built]         the ids of the strings a regular expression finds

Text_Get(id) walks gTextBanks (ov000), a list of {first id, string table}
sorted by first id going down, and returns table[id & 0xffffff] of the
first bank whose first id is at or below id. Each table is an array of
pointers to strings. A string is 8-bit text: ASCII and Latin-1 letters, a
byte 0x0d for a new line and control codes below 0x20, ended by a 0 byte.
See docs/text.md for the codes and the banks.

The strings and tables of all banks are one run of ov000 .data,
0x022f6d78-0x0234eb64, bank after bank. Inside a bank the strings and the
table are in order of size, smallest first, as mwccarm emits the data
objects of one file (tools/mwcc_order.py). An empty string sits in ov000
.bss, where the compiler puts objects that are all zero.

The YAML files hold game data from the builder's ROM: they belong in
extracted/ or build/ and never enter git (tenet 9). The build compiles
ov000's text from them: `pack --c` (the TEXT step) writes the C that the
committed files in src/game/ov000/text/ include.

  extracted/AWRE/assets/text/
      banks.yaml               the bank files in gTextBanks order
      f0000000.yaml            one bank, named by its first id: the strings by index
  build/gen/text/              written by pack --c
      bank_f0000000.inc        one bank's strings and table, for text/banks/text_bank_f0000000.c
      text_banks.h, .inc       the tables' declarations and the values of gTextBanks
"""

import argparse
import hashlib
import re
import sys
from pathlib import Path

import yaml

import mwcc_order

ROOT = Path(__file__).resolve().parent.parent
OV000 = Path("extract/arm9_overlays/ov000.bin")
OVERLAYS = Path("extract/arm9_overlays/overlays.yaml")
SYMBOLS = Path("config/AWRE/arm9/overlays/ov000/symbols.txt")
BUILT_OV000 = Path("build/build/arm9_ov000.bin")  # get and grep --built
BUILT_OVERLAYS = Path("build/build/arm9_overlays.yaml")
BUILT_MAP = Path("build/arm9.elf.xMAP")
EXTRACTED = Path("extracted/AWRE/assets/text")  # gitignored
UNPACKED = ".unpacked"  # written last by unpack, so a build without the files fails by name
PACKED = Path("build/text")
GEN = Path("build/gen/text")  # pack --c; configure.py adds -i build/gen
# One committed file per bank of the ROM, text_bank_<first id>.c, each with a
# delinks.txt entry over the bank's range. It includes bank_<first id>.inc.
GLUE = Path("src/game/ov000/text/banks")
BANK_LIST = "banks.yaml"
TABLE = "table"  # the pointer table's place in a bank's `order`
INDEX_MASK = 0xFFFFFF

# ---------------------------------------------------------------------------
# Encoding
#
# One byte per character. 0x20-0x7e are ASCII and 0xa1-0xff Latin-1: the
# glyph widths in Ov000_gGlyphWidths fit those letters (0xe9 is the e of
# "passé"). 0x0d is a line break, written as a new line. The codes that
# TextPrinter_ReadControlCode handles are written in braces, and every other
# byte as {0xNN}, so any string comes back to the same bytes.

NEWLINE = 0x0D
SIMPLE = {0x0C: "clear", 0x0E: "pause", 0x0F: "wait"}
# code: (name, lowest parameter byte, highest, value written for the lowest)
WITH_PARAM = {
    0x09: ("icon", 0x80, 0xFF, 0),    # PopupMenu_SetItemIcon(byte - 0x80)
    0x0A: ("cargo", 0x11, 0x12, 1),   # the acting unit's first or second cargo
    0x0B: ("speed", 0x10, 0x19, 0),   # 0: the rest at once, 1-9: frames per character
}
RUNS = {"color": (0x10, 4), "space": (0x18, 8)}  # name: (first code, count); written as code - first code
SOFT_HYPHEN = 0xAD  # invisible in an editor, so written as {0xad}


def decode(raw: bytes) -> str:
    out = []
    i = 0
    while i < len(raw):
        c = raw[i]
        i += 1
        if c in WITH_PARAM and i < len(raw):
            name, low, high, first = WITH_PARAM[c]
            if low <= raw[i] <= high:
                out.append(f"{{{name}:{raw[i] - low + first}}}")
                i += 1
                continue
        run = next((n for n, (start, count) in RUNS.items() if start <= c < start + count), None)
        if c == NEWLINE:
            out.append("\n")
        elif c in SIMPLE:
            out.append(f"{{{SIMPLE[c]}}}")
        elif run:
            out.append(f"{{{run}:{c - RUNS[run][0]}}}")
        elif 0x20 <= c <= 0x7E and c != ord("{") or 0xA1 <= c <= 0xFF and c != SOFT_HYPHEN:
            out.append(chr(c))
        else:
            out.append(f"{{{c:#04x}}}")
    return "".join(out)


def encode(text: str) -> bytes:
    """The bytes of a string as decode writes it; raises ValueError on what it cannot write."""
    out = bytearray()
    i = 0
    while i < len(text):
        ch = text[i]
        if ch == "{":
            end = text.find("}", i)
            if end < 0:
                raise ValueError(f"a {{ at {i} has no }}")
            out += encode_code(text[i + 1:end])
            i = end + 1
            continue
        o = ord(ch)
        if ch == "\n":
            out.append(NEWLINE)
        elif 0x20 <= o <= 0x7E or 0xA1 <= o <= 0xFF:
            out.append(o)
        else:
            raise ValueError(f"{ch!r} at {i} has no byte in the game's font")
        i += 1
    return bytes(out)


def encode_code(code: str) -> bytes:
    if re.fullmatch(r"0x[0-9a-fA-F]{1,2}", code):
        return bytes([int(code, 16)])
    name, _, value = code.partition(":")
    for byte, simple in SIMPLE.items():
        if name == simple and not value:
            return bytes([byte])
    if value.isdigit():
        n = int(value)
        for byte, (pname, low, high, first) in WITH_PARAM.items():
            if name == pname and 0 <= n - first <= high - low:
                return bytes([byte, low + n - first])
        if name in RUNS and n < RUNS[name][1]:
            return bytes([RUNS[name][0] + n])
    raise ValueError(f"unknown code {{{code}}}")


# ---------------------------------------------------------------------------
# A bank
#
# A dict: first_id, table (symbol of the pointer table), address (where the
# bank starts in ov000; None for a bank the ROM does not have), sha1 (of the
# ROM's bytes), entries, order. An entry is a string; {"same": I} for the
# string of entry I; {"bss": SYMBOL} for an empty string in .bss; or None
# for a null pointer. order lists the bank's data objects in the order they
# sit in the ROM: entry indices and TABLE.


class Ov000:
    """ov000 of the ROM (extract/), or with built=True the one the last build
    linked, with its symbols from the linker's map."""

    def __init__(self, built: bool = False):
        path = ROOT / (BUILT_OV000 if built else OV000)
        if not path.exists():
            sys.exit(f"error: {shown(path)} not found; run {'make' if built else 'make extract'}")
        self.data = path.read_bytes()
        info = yaml.safe_load((ROOT / (BUILT_OVERLAYS if built else OVERLAYS)).read_text())["overlays"][0]
        self.base = info["base_address"]
        self.bss_end = self.base + len(self.data) + info["bss_size"]
        self.symbols, self.names = built_symbols(self.base, self.bss_end) if built else read_symbols()

    def word(self, addr: int) -> int:
        return int.from_bytes(self.data[addr - self.base:addr - self.base + 4], "little")

    def in_data(self, addr: int) -> bool:
        return self.base <= addr < self.base + len(self.data)

    def in_bss(self, addr: int) -> bool:
        return self.base + len(self.data) <= addr < self.bss_end

    def cstring(self, addr: int) -> bytes:
        start = addr - self.base
        return self.data[start:self.data.index(b"\0", start)]

    def bytes(self, addr: int, size: int) -> bytes:
        return self.data[addr - self.base:addr - self.base + size]


def read_symbols() -> tuple[dict[str, tuple[int, int]], dict[int, str]]:
    """ov000's symbols: name -> (address, size), and address -> the first name there."""
    symbols, names = {}, {}
    for line in (ROOT / SYMBOLS).read_text().splitlines():
        m = re.match(r"(\S+) kind:(\S+) addr:(0x[0-9a-f]+)", line)
        if m:
            size = re.search(r"\[(\d+)\]|size=(0x[0-9a-f]+)", m.group(2))
            addr = int(m.group(3), 16)
            symbols[m.group(1)] = (addr, int(size.group(1) or size.group(2), 0) if size else 0)
            names.setdefault(addr, m.group(1))
    return symbols, names


def built_symbols(start: int, end: int) -> tuple[dict[str, tuple[int, int]], dict[int, str]]:
    """The symbols the linker placed in start-end, as read_symbols gives them."""
    symbols, names = {}, {}
    for line in (ROOT / BUILT_MAP).read_text().splitlines():
        m = re.match(r"  ([0-9A-F]{8}) ([0-9A-F]{8}) \.(?:data|bss)\s+(\S+)\t", line)
        if m and start <= int(m.group(1), 16) < end:
            addr = int(m.group(1), 16)
            symbols[m.group(3)] = (addr, int(m.group(2), 16))
            names.setdefault(addr, m.group(3))
    return symbols, names


def rom_banks(ov: Ov000) -> tuple[list[dict], int]:
    """Every bank of gTextBanks, in its order, and the first id of the entry that ends the list."""
    at = ov.symbols["gTextBanks"][0]
    banks = []
    while True:
        first_id, table = ov.word(at), ov.word(at + 4)
        at += 8
        if not table:
            return banks, first_id
        banks.append(rom_bank(ov, first_id, table))


def rom_bank(ov: Ov000, first_id: int, table: int) -> dict:
    name = ov.names[table]
    count = ov.symbols[name][1] // 4
    entries: list = []
    objects = {table: (TABLE, count * 4)}  # address -> (order key, size)
    first_use: dict[int, int] = {}
    for i in range(count):
        p = ov.word(table + 4 * i)
        if p == 0:
            entries.append(None)
        elif p in first_use:
            entries.append({"same": first_use[p]})
        elif ov.in_data(p):
            raw = ov.cstring(p)
            first_use[p] = i
            objects[p] = (i, len(raw) + 1)
            entries.append(decode(raw))
        elif ov.in_bss(p) and p in ov.names:
            entries.append({"bss": ov.names[p]})
        else:
            sys.exit(f"error: entry {i:#x} of {name} points to {p:#010x}, outside ov000")
    start = min(objects)
    end = max(a + size for a, (_, size) in objects.items())
    return {"first_id": first_id, "table": name, "address": start,
            "sha1": hashlib.sha1(ov.bytes(start, end - start)).hexdigest(),
            "entries": entries, "order": [objects[a][0] for a in sorted(objects)]}


def layout(bank: dict) -> tuple[list[tuple[object, bytes | None]], dict[object, int]]:
    """The bank's data objects in memory order, with their bytes (None for the
    table), and the offset of each from the bank's start. Objects go smallest
    first, as the compiler emits them; objects of one size keep the order of
    `order`, and the ones it does not list follow in index order. An empty
    string is no object (see empty_string)."""
    entries = bank["entries"]
    rank = {key: k for k, key in enumerate(bank["order"])}
    objects = []
    for i, e in enumerate(entries):
        if e == "":
            empty_string(bank)  # raises if it has nowhere to point
        elif isinstance(e, str):
            try:
                raw = encode(e) + b"\0"
            except ValueError as err:
                raise ValueError(f"entry {i:#x}: {err}") from None
            objects.append((len(raw), rank.get(i, len(rank) + i), i, raw))
        elif isinstance(e, dict) and "same" in e and not (isinstance(entries[e["same"]], str) and entries[e["same"]]):
            raise ValueError(f"entry {i:#x}: same: {e['same']:#x} is not a string, or an empty one")
    objects.append((4 * len(entries), rank.get(TABLE, len(rank) + len(entries)), TABLE, None))
    objects.sort(key=lambda o: o[:2])
    offsets, pos = {}, 0
    for size, _, key, _ in objects:
        offsets[key] = pos
        pos = (pos + size + 3) & ~3
    return [(key, raw) for _, _, key, raw in objects], offsets


def empty_string(bank: dict) -> tuple[int, int]:
    """Where an entry written as "" points: (index, offset) of the 0 byte that
    ends the bank's first string that is not empty. A string of its own would
    be all zero, and the compiler would put it in .bss, where the build has
    no place for a bank's objects; the ROM's banks point their empty strings
    at the 4-byte objects in .bss instead ({bss: SYMBOL})."""
    for i, e in enumerate(bank["entries"]):
        if isinstance(e, str) and e:
            return i, len(encode(e))
    raise ValueError('an empty string ("") needs a string that is not empty in its bank, or write it as {bss: SYMBOL}')


def pack_bytes(bank: dict, bss_address) -> bytes:
    """The bank's strings and table as they sit in ov000 at bank["address"]."""
    if bank.get("address") is None:
        raise ValueError("no address: a bank the ROM does not have can only be written as C (--c)")
    objects, offsets = layout(bank)
    base = bank["address"]
    out = bytearray()
    for key, raw in objects:
        out += bytes(offsets[key] - len(out))
        if raw is not None:
            out += raw
            continue
        for i, e in enumerate(bank["entries"]):
            if e is None:
                word = 0
            elif e == "":
                index, offset = empty_string(bank)
                word = base + offsets[index] + offset
            elif isinstance(e, str):
                word = base + offsets[i]
            elif "same" in e:
                word = base + offsets[e["same"]]
            else:
                word = bss_address(e["bss"])
            out += word.to_bytes(4, "little")
    return bytes(out)


def c_literal(raw: bytes) -> str:
    out = []
    for k, b in enumerate(raw):
        ch = chr(b)
        if b in (0x22, 0x5C) or not 0x20 <= b <= 0x7E or ch == "?" and raw[k + 1:k + 2] == b"?":
            out.append(f"\\{b:03o}")  # three digits, so a digit after it is not taken in
        else:
            out.append(ch)
    return '"' + "".join(out) + '"'


def pack_c(bank: dict) -> str:
    """C that defines the bank's strings and table. The definitions are in the
    order that makes mwccarm emit them as `layout` places them."""
    objects, _ = layout(bank)
    table = bank["table"]
    sizes = [4 * len(bank["entries"]) if raw is None else len(raw) for _, raw in objects]
    definition = mwcc_order.definition_order(sizes)
    name = {key: table if key == TABLE else f"{table}_{key:03x}" for key, _ in objects}
    lines = [f"/* Text bank {bank['first_id']:#010x}, written by tools/text.py pack (docs/text.md). */",
             '#include "types.h"', ""]
    bss = sorted({e["bss"] for e in bank["entries"] if isinstance(e, dict) and "bss" in e})
    lines += [f"extern char {s}[];" for s in bss] + ([""] if bss else [])
    defined = set()
    for k in definition:
        key, raw = objects[k]
        if raw is not None:
            lines.append(f"char {name[key]}[] = {c_literal(raw[:-1])};")
            defined.add(key)
            continue
        later = [name[i] for i, e in enumerate(bank["entries"]) if e and isinstance(e, str) and i not in defined]
        lines += [f"extern char {n}[];" for n in later]
        lines.append(f"const void* {table}[] = {{")
        for i, e in enumerate(bank["entries"]):
            if e == "":
                index, offset = empty_string(bank)
                target = f"{name[index]} + {offset}"
            else:
                target = ("0" if e is None else name[i] if isinstance(e, str)
                          else name[e["same"]] if "same" in e else e["bss"])
            lines.append(f"    {target}, /* {i:#x} */")
        lines.append("};")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Files


def quote(text: str) -> str:
    return '"' + text.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n") + '"'


def bank_file(first_id: int) -> str:
    return f"{first_id:08x}.yaml"


def bank_yaml(bank: dict) -> str:
    lines = [f"# Text bank {bank['first_id']:#010x}: Text_Get ids {bank['first_id']:#010x} + index. "
             "Written by tools/text.py unpack;",
             "# the codes in braces are in docs/text.md. A new string takes the next index.",
             f"first_id: {bank['first_id']:#010x}",
             f"table: {bank['table']}"]
    if bank.get("address") is not None:
        lines.append(f"address: {bank['address']:#010x}")
    if bank.get("sha1"):
        lines.append(f"sha1: {bank['sha1']}")
    lines.append("strings:")
    for i, e in enumerate(bank["entries"]):
        if e is None:
            value = "null"
        elif isinstance(e, str):
            value = quote(e)
        elif "same" in e:
            value = f"{{same: {e['same']:#05x}}}"
        else:
            value = f"{{bss: {e['bss']}}}"
        lines.append(f"  {i:#05x}: {value}")
    lines.append("# The data objects in memory order (entry indices and the table). Objects go")
    lines.append("# smallest first; this list orders the ones of one size. A string not listed")
    lines.append("# goes after the listed ones of its size.")
    keys = [k if k == TABLE else f"{k:#05x}" for k in bank["order"]]
    rows = [", ".join(keys[k:k + 12]) for k in range(0, len(keys), 12)]
    lines.append("order: [" + ",\n  ".join(rows) + "]")
    return "\n".join(lines) + "\n"


def read_bank(text: str, where: str) -> dict:
    doc = yaml.safe_load(text)
    try:
        strings = doc["strings"] or {}
        first_id, table = int(doc["first_id"]), str(doc["table"])
    except (KeyError, TypeError, ValueError):
        raise ValueError(f"{where}: needs first_id, table and strings") from None
    if sorted(strings) != list(range(len(strings))):
        missing = sorted(set(range(max(strings, default=-1) + 1)) - set(strings))
        raise ValueError(f"{where}: string indices must run from 0 with no gap; missing {missing[:8]}")
    entries = [strings[i] for i in range(len(strings))]
    for i, e in enumerate(entries):
        same = isinstance(e, dict) and isinstance(e.get("same"), int) and 0 <= e["same"] < len(entries)
        bss = isinstance(e, dict) and isinstance(e.get("bss"), str)
        if not (e is None or isinstance(e, str) or (same or bss) and len(e) == 1):
            raise ValueError(f"{where}: entry {i:#x}: expected a string, {{same: INDEX}}, {{bss: SYMBOL}} or null")
    return {"first_id": first_id, "table": table, "address": doc.get("address"), "sha1": doc.get("sha1"),
            "entries": entries, "order": doc.get("order") or []}


def banks_yaml(banks: list[dict], end_id: int) -> str:
    lines = ["# gTextBanks (ov000) in its order: Text_Get takes the first bank whose first id",
             "# is at or below the id, so the first ids go down. Written by tools/text.py unpack.",
             "banks:"]
    for bank in banks:
        lines.append(f"- {bank_file(bank['first_id'])}  # {len(bank['entries'])} strings, {bank['table']}")
    lines.append(f"end: {end_id:#010x}  # first id of the entry that ends the list (its table is 0)")
    return "\n".join(lines) + "\n"


def read_dir(directory: Path) -> tuple[list[dict], int]:
    """The banks of an unpacked directory, in banks.yaml order, and the end id."""
    listing = directory / BANK_LIST
    if not listing.exists():
        sys.exit(f"error: {listing} not found; run tools/text.py unpack")
    doc = yaml.safe_load(listing.read_text())
    banks = []
    for name in doc["banks"]:
        try:
            banks.append(read_bank((directory / name).read_text(), name))
        except (OSError, ValueError) as err:
            sys.exit(f"error: {err}")
    ids = [b["first_id"] for b in banks]
    if ids != sorted(ids, reverse=True) or len(set(ids)) != len(ids):
        sys.exit(f"error: {listing}: the first ids must go down, with no two the same")
    return banks, int(doc.get("end", 0xFFFFFFFF))


def banks_h(banks: list[dict]) -> str:
    """The declarations of the banks' tables, for the file that defines gTextBanks."""
    lines = ["/* The banks' string tables, written by tools/text.py pack from banks.yaml. */"]
    lines += [f"extern const void* {b['table']}[];" for b in banks]
    return "\n".join(lines) + "\n"


def banks_inc(banks: list[dict], end_id: int) -> str:
    """The values of gTextBanks, for the C that includes them inside its initializer."""
    lines = ["/* gTextBanks, written by tools/text.py pack from banks.yaml. */"]
    lines += [f"{b['first_id']:#010x}, (void*)&{b['table']}," for b in banks]
    lines.append(f"{end_id:#010x}, 0,")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Commands


def shown(path: Path) -> Path:
    return path.relative_to(ROOT) if path.is_relative_to(ROOT) else path


def load(args) -> list[dict]:
    if args.rom or args.built:
        return rom_banks(Ov000(built=args.built))[0]
    return read_dir(ROOT / args.dir)[0]


def lookup(banks: list[dict], text_id: int) -> tuple[dict, int] | None:
    """The bank and index Text_Get reads for text_id."""
    for bank in banks:
        if text_id >= bank["first_id"]:
            return bank, text_id & INDEX_MASK
    return None


def resolve(bank: dict, index: int) -> str:
    e = bank["entries"][index]
    if isinstance(e, dict) and "same" in e:
        e = bank["entries"][e["same"]]
    return "" if e is None or isinstance(e, dict) else e


def cmd_unpack(args) -> int:
    ov = Ov000()
    banks, end_id = rom_banks(ov)
    out = ROOT / args.out
    out.mkdir(parents=True, exist_ok=True)
    for old in out.glob("*.yaml"):  # a bank file under an old name must not stay behind
        old.unlink()
    for bank in banks:
        (out / bank_file(bank["first_id"])).write_text(bank_yaml(bank))
    (out / BANK_LIST).write_text(banks_yaml(banks, end_id))
    (out / UNPACKED).write_text("")
    count = sum(len(b["entries"]) for b in banks)
    print(f"unpack: {len(banks)} banks, {count} strings in {shown(out)}")
    return 0


def write_if_changed(path: Path, text: str) -> bool:
    """Write only a changed file, so ninja (restat) recompiles only what changed."""
    if path.exists() and path.read_text() == text:
        return False
    path.write_text(text)
    return True


def slot_ids() -> list[int]:
    """The first ids of the banks that have a committed file in GLUE."""
    return [int(p.stem.removeprefix("text_bank_"), 16) for p in sorted((ROOT / GLUE).glob("text_bank_*.c"))]


def slot_groups(banks: list[dict], slots: list[int]) -> dict[int, list[dict]]:
    """The banks each committed file defines. A bank of the ROM has its own
    file; a bank without one (a new bank) goes into the file of the next bank
    in the list that has one, or of the last such bank. Its objects then sit
    among that bank's, which is no problem in a build that does not match
    retail. A committed file left with no bank defines nothing."""
    groups: dict[int, list[dict]] = {s: [] for s in slots}
    waiting, last = [], None
    for bank in banks:
        waiting.append(bank)
        if bank["first_id"] in groups:
            last = bank["first_id"]
            groups[last] += waiting
            waiting = []
    if waiting:
        if last is None:
            sys.exit(f"error: no bank of banks.yaml has a file in {GLUE}, so the new banks have no place in ov000")
        groups[last] += waiting
    return groups


def renamed_tables(banks: list[dict]) -> None:
    """A bank file unpacked before its table was renamed in symbols.txt names
    it data_ov000_<address>, dsd's name: use the name at that address now, so
    the strings get the names symbols.txt gives them."""
    symbols, names = read_symbols()
    for bank in banks:
        m = re.fullmatch(r"data_ov000_([0-9a-f]{8})", bank["table"])
        if m and bank["table"] not in symbols and int(m.group(1), 16) in names:
            bank["table"] = names[int(m.group(1), 16)]


def pack_build(banks: list[dict], end_id: int, out: Path) -> int:
    """The C the build compiles: bank_<id>.inc for each committed file in
    GLUE, and text_banks.h and text_banks.inc for gTextBanks."""
    renamed_tables(banks)
    note = "/* Generated by tools/text.py pack from the unpacked text banks. Do not edit. */\n"
    c = {}
    for bank in banks:
        try:
            c[bank["first_id"]] = pack_c(bank)
        except ValueError as err:
            sys.exit(f"error: {bank_file(bank['first_id'])}: {err}")
    changed = 0
    for slot, group in slot_groups(banks, slot_ids()).items():
        body = "".join(c[b["first_id"]] for b in group) if group else '#include "types.h"\n'
        changed += write_if_changed(out / f"bank_{slot:08x}.inc", note + body)
    changed += write_if_changed(out / "text_banks.h", banks_h(banks))
    changed += write_if_changed(out / "text_banks.inc", banks_inc(banks, end_id))
    count = sum(len(b["entries"]) for b in banks)
    print(f"pack: {len(banks)} banks, {count} strings as C in {shown(out)}, {changed} files changed")
    return 0


def cmd_pack(args) -> int:
    directory = ROOT / args.dir
    out = ROOT / (args.out or (GEN if args.c else PACKED))
    out.mkdir(parents=True, exist_ok=True)
    if not args.files:
        if not (directory / UNPACKED).exists():
            sys.exit(f"error: no text banks in {args.dir}; run `make setup` (it unpacks them from your ROM)")
        banks, end_id = read_dir(directory)
        if args.c:
            return pack_build(banks, end_id, out)
    else:
        banks = []
        for f in args.files:
            try:
                banks.append(read_bank(Path(f).read_text(), f))
            except (OSError, ValueError) as err:
                sys.exit(f"error: {err}")
    ov = None if args.c else Ov000()
    for bank in banks:
        stem = f"{bank['first_id']:08x}"
        try:
            if args.c:
                (out / f"bank_{stem}.inc").write_text(pack_c(bank))
                print(f"{shown(out)}/bank_{stem}.inc: {len(bank['entries'])} strings")
                continue
            if bank.get("address") is None:
                print(f"{stem}: skipped, not in the ROM, so it has no address; --c writes it as C")
                continue
            data = pack_bytes(bank, lambda s: ov.symbols[s][0])
        except (ValueError, KeyError) as err:
            sys.exit(f"error: {stem}: {err}")
        (out / f"{stem}.bin").write_bytes(data)
        same = hashlib.sha1(data).hexdigest() == bank.get("sha1")
        print(f"{shown(out / stem)}.bin: {len(data)} bytes at {bank['address']:#010x}"
              f"{', same as the ROM' if same else ', changed'}")
    return 0


def cmd_check(args) -> int:
    ov = Ov000()
    banks, _ = rom_banks(ov)
    bad = []
    for bank in banks:
        where = bank_file(bank["first_id"])
        back = read_bank(bank_yaml(bank), where)  # through the file format, as an unchanged file
        data = pack_bytes(back, lambda s: ov.symbols[s][0])
        if data != ov.bytes(bank["address"], len(data)) or hashlib.sha1(data).hexdigest() != bank["sha1"]:
            bad.append(where)
    count = sum(len(b["entries"]) for b in banks)
    print(f"check: {len(banks) - len(bad)} of {len(banks)} banks ({count} strings) come back the same, "
          f"{len(bad)} differ")
    for name in bad:
        print(f"  differs: {name}")
    return 1 if bad else 0


def one_line(text: str) -> str:
    return text.replace("\n", "\\n")


def cmd_get(args) -> int:
    text_id = int(args.id, 0)
    found = lookup(load(args), text_id)
    if not found or found[1] >= len(found[0]["entries"]):
        sys.exit(f"error: no string has id {text_id:#010x}")
    bank, index = found
    e = bank["entries"][index]
    if isinstance(e, dict) and "bss" in e:
        print(f"(empty: {e['bss']} in .bss)")
    else:
        print(resolve(bank, index))
    return 0


def cmd_grep(args) -> int:
    pattern = re.compile(args.pattern, re.I)
    hits = 0
    for bank in load(args):
        for index in range(len(bank["entries"])):
            text = resolve(bank, index)
            if pattern.search(text):
                print(f"{bank['first_id'] | index:#010x}  {one_line(text)}")
                hits += 1
    return 0 if hits else 1


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = parser.add_subparsers(required=True)
    p = sub.add_parser("unpack", help="every bank from the ROM to one YAML file each, and banks.yaml")
    p.add_argument("--out", default=str(EXTRACTED))
    p.set_defaults(func=cmd_unpack)
    p = sub.add_parser("pack", help="bank files to their bytes in ov000, or to C")
    p.add_argument("files", nargs="*", metavar="YAML", help="bank files (default: every file of banks.yaml)")
    p.add_argument("--dir", default=str(EXTRACTED), help="the unpacked directory")
    p.add_argument("--out", help=f"default {PACKED}, or {GEN} with --c")
    p.add_argument("--c", action="store_true",
                   help="write the C the build compiles instead of bytes: with no file named, one file per"
                   f" committed file in {GLUE} and gTextBanks' tables and values from banks.yaml")
    p.set_defaults(func=cmd_pack)
    p = sub.add_parser("check", help="unpack and pack every bank in memory; compare with the ROM")
    p.set_defaults(func=cmd_check)
    for name, func, what in (("get", cmd_get, "one string by text id"),
                             ("grep", cmd_grep, "ids of the strings a regular expression finds (any case)")):
        p = sub.add_parser(name, help=what)
        p.add_argument("id" if name == "get" else "pattern")
        p.add_argument("--dir", default=str(EXTRACTED), help="the unpacked directory")
        p.add_argument("--rom", action="store_true", help="read the ROM instead of the unpacked files")
        p.add_argument("--built", action="store_true", help="read the ov000 of the last build (build/AWRE.nds)")
        p.set_defaults(func=func)
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
