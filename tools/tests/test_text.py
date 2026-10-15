#!/usr/bin/env python3
"""Check tools/text.py.

    python3 tools/tests/test_text.py

- Encoding: every byte from 1 to 0xff, alone and in random strings, comes
  back from decode and encode unchanged; the named codes decode as
  docs/text.md lists them; an unknown code or a character the font lacks
  stops encode.
- Layout: a made-up bank with shared, .bss and null entries packs its
  objects smallest first, in `order` among equal sizes, with the table
  words pointing at them; a string added at the end goes after the listed
  strings of its size; the bank file gives back the bank.
- C: the definitions pack --c writes come out of mwcc_order's model in the
  layout's order.
- Empty strings ("") point at the 0 byte that ends the bank's first string,
  in the bytes and in the C; a bank of empty strings only stops pack.
- Build: a bank without a committed file goes into the file of the next
  bank that has one, or of the last; a table under dsd's old name gets the
  name symbols.txt gives it now.
- With extract/ present: `text.py check` (every bank of the ROM through
  the file format and back to the same bytes), a committed file and a
  delinks.txt entry over its range for every bank of the ROM, and, with
  the compiler from make setup, the C of three banks compiled and its
  objects in the ROM's order. Skipped with a message when there is no
  extract/.
"""

import contextlib
import io
import random
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import configure  # noqa: E402
import mwcc_order  # noqa: E402
import text  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent.parent
failures = []


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if got == want else 'FAIL'}")
    if got != want:
        print(f"  got:  {got!r}\n  want: {want!r}")
        failures.append(what)


def raises(what: str, func) -> None:
    try:
        func()
        expect(what, "no error", "ValueError")
    except ValueError:
        expect(what, "ValueError", "ValueError")


rng = random.Random(3)
singles = [bytes([b]) for b in range(1, 256)]
expect("every byte comes back", [text.encode(text.decode(b)) for b in singles], singles)
mixed = [bytes(rng.randrange(1, 256) for _ in range(rng.randrange(1, 40))) for _ in range(500)]
expect("random strings come back", [text.encode(text.decode(b)) for b in mixed], mixed)
expect("named codes", text.decode(b"\x09\x8aUnit\x0b\x10\x0dA\x0a\x12\x10B\x1f\x0e\x0c\x0f\x16{\xe9"),
       "{icon:10}Unit{speed:0}\nA{cargo:2}{color:0}B{space:7}{pause}{clear}{wait}{0x16}{0x7b}é")
expect("a code without its parameter", text.decode(b"A\x0b\x01"), "A{0x0b}{0x01}")
raises("unknown code stops encode", lambda: text.encode("{shout}"))
raises("a value out of range stops encode", lambda: text.encode("{space:8}"))
raises("a character the font lacks stops encode", lambda: text.encode("snow ☃"))

# A bank of 8 entries: sizes 4 ("abc"), 4 ("xyz"), 2, 6, shared, .bss, null.
bank = {"first_id": 0xF1000000, "table": "gTextBank_f1000000", "address": 0x02000000, "sha1": None,
        "entries": ["abc", "xyz", "q", "hello", {"same": 0}, {"bss": "empty_slot"}, None, "def"],
        "order": [2, 1, 0, 3, "table"]}
symbols = {"empty_slot": 0x02100000}
packed = text.pack_bytes(bank, symbols.__getitem__)
objects, offsets = text.layout(bank)
expect("objects smallest first, ties in order, unlisted last", [k for k, _ in objects], [2, 1, 0, 7, 3, "table"])
expect("offsets on 4 bytes", offsets, {2: 0, 1: 4, 0: 8, 7: 12, 3: 16, "table": 24})
words = list(struct.unpack_from("<8I", packed, offsets["table"]))
expect("table words", words, [0x02000008, 0x02000004, 0x02000000, 0x02000010, 0x02000008, 0x02100000, 0, 0x0200000C])
expect("strings in place", (packed[0:2], packed[8:12], packed[16:22]), (b"q\0", b"abc\0", b"hello\0"))
back = text.read_bank(text.bank_yaml(bank), "f1000000.yaml")
expect("bank file round trip", {k: back[k] for k in ("first_id", "table", "address", "entries", "order")},
       {k: bank[k] for k in ("first_id", "table", "address", "entries", "order")})
raises("a gap in the indices stops read_bank",
       lambda: text.read_bank('first_id: 1\ntable: t\nstrings:\n  0x000: "a"\n  0x002: "b"\n', "gap.yaml"))
raises("same: on a non-string stops pack", lambda: text.layout(dict(bank, entries=[None, {"same": 0}])))

# pack --c: the definition order goes through the compiler's sort to the layout's order.
c = text.pack_c(bank)
defined = [re.search(r"(\w+)\[\]", line).group(1) for line in c.splitlines()
           if line.startswith(("char ", "const void*"))]
names = {"gTextBank_f1000000": "table"} | {f"gTextBank_f1000000_{k:03x}": k for k in range(8)}
sizes = [4 * 8 if names[n] == "table" else len(text.encode(bank["entries"][names[n]])) + 1 for n in defined]
expect("pack --c definition order", [names[defined[i]] for i in mwcc_order.mwcc_heapsort(sizes)],
       [k for k, _ in objects])

# Empty strings: no object of their own; they share the 0 byte that ends "abc".
empty = dict(bank, entries=["", "abc", "", "q"], order=[])
objects, offsets = text.layout(empty)
expect("an empty string is no object", [k for k, _ in objects], [3, 1, "table"])
words = list(struct.unpack_from("<4I", text.pack_bytes(empty, symbols.__getitem__), offsets["table"]))
expect("an empty string points at the end of the first string", words,
       [0x02000007, 0x02000004, 0x02000007, 0x02000000])
expect("an empty string in C", "    gTextBank_f1000000_001 + 3, /* 0x0 */" in text.pack_c(empty).splitlines(), True)
raises("a bank of empty strings only stops pack", lambda: text.layout(dict(bank, entries=["", None], order=[])))

# Build: new banks go into the file of the next bank that has one, or of the last.
ids = [0xF1000000, 0xF0000000, 0xE0000000, 0xC0000000, 0x01000000]
groups = text.slot_groups([{"first_id": i} for i in ids], [0xF0000000, 0xC0000000])
expect("banks without a committed file", {k: [b["first_id"] for b in v] for k, v in groups.items()},
       {0xF0000000: ids[:2], 0xC0000000: ids[2:]})
symbols_txt, names_txt = text.read_symbols()
if "gTextBank_f0000000" in symbols_txt:
    old = {"table": f"data_ov000_{symbols_txt['gTextBank_f0000000'][0]:08x}"}
    text.renamed_tables([old])
    expect("a table under dsd's name gets its name from symbols.txt", old["table"], "gTextBank_f0000000")


def object_symbols(obj: Path, prefix: str) -> list[str]:
    """Symbols starting with prefix, by section index: the order the linker places them in."""
    d = obj.read_bytes()
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", d, 0x2e)
    sections = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    found = []
    for s in sections:
        if s[1] != 2:  # SHT_SYMTAB
            continue
        strings = sections[s[6]][4]
        for j in range(s[5] // 16):
            name, _, _, _, _, shndx = struct.unpack_from("<IIIBBH", d, s[4] + j * 16)
            label = d[strings + name:d.index(b"\0", strings + name)].decode()
            if label.startswith(prefix) and 0 < shndx < 0xFF00:
                found.append((shndx, label))
    return [n for _, n in sorted(found)]


if (ROOT / text.OV000).exists():
    with contextlib.redirect_stdout(io.StringIO()) as out:
        status = text.cmd_check(None)
    expect("text.py check", (status, out.getvalue().split(", ")[-1].strip()), (0, "0 differ"))
    ov = text.Ov000()
    banks, end_id = text.rom_banks(ov)
    delinks = (ROOT / text.SYMBOLS).with_name("delinks.txt").read_text()
    entries = dict(re.findall(r"^(\S+):\n    complete\n    \.data\s+(start:0x\w+ end:0x\w+)$", delinks, re.M))
    missing = []
    for b in banks:
        objects, offsets = text.layout(b)
        end = max(offsets[k] + (4 * len(b["entries"]) if raw is None else len(raw)) for k, raw in objects)
        glue = f"{text.GLUE.as_posix()}/text_bank_{b['first_id']:08x}.c"
        want = f"start:{b['address']:#010x} end:{(b['address'] + end + 3) & ~3:#010x}"
        if not (ROOT / glue).exists() or entries.get(glue) != want:
            missing.append(glue)
    expect("every bank of the ROM has a file and a delinks.txt entry over its range", missing, [])
    cc = ROOT / configure.CC
    if cc.exists():
        with tempfile.TemporaryDirectory() as tmp:
            for bank in [b for b in banks if b["first_id"] in (0xF0000000, 0x43000000, 0x2B000000)]:
                src, obj = Path(tmp) / "bank.c", Path(tmp) / "bank.o"
                src.write_text(text.pack_c(bank))
                r = subprocess.run([str(cc), "-c", *configure.CFLAGS.split(), "-o", str(obj), str(src)],
                                   cwd=ROOT, capture_output=True, text=True)
                objects, _ = text.layout(bank)
                want = [bank["table"] if k == text.TABLE else f"{bank['table']}_{k:03x}" for k, _ in objects]
                got = object_symbols(obj, bank["table"]) if r.returncode == 0 else r.stdout + r.stderr
                expect(f"C of bank {bank['first_id']:#010x} compiles to the ROM's order", got, want)
    else:
        print("C compiled: skipped, no compiler (run make setup)")
else:
    print("text.py check: skipped, no extract/ (run make extract)")

if failures:
    sys.exit(f"test_text: {len(failures)} failed")
