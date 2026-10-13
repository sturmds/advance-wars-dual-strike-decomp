#!/usr/bin/env python3
"""Define a module's .bss in C, so that it links from files marked `complete`
(issue 0082, docs/bss-tool.md, docs/workflow.md "Defining .bss").

    bss.py scan MODULE [--verbose]                  variables, aliases, filler and runs; writes nothing
    bss.py plan MODULE [--range LO-HI] [--show]     the files it would write, and where (--show: their C)
    bss.py plan MODULE [--range LO-HI] --write      write them, build, and check the ROM
    bss.py lcf [--arm7] LCF                         place the extra .bss sections and the aliases in a linker script

MODULE: main, ovNNN or arm7. ARM7 has .bss in two of its three parts and a
.sbss (docs/tool-issues.md); a run in .sbss goes in `.sbss` or `.sbss.runN`.

It works on the parts of each .bss range that no delinks.txt entry covers.
Its inputs are the committed config, src/ and include/, build/fnindex.json
(who uses each symbol), build/dsindex.json (struct sizes) and the
compiler; the same inputs give the same output. The rules:

- Declarations: every `extern` statement in src/ and include/, read here
  (dsindex's regex leaves out function pointers). A variable's size is the
  size a header declares, else the size most declarations agree on (by
  type, or sizeof from a compile), else the size in symbols.txt.
- Variables: one starts at each dsd symbol that is 4-aligned and not
  inside the declared size of the one before. With no size known it runs
  to the next symbol. An array declared with no count gets as many whole
  elements as fit, at least one.
- Aliases: every other symbol, an address inside a variable. symbols.txt
  gives them size 0, and the linker script defines them as
  `VARIABLE + OFFSET` (`bss.py lcf`).
- Filler: bytes after a variable's declared size that no symbol names.
  They become `u8` arrays named by address. Each stretch lies between two
  variables, so each is cut on its own (split_filler): fewest places where
  the size goes down, then fewest pieces. Less than 32 bytes before a
  32-aligned variable can instead be an `aligned(32)` on that variable,
  which wins on a tie because it adds no piece.
- Runs: mwccarm sorts a file's variables by size (tools/mwcc_order.py), so
  a run of ascending sizes is the most one section holds. A file can hold
  several neighbouring runs: the first in .bss, run N in `.bss.runN`
  (include/bss_run.h), which the linker script places after the file's
  .bss and objdiff reads as .bss. At most 16 runs per file.
- Hosts: a run goes into an existing file when more than half of its named
  variables are used by that file's functions (the file with the most, ties
  in fnindex order), the file is `complete`, has its code in the same part
  of the module (ARM7 has three, tools/arm7.py), compiles to code only, declares
  the variables with their sizes, keeps one link order (dsd stops with
  "Link order cycle detected" otherwise), and compiles with the
  definitions in the right order.
- New files: in the directory where the users of most of the run's
  variables are, else the module's directory. Named `<directory>_bss.c` in
  a topic directory, else `bss_<address>.c` (`ovNNN_bss_<address>.c` in an
  overlay); the address goes on the end when the name is taken.
- Neighbouring runs share a file when they go to the same host, when a run
  with no host goes to the directory of the file before it, when a run with
  no users follows any file, or when a hosted run follows a new file in the
  host's directory.
- The C: definitions in the order mwcc_order.definition_order gives, each
  a copy of a declaration without `extern` (the host's own, else a
  header's, else the most common one), or a `u8` array when there is none
  or its type is in no header. A host's `extern` statements for them are
  replaced; when that does not compile, the definitions go at the end of
  the file. A variable only the host file names becomes `static`.
"""

import argparse
import dataclasses
import json
import math
import re
import struct
import subprocess
import sys
import tempfile
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from fractions import Fraction
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import configure  # noqa: E402
import data_inc  # noqa: E402
from mwcc_order import definition_order  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
FNINDEX = ROOT / "build/fnindex.json"
DSINDEX = ROOT / "build/dsindex.json"
RUN_HEADER = "bss_run.h"
BSS_SECTIONS = (".bss", ".sbss")
# Runs one file can hold in each section: the sections include/bss_run.h
# defines (.bss.run1 to .bss.run15, .sbss and .sbss.run1 to .sbss.run3).
MAX_RUNS = {".bss": 16, ".sbss": 4}
# Sections an object may have and still take .bss definitions: with data of
# its own, the compiler would sort that data together with ours.
CODE_SECTIONS = {".text", ".exceptix"}

BASIC_SIZES = {
    "char": 1, "unsigned char": 1, "signed char": 1, "u8": 1, "s8": 1, "BOOL8": 1,
    "short": 2, "unsigned short": 2, "u16": 2, "s16": 2, "fx16": 2,
    "int": 4, "unsigned int": 4, "unsigned": 4, "long": 4, "unsigned long": 4, "u32": 4, "s32": 4,
    "BOOL": 4, "fx32": 4, "f32": 4, "float": 4,
    "long long": 8, "unsigned long long": 8, "u64": 8, "s64": 8, "fx64": 8, "double": 8,
}


def fail(msg: str) -> None:
    sys.exit(f"bss.py: error: {msg}")


# ---------------------------------------------------------------- config files

@dataclass
class Entry:
    name: str
    complete: bool
    sections: dict[str, tuple[int, int]]


@dataclass
class Symbol:
    name: str
    kind: str
    addr: int
    size: int | None


SECTION_LINE = re.compile(r"^\s+(\.\w+)\s+start:(0x[0-9a-fA-F]+)\s+end:(0x[0-9a-fA-F]+)")
SYMBOL_LINE = re.compile(r"^(\S+) kind:(\w+)(?:\(([^)]*)\))? addr:(0x[0-9a-fA-F]+)")


def read_delinks(path: Path) -> tuple[list[tuple[str, int, int]], list[Entry]]:
    """The module's sections (name, start, end) and its file entries."""
    header: list[tuple[str, int, int]] = []
    entries: list[Entry] = []
    cur: Entry | None = None
    for line in path.read_text().splitlines():
        text = line.split("//")[0].rstrip()
        if not text.strip():
            cur = None
            continue
        if not text[0].isspace():
            cur = Entry(text.strip().rstrip(":"), False, {})
            entries.append(cur)
            continue
        if text.strip() == "complete":
            cur.complete = True
            continue
        m = SECTION_LINE.match(text)
        if m:
            sec = (m.group(1), int(m.group(2), 16), int(m.group(3), 16))
            if cur is None:
                header.append(sec)
            else:
                cur.sections[sec[0]] = sec[1:]
    return header, entries


def read_symbols(path: Path) -> list[Symbol]:
    syms = []
    for line in path.read_text().splitlines():
        m = SYMBOL_LINE.match(line)
        if not m:
            continue
        size = None
        if m.group(2) == "bss" and m.group(3):
            sm = re.search(r"size=(0x[0-9a-fA-F]+|\d+)", m.group(3))
            size = int(sm.group(1), 0) if sm else None
        syms.append(Symbol(m.group(1), m.group(2), int(m.group(4), 16), size))
    return syms


def symbol_prefix(module: str) -> str:
    return {"main": "data_", "arm7": "data_arm7_"}.get(module, f"data_{module}_")


def module_src_dir(module: str) -> str:
    return {"main": "src/game/main", "arm7": "src/arm7/sdk"}.get(module, f"src/game/{module}")


def module_modules() -> list[str]:
    """Every module with a .bss range: main, the overlays, arm7."""
    overlays = sorted(p.name for p in (ROOT / data_inc.CONFIG9 / "overlays").iterdir() if p.is_dir())
    return ["main", *overlays, "arm7"]


# ---------------------------------------------------------------- ELF objects

def elf_read(path: Path) -> tuple[list[dict], list[dict]]:
    """Sections and symbols of a 32-bit little-endian ELF file."""
    d = path.read_bytes()
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", d, 0x2e)
    raw = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]

    def cstr(off: int) -> str:
        return d[off:d.index(b"\0", off)].decode()

    names_off = raw[shstrndx][4]
    sections = [{"name": cstr(names_off + s[0]), "type": s[1], "flags": s[2], "size": s[5], "align": s[8]}
                for s in raw]
    symbols = []
    for s in raw:
        if s[1] != 2:  # SHT_SYMTAB
            continue
        strings = raw[s[6]][4]
        for j in range(s[5] // 16):
            name, value, size, info, _, shndx = struct.unpack_from("<IIIBBH", d, s[4] + j * 16)
            symbols.append({"name": cstr(strings + name), "value": value, "size": size,
                            "type": info & 0xf, "bind": info >> 4, "shndx": shndx})
    return sections, symbols


def emitted_order(elf: tuple[list[dict], list[dict]]) -> dict[str, list[str]]:
    """Object symbols in .bss sections, by section name, in the order the
    linker places them."""
    sections, symbols = elf
    found = sorted((s["shndx"], s["name"]) for s in symbols
                   if s["type"] == 1 and 0 < s["shndx"] < len(sections)
                   and sections[s["shndx"]]["type"] == 8)  # STT_OBJECT in SHT_NOBITS
    out: dict[str, list[str]] = defaultdict(list)
    for shndx, name in found:
        out[sections[shndx]["name"]].append(name)
    return out


# ---------------------------------------------------------------- what we know

class Index:
    """build/fnindex.json and build/dsindex.json, read once."""

    def __init__(self) -> None:
        for path, target in ((FNINDEX, "fnindex"), (DSINDEX, "dsindex")):
            if not path.exists():
                fail(f"{path.relative_to(ROOT)} is missing: run `make {target}`")
        fn = json.loads(FNINDEX.read_text())
        ds = json.loads(DSINDEX.read_text())
        self.functions = fn["functions"]
        self.decls = Decls()
        self.probed: dict[tuple[str, str], int | None] = {}
        self.struct_size: dict[str, int] = {}
        self.struct_headers: dict[str, list[str]] = defaultdict(list)
        sizes_seen: dict[str, set[int]] = defaultdict(set)
        for where in ("headers", "sources"):
            for path, info in ds[where].items():
                for st in info.get("structs", []):
                    sizes_seen[st["name"]].add(st["size"])
                    if where == "headers" and path not in self.struct_headers[st["name"]]:
                        self.struct_headers[st["name"]].append(path)
        for name, sizes in sizes_seen.items():
            if len(sizes) == 1:
                self.struct_size[name] = sizes.pop()

    def base_type(self, type_text: str) -> str:
        t = re.sub(r"\b(const|volatile|struct|union|enum)\b", " ", type_text)
        return " ".join(t.split())

    def type_size(self, type_text: str) -> int | None:
        t = self.base_type(type_text)
        if "*" in t:
            return 4
        return BASIC_SIZES.get(t, self.struct_size.get(t))

    def type_header(self, type_text: str, near: str) -> str | None:
        """The include line a file needs for this type: '' when types.h is
        enough, None when no header defines it. When several headers define
        it (ARM9's and ARM7's `struct OSThread`), the one the file `near`
        includes, the file that declares the variable (pick_header)."""
        t = self.base_type(type_text).replace("*", "").strip()
        if t in BASIC_SIZES or t == "void":
            return ""
        return pick_header(self.struct_headers.get(t) or self.decls.type_headers.get(t, []), near)


def pick_header(paths: list[str], near: str) -> str | None:
    """One of the headers `paths` for a file that declares something with
    a type they define: the only one, else the first that `near` includes,
    else the first. A wrong pick shows up as a size the compile check
    refuses (order_errors)."""
    if len(paths) > 1:
        text = (ROOT / near).read_text(errors="replace")
        paths = [p for p in paths if f'#include "{p.removeprefix("include/")}"' in text] or paths
    return paths[0].removeprefix("include/") if paths else None


_compiled: dict[tuple[str, str], tuple[list[dict], list[dict]] | None] = {}
compile_errors: dict[tuple[str, str], str] = {}


def compile_probe(src: str, text: str) -> tuple[list[dict], list[dict]] | None:
    """Compiles `text` as if it were the file `src`. Returns the object's
    sections and symbols, or None when it does not compile."""
    key = (src, text)
    if key not in _compiled:
        cc, flags = configure.compiler_for(src)
        cmd = [sys.executable, str(ROOT / cc)] if cc.endswith(".py") else [str(ROOT / cc)]
        flags = flags.replace("-i include", f"-i include -i {Path(src).parent}")
        with tempfile.TemporaryDirectory(prefix="bss_probe") as d:
            c, obj = Path(d) / "probe.c", Path(d) / "probe.o"
            c.write_text(text)
            r = subprocess.run([*cmd, "-c", *flags.split(), "-o", str(obj), str(c)], cwd=ROOT,
                               capture_output=True, text=True)
            _compiled[key] = elf_read(obj) if r.returncode == 0 and obj.exists() else None
            if _compiled[key] is None:
                compile_errors[key] = " ".join((r.stdout + r.stderr).split())[:300]
    return _compiled[key]


def context_text(path: str) -> str:
    """C that sees the declarations of `path`: the file itself, or a file
    that includes the header."""
    if path.startswith("include/"):
        return f'#include "types.h"\n#include "{path.removeprefix("include/")}"\n'
    return (ROOT / path).read_text()


def probe_size(path: str, expr: str) -> int | None:
    """sizeof(expr) where `path` declares it, from a compile; None when the
    file does not give it (`extern u8 x[];`)."""
    src = path if path.startswith("src/") else "src/probe.c"
    elf = compile_probe(src, context_text(path) + f"\nchar bss_probe[sizeof({expr})] = {{1}};\n")
    if elf is None:
        return None
    return next((s["size"] for s in elf[1] if s["name"] == "bss_probe"), None)


EXTERN_STATEMENT = re.compile(r"^extern\b[^;{}]*;", re.M)
# A comment of whole lines that ends on the line just above a statement.
LEAD_COMMENT = re.compile(r"(?:^|(?<=\n))(/\*(?:(?!\*/).)*\*/)[ \t]*\n\Z", re.S)
TYPEDEF_NAME = re.compile(r"\btypedef\b[^;{}]*?(?:\(\s*\*\s*(\w+)\s*\)[^;{}]*|\b(\w+)\s*(?:\[[^\]]*\]\s*)*);")
CLOSE_NAME = re.compile(r"\}\s*(\w+)\s*;")
DEFINE_NAME = re.compile(r"^\s*#\s*define\s+(\w+)", re.M)
ENUM_BODY = re.compile(r"\benum\b[^{;]*\{([^}]*)\}")


def declarator(body: str) -> str | None:
    """The name an `extern` statement declares (`body` without `extern` and
    `;`), or None for a function or a list of several names."""
    depth = 0
    for ch in body:
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        elif ch == "," and depth == 0:
            return None
    m = re.search(r"\(\s*\*\s*(\w+)", body)
    if m:
        return m.group(1)
    if body.rstrip().endswith(")"):
        return None
    m = re.search(r"(\w+)$", re.sub(r"(\s*\[[^\]]*\])+$", "", body.rstrip()))
    return m.group(1) if m else None


@dataclass
class Decl:
    name: str
    file: str
    start: int      # where the statement starts in the file
    end: int        # just past the `;`
    body: str       # the statement without `extern` and `;`, on one line
    comment: str    # a /* */ comment after it on the same line
    lead: str = ""  # a /* */ comment on the lines just above it, as written
    lead_start: int = -1  # where that comment starts (start when there is none)


class Decls:
    """The `extern` statements in src/ and include/, by the name they
    declare. Read from the files rather than from build/dsindex.json, whose
    regex leaves out function pointers."""

    def __init__(self) -> None:
        self.by_name: dict[str, list[Decl]] = defaultdict(list)
        self.type_headers: dict[str, list[str]] = defaultdict(list)
        for top, pattern in (("include", "*.h"), ("src", "*.c")):
            for path in sorted((ROOT / top).rglob(pattern)):
                rel = path.relative_to(ROOT).as_posix()
                text = path.read_text(errors="replace")
                for m in EXTERN_STATEMENT.finditer(text):
                    body = " ".join(m.group(0)[len("extern"):-1].split())
                    name = declarator(body)
                    if name is None:
                        continue
                    rest = text[m.end():text.find("\n", m.end()) if "\n" in text[m.end():] else len(text)]
                    cm = re.fullmatch(r"\s*/\*(.*)\*/\s*", rest)
                    lm = LEAD_COMMENT.search(text, 0, m.start())
                    self.by_name[name].append(Decl(name, rel, m.start(), m.end(), body,
                                                   " ".join(cm.group(1).split()) if cm else "",
                                                   lm.group(1) if lm else "", lm.start(1) if lm else m.start()))
                if top == "include":
                    for m in DEFINE_NAME.finditer(text):
                        self.add_type(m.group(1), rel)
                    for block in ENUM_BODY.finditer(text):
                        for m in re.finditer(r"^\s*([A-Za-z_]\w*)\s*[=,}]", block.group(1) + ",", re.M):
                            self.add_type(m.group(1), rel)
                    for m in TYPEDEF_NAME.finditer(text):
                        self.add_type(m.group(1) or m.group(2), rel)
                    for m in CLOSE_NAME.finditer(text):
                        self.add_type(m.group(1), rel)

    def add_type(self, name: str, header: str) -> None:
        if header not in self.type_headers[name]:
            self.type_headers[name].append(header)

    def of(self, name: str) -> list[Decl]:
        return self.by_name.get(name, [])


def decl_size(index: "Index", d: Decl) -> int | None:
    """sizeof the declared object; None for an array with no count."""
    m = re.fullmatch(r"((?:\w+\s+)*\w+[\s*]*?)\s*\b" + d.name + r"\s*((?:\[\s*\w*\s*\])*)", d.body)
    if m:
        dims = re.findall(r"\[\s*(\w*)\s*\]", m.group(2))
        if "" in dims:
            return None
        size = index.type_size(m.group(1))
        for n in dims:
            size = size * int(n, 0) if size is not None and re.fullmatch(r"0x[0-9a-fA-F]+|\d+", n) else None
        if size is not None:
            return size
    if "[]" in d.body.replace(" ", ""):
        return None
    key = (d.file, d.name)
    if key not in index.probed:
        index.probed[key] = probe_size(d.file, d.name)
    return index.probed[key]


def declared_size(index: "Index", name: str, sym: Symbol) -> tuple[int | None, int | None]:
    """The size a variable is declared with: a header's if one declares it,
    else the size most declarations agree on, else the size in symbols.txt.
    For an array declared with no count, (None, element size)."""
    decls = index.decls.of(name)
    for pick in (lambda d: d.file.startswith("include/"), lambda d: True):
        sizes = Counter(s for d in decls if pick(d) and (s := decl_size(index, d)) is not None)
        if sizes:
            return sizes.most_common(1)[0][0], None
    if sym.size:
        return sym.size, None
    for d in decls:
        if re.search(rf"\b{name}\s*\[\s*\]", d.body):
            key = (d.file, f"{name}[0]")
            if key not in index.probed:
                index.probed[key] = probe_size(d.file, f"{name}[0]")
            if index.probed[key]:
                return None, index.probed[key]
    return None, None


# ---------------------------------------------------------------- variables

@dataclass
class Var:
    addr: int
    size: int       # bytes it takes up to the next variable: a multiple of 4
    sort: int       # its own size, which the compiler sorts by
    name: str
    placeholder: bool = False
    declared: bool = True
    aliases: list[tuple[str, int]] = field(default_factory=list)
    note: str = ""
    align32: bool = False


@dataclass
class Filler:
    addr: int
    size: int
    before: str     # the symbol after it, for the comment


def round4(n: int) -> int:
    return (n + 3) & ~3


def build_items(syms: list[tuple[str, int, int | None, int | None]], start: int, end: int,
                prefix: str) -> tuple[list, list[tuple[str, int]]]:
    """Variables and filler stretches of one gap [start, end), from its
    symbols (name, address, declared size or None, element size of an array
    declared with no count or None) in address order. Such an array gets as
    many elements as fit before the next variable.
    Returns them with the aliases (name, address) not yet given a variable."""
    starts: list[tuple[str, int, int | None, int | None]] = []
    aliases: list[tuple[str, int]] = []
    for name, addr, size, elem in syms:
        if not start <= addr < end:
            fail(f"{name} at {addr:#x} is outside {start:#x}-{end:#x}")
        prev = starts[-1] if starts else None
        # An array with no count takes at least one element.
        if prev and (prev[2] or prev[3]) and addr < prev[1] + (prev[2] or prev[3]):
            aliases.append((name, addr))
        elif addr % 4:
            aliases.append((name, addr))
        else:
            starts.append((name, addr, size, elem))
    items: list = []
    if not starts or starts[0][1] > start:
        first = starts[0][1] if starts else end
        items.append(Filler(start, first - start, starts[0][0] if starts else "the end"))
    for i, (name, addr, size, elem) in enumerate(starts):
        nxt = starts[i + 1][1] if i + 1 < len(starts) else end
        nxt_name = starts[i + 1][0] if i + 1 < len(starts) else "the end"
        fitted = bool(not size and elem and (nxt - addr) % elem)
        if fitted:
            size = max(elem, (nxt - addr) // elem * elem)
        if size is None or size == 0:
            items.append(Var(addr, nxt - addr, nxt - addr, name, declared=False,
                             note="size: up to the next symbol"))
            continue
        if addr + size > end:
            fail(f"{name} at {addr:#x} is declared {size:#x} bytes, past the end of the range ({end:#x})")
        lay = round4(size)
        items.append(Var(addr, lay, size, name, declared=not fitted,
                         note="count: as many elements as fit before the next symbol" if fitted else ""))
        if nxt - addr > lay:
            items.append(Filler(addr + lay, nxt - addr - lay, nxt_name))
    return items, aliases


def descents(sizes: list) -> int:
    """Places where the size goes down."""
    return sum(1 for a, b in zip(sizes, sizes[1:]) if b < a)


def split_filler(size: int, s: int, t: float, aligned_next: bool) -> tuple[int, list[int] | None]:
    """How to cut `size` bytes of filler between a variable of sort size `s`
    (0 at the start) and one of size `t` (inf at the end) into pieces that
    are multiples of 4. Returns (descents, pieces) with the fewest descents
    (places where the size goes down), then the fewest pieces. pieces None
    means no filler at all: the next variable is aligned to 32 instead,
    allowed when `aligned_next` (the filler ends at a 32-byte boundary and
    is shorter than 32 bytes)."""
    lo = max(4, round4(s))
    hi = (int(t) & ~3) if t != math.inf else size

    def pieces(k: int, low: int) -> list[int]:
        out = [low] * k
        extra = size - k * low
        for i in range(k - 1, -1, -1):
            add = min(extra, hi - low)
            out[i] += add
            extra -= add
        assert extra == 0 and sum(out) == size
        return out

    candidates = [[size]]
    if hi >= 4:
        k = -(-size // hi)
        if lo <= hi and k * lo <= size:
            candidates.append(pieces(k, lo))
        candidates.append(pieces(k, 4))
    options = [(descents([s, *p, t]), len(p), p) for p in candidates]
    if aligned_next:
        options.append((descents([s, t]), 0, None))
    fewest, _, best = min(options, key=lambda o: (o[0], o[1]))
    return fewest, best


def fill(items: list, prefix: str, split: bool = True, aliases: list[tuple[str, int]] = ()) -> list[Var]:
    """Replace each filler stretch by placeholder variables, cut so as to keep
    the runs few (split_filler). With split=False, one placeholder each."""
    items = [dataclasses.replace(x, aliases=[]) if isinstance(x, Var) else x for x in items]
    out: list[Var] = []
    for i, item in enumerate(items):
        if isinstance(item, Var):
            out.append(item)
            continue
        s = out[-1].sort if out else 0
        nxt = items[i + 1] if i + 1 < len(items) else None
        t = nxt.sort if isinstance(nxt, Var) else math.inf
        aligned = (isinstance(nxt, Var) and nxt.addr % 32 == 0 and item.size < 32
                   and not any(item.addr <= a < nxt.addr for _, a in aliases))
        cut = split_filler(item.size, s, t, aligned and split)[1] if split else [item.size]
        if cut is None:
            nxt.align32 = True
            nxt.note = (nxt.note + "; " if nxt.note else "") + "aligned to 32: no symbol covers the bytes before it"
            continue
        addr = item.addr
        for j, n in enumerate(cut):
            note = ""
            if j == 0:
                note = (f"{item.size:#x} bytes no code names, before {item.before}"
                        + (f", cut in {len(cut)} to keep the sizes ascending" if len(cut) > 1 else ""))
            out.append(Var(addr, n, n, f"{prefix}{addr:08x}", placeholder=True, note=note))
            addr += n
    return out


def attach_aliases(variables: list[Var], aliases: list[tuple[str, int]]) -> None:
    for name, addr in aliases:
        owner = next((v for v in variables if v.addr <= addr < v.addr + v.size), None)
        if owner is None:
            fail(f"alias {name} at {addr:#x} is in no variable")
        owner.aliases.append((name, addr - owner.addr))


def split_runs(variables: list[Var]) -> list[list[Var]]:
    runs: list[list[Var]] = []
    for v in variables:
        if runs and v.sort >= runs[-1][-1].sort:
            runs[-1].append(v)
        else:
            runs.append([v])
    return runs


# ---------------------------------------------------------------- one module

@dataclass
class Gap:
    section: str
    start: int
    end: int
    items: list
    aliases: list[tuple[str, int]]
    variables: list[Var] = field(default_factory=list)


class Module:
    def __init__(self, name: str, index: Index) -> None:
        if name not in module_modules():
            fail(f"unknown module {name}; one of {', '.join(module_modules())}")
        self.name = name
        self.index = index
        self.cfg = ROOT / data_inc.config_dir(name)
        self.header, self.entries = read_delinks(self.cfg / "delinks.txt")
        self.symbols = read_symbols(self.cfg / "symbols.txt")
        self.prefix = symbol_prefix(name)
        self.gaps = self._gaps()

    def segment(self, addr: int) -> int | None:
        """Which part of the module an address is in. The header lists each
        part's sections together, starting with its .text: ARM7 has three
        parts that load to different places (tools/arm7.py), an ARM9 module
        one."""
        part = -1
        for sec, start, end in self.header:
            part += sec == ".text"
            if start <= addr < end or addr == start:
                return part
        return None

    def bss_ranges(self) -> list[tuple[str, int, int]]:
        return [h for h in self.header if h[0] in BSS_SECTIONS and h[2] > h[1]]

    def _gaps(self) -> list[Gap]:
        gaps = []
        for sec, start, end in self.bss_ranges():
            covered = sorted(e.sections[sec] for e in self.entries
                             if sec in e.sections and start <= e.sections[sec][0] < end)
            pos = start
            for a, b in covered + [(end, end)]:
                if a > pos:
                    gaps.append(self._gap(sec, pos, a))
                pos = max(pos, b)
        return gaps

    def _gap(self, sec: str, start: int, end: int) -> Gap:
        syms = sorted((s for s in self.symbols if start <= s.addr < end), key=lambda s: s.addr)
        rows = [(s.name, s.addr, *declared_size(self.index, s.name, s)) for s in syms]
        items, aliases = build_items(rows, start, end, self.prefix)
        return Gap(sec, start, end, items, aliases)

    def plan_variables(self, split: bool = True) -> None:
        for gap in self.gaps:
            gap.variables = fill(gap.items, self.prefix, split, gap.aliases)
            attach_aliases(gap.variables, gap.aliases)


# ---------------------------------------------------------------- users and files

def reloc_module(module: str) -> str:
    m = re.match(r"ov(\d+)$", module)
    return f"overlay({int(m.group(1))})" if m else module


class Users:
    """For each symbol of the module, the source files of the functions
    whose relocations point at it, and which of them are in the module.
    Files come in fnindex order, so that ties break the same way on every
    run (a set's order changes with Python's hash seed)."""

    def __init__(self, module: Module) -> None:
        target = reloc_module(module.name)
        self.module = module
        self.files: dict[str, set[str]] = defaultdict(set)
        self.rank: dict[str, int] = {}
        self.local_files: set[str] = {e.name for e in module.entries}
        for fn in module.index.functions:
            if not fn.get("file"):
                continue
            self.rank.setdefault(fn["file"], len(self.rank))
            for reloc in fn["relocs"]:
                if reloc[3] == target and reloc[4]:
                    self.files[reloc[4]].add(fn["file"])

    def of(self, v: Var) -> list[str]:
        names = [v.name] + [a for a, _ in v.aliases]
        return sorted(set().union(*(self.files.get(n, set()) for n in names)), key=self.rank.__getitem__)

    def local_of(self, v: Var) -> list[str]:
        return [f for f in self.of(v) if f in self.local_files]


@dataclass
class Group:
    runs: list[list[Var]]
    directory: str
    host: str | None = None
    path: str = ""
    section: str = ".bss"

    @property
    def variables(self) -> list[Var]:
        return [v for run in self.runs for v in run]

    @property
    def start(self) -> int:
        return self.runs[0][0].addr

    @property
    def end(self) -> int:
        last = self.runs[-1][-1]
        return last.addr + last.size


def host_allowed(module: Module, path: str, addr: int) -> bool:
    """Whether `path` can take a run at `addr`. Among other things its code
    must be in the same part of the module: tools/arm7.py moves each ARM7
    part on its own and wants each file in one part."""
    entry = next((e for e in module.entries if e.name == path), None)
    if entry is None or not entry.complete or not (ROOT / path).exists():
        return False
    if any(s in entry.sections for s in BSS_SECTIONS):
        return False
    if any(module.segment(start) != module.segment(addr) for start, _ in entry.sections.values()):
        return False
    # Compiled from the source as it is now, not read from build/: an
    # object left there by an earlier --write would give another answer.
    elf = compile_probe(path, (ROOT / path).read_text())
    if elf is None:
        return False
    return all(s["name"] in CODE_SECTIONS for s in elf[0] if s["flags"] & 2)  # SHF_ALLOC


def best_host(module: Module, users: Users, run: list[Var], banned: set[str]) -> str | None:
    named = [v for v in run if not v.placeholder]
    if not named:
        return None
    count = Counter(f for v in named for f in users.local_of(v))
    for path, n in sorted(count.items(), key=lambda kv: (-kv[1], users.rank[kv[0]])):
        if 2 * n <= len(named):
            return None
        if path not in banned and host_allowed(module, path, run[0].addr) and declarations_fit(module.index, path, run):
            return path
    return None


def declarations_fit(index: Index, path: str, run: list[Var]) -> bool:
    """Whether every declaration in `path` of the run's variables has the
    variable's size (or no count), so that its definition can replace it."""
    return all(decl_size(index, d) in (None, v.sort) for v in run for d in index.decls.of(v.name) if d.file == path)


def users_directory(module: Module, users: Users, run: list[Var]) -> str | None:
    """The directory where most of the run's users are, the module's own
    directory when they are spread out, None when nothing uses the run."""
    votes: Counter = Counter()
    used = 0
    for v in run:
        dirs = list(dict.fromkeys(str(Path(f).parent) for f in users.local_of(v)))
        if dirs:
            used += 1
            for d in dirs:
                votes[d] += Fraction(1, len(dirs))
    if not used:
        return None
    top, n = votes.most_common(1)[0]
    return top if 2 * n > used else module_src_dir(module.name)


def link_order_ok(entries: list[Entry]) -> bool:
    """dsd's check: the entries sorted by start address in each section must
    give one order, with no cycle."""
    graph: dict[int, set[int]] = defaultdict(set)
    indeg = [0] * len(entries)
    by_section: dict[str, list[tuple[int, int]]] = defaultdict(list)
    for i, e in enumerate(entries):
        for sec, (start, _) in e.sections.items():
            by_section[sec].append((start, i))
    for rows in by_section.values():
        rows.sort()
        for (_, a), (_, b) in zip(rows, rows[1:]):
            if b not in graph[a]:
                graph[a].add(b)
                indeg[b] += 1
    ready = [i for i in range(len(entries)) if indeg[i] == 0]
    seen = 0
    while ready:
        i = ready.pop()
        seen += 1
        for j in graph[i]:
            indeg[j] -= 1
            if indeg[j] == 0:
                ready.append(j)
    return seen == len(entries)


def group_runs(module: Module, users: Users, runs: list[list[Var]], section: str,
               relocs: list) -> list[Group]:
    """Neighbouring runs that go to the same host, or the same directory,
    share a file. A host that would break the link order, or whose C does
    not compile with the definitions (group_text), is dropped."""
    banned: set[str] = set()
    while True:
        groups: list[Group] = []
        for run in runs:
            host = best_host(module, users, run, banned)
            directory = (str(Path(host).parent) if host else users_directory(module, users, run))
            g = groups[-1] if groups else None
            joins = g is not None and len(g.runs) < MAX_RUNS[section] and (
                (host is not None and host == g.host)
                or (host is None and directory in (None, g.directory))
                or (host is not None and g.host is None and g.directory == directory))
            if joins:
                g.runs.append(run)
                g.host = g.host or host
            else:
                groups.append(Group([run], directory or module_src_dir(module.name), host, section=section))
        bad = None
        entries = [Entry(e.name, e.complete, dict(e.sections)) for e in module.entries]
        by_name = {e.name: e for e in entries}
        for g in groups:
            if g.host:
                by_name[g.host].sections[section] = (g.start, g.end)
                g.path = g.host
                if not link_order_ok(entries) or render(module, g, relocs)[0] is None:
                    bad = g.host
                    break
        if bad is None:
            return groups
        banned.add(bad)


def all_basenames() -> set[str]:
    names = set()
    for cfg in (data_inc.CONFIG9, data_inc.CONFIG7):
        for delinks in (ROOT / cfg).rglob("delinks.txt"):
            for line in delinks.read_text().splitlines():
                if line and not line[0].isspace() and line.rstrip().endswith(":"):
                    names.add(Path(line.rstrip()[:-1]).stem)
    return names


def name_groups(module: Module, groups: list[Group]) -> None:
    taken = all_basenames()
    lead = "" if module.name == "main" else f"{module.name}_"
    for g in groups:
        if g.host:
            g.path = g.host
            continue
        topic = Path(g.directory).name
        stem = f"{topic}_bss" if g.directory != module_src_dir(module.name) else f"{lead}bss_{g.start:08x}"
        if stem in taken:
            stem = f"{topic}_bss_{g.start:08x}"
        taken.add(stem)
        g.path = f"{g.directory}/{stem}.c"


# ---------------------------------------------------------------- planning

@dataclass
class Plan:
    module: Module
    groups: list[Group]
    runs_before: int
    runs_after: int


def make_plan(module: Module, lo: int | None = None, hi: int | None = None) -> Plan:
    module.plan_variables(split=False)
    runs_before = sum(len(split_runs(g.variables)) for g in module.gaps if g.variables)
    module.plan_variables(split=True)
    users = Users(module)
    relocs = read_relocs()
    groups: list[Group] = []
    runs_after = 0
    for gap in module.gaps:
        variables = [v for v in gap.variables
                     if (lo is None or v.addr >= lo) and (hi is None or v.addr < hi)]
        if not variables:
            continue
        runs = split_runs(variables)
        runs_after += len(runs)
        groups += group_runs(module, users, runs, gap.section, relocs)
    name_groups(module, groups)
    return Plan(module, groups, runs_before, runs_after)


def parse_range(text: str) -> tuple[int, int]:
    m = re.fullmatch(r"(0x[0-9a-fA-F]+|[0-9a-fA-F]{8})-(0x[0-9a-fA-F]+|[0-9a-fA-F]{8})", text)
    if not m:
        fail(f"--range {text}: expected LO-HI, for example 0x0216e020-0x0216f000")
    return int(m.group(1), 16), int(m.group(2), 16)


# ---------------------------------------------------------------- the C

def run_section(section: str, k: int) -> str:
    """The section run k of a file's `section` (.bss or .sbss) goes in."""
    return section if k == 0 else f"{section}.run{k}"


def run_pragma(section: str, k: int) -> str | None:
    """The include/bss_run.h section that puts run k in run_section; None
    for the first run of .bss, which the compiler puts there itself."""
    if section == ".bss" and k == 0:
        return None
    return section.lstrip(".").upper() + (f"_RUN{k}" if k else "")


def chosen_decl(index: Index, v: Var, host: str | None) -> Decl | None:
    """The declaration a definition copies: the host file's own, else a
    header's, else the most common one in a .c file. Outside the host, only
    a declaration of the variable's size counts (or an array with no count)."""
    decls = index.decls.of(v.name)
    own = [d for d in decls if d.file == host]
    if own:
        return own[0]
    fits = [d for d in decls if decl_size(index, d) in (None, v.sort)]
    headers = [d for d in fits if d.file.startswith("include/")]
    if headers:
        return headers[0]
    bodies = Counter(d.body for d in fits)
    return next((d for d in fits if d.body == bodies.most_common(1)[0][0]), None) if fits else None


def needed_headers(index: Index, decl: Decl) -> set[str] | None:
    """Headers that define the type names in a declaration; None when one of
    them is defined in no header."""
    out = set()
    for word in set(re.findall(r"\b[A-Za-z_]\w*\b", decl.body)):
        if word in BASIC_SIZES or word in C_WORDS:
            continue
        if word.isupper() or word[0].isupper() and "_" in word and word.upper() == word:
            header = pick_header(index.decls.type_headers.get(word, []), decl.file)
            if header is None:
                return None
            out.add(header)
            continue
        header = index.type_header(word, decl.file)
        if header:
            out.add(header)
        elif header is None and (word in index.struct_size or word in index.struct_headers or re.search(rf"\b{word}\b\s*[*\w(]", decl.body)):
            return None
    return out


C_WORDS = {"const", "volatile", "struct", "union", "enum", "void", "signed", "unsigned", "static"}


def definition(index: Index, v: Var, decl: Decl | None, host: str | None, static: bool,
               plain: bool = False, with_lead: bool = False) -> tuple[str, set[str]]:
    """The definition of one variable and the headers it needs: the
    declaration it copies without `extern`, an array with no count given
    one, or a `u8` array when there is no declaration (or `plain`). With
    `with_lead`, the comment above the host's own declaration goes above
    the definition, since the declaration and the comment are taken out."""
    notes = [v.note] if v.note else []
    attr = " __attribute__((aligned(32)))" if v.align32 else ""
    lead = "static " if static else ""
    body, headers = f"u8 {v.name}[{v.sort:#x}]", set()
    if decl is not None:
        if decl.comment:
            notes.insert(0, decl.comment)
        headers = ({decl.file.removeprefix("include/")} if decl.file.startswith("include/")
                   else set() if decl.file == host else needed_headers(index, decl))
        if plain or headers is None:
            notes.append(f"declared in {decl.file}, with a type no header defines")
            headers = set()
        else:
            body = decl.body
            if "*" not in body.split(v.name)[0]:
                body = re.sub(r"^const\s+", "", body)
            if re.search(rf"\b{v.name}\s*\[\s*\]", body):
                elem = probe_size(decl.file, f"{v.name}[0]")
                if elem and v.sort % elem == 0:
                    body = re.sub(rf"\b{v.name}\s*\[\s*\]", f"{v.name}[{v.sort // elem:#x}]", body, count=1)
                else:
                    notes.append(f"declared as {decl.body}, but its elements do not fill {v.sort:#x} bytes")
                    body, headers = f"u8 {v.name}[{v.sort:#x}]", set()
        others = sorted({d.file for d in index.decls.of(v.name)} - {host, decl.file})
        if others and decl.file == host and not any(f.startswith("include/") for f in others):
            notes.append(f"also declared in {others[0]}" + (f" and {len(others) - 1} more" if len(others) > 1 else ""))
    code = f"{lead}{body}{attr};"
    above = decl.lead + "\n" if with_lead and decl is not None and decl.file == host and decl.lead else ""
    if not notes:
        return above + code, headers
    note = f"/* {'; '.join(notes)} */"
    if len(code) + 1 + len(note) <= 80:
        return f"{above}{code} {note}", headers
    return f"{above}{note}\n{code}", headers


def body_lines(index: Index, g: Group, static: set[str], plain: set[str] = frozenset(),
               with_lead: bool = False) -> tuple[list[str], set[str]]:
    """The definitions of a group in the order that makes the compiler emit
    each run in address order, with the run's section pragma around the
    ones past the first run. Returns them and the headers they need.
    Variables in `plain` are written as `u8` arrays. `with_lead`: see
    definition."""
    tagged = [(v.sort, k, i, v) for k, run in enumerate(g.runs) for i, v in enumerate(run)]
    tagged.sort(key=lambda r: r[:3])
    order = definition_order([r[0] for r in tagged], initialized=False)
    lines: list[str] = []
    headers: set[str] = set()
    open_pragma = None
    for idx in order:
        _, k, _, v = tagged[idx]
        pragma = run_pragma(g.section, k)
        if pragma != open_pragma:
            if open_pragma:
                lines.append(f"#pragma section {open_pragma} end")
            if pragma:
                lines.append(f"#pragma section {pragma} begin")
            open_pragma = pragma
        text, need = definition(index, v, chosen_decl(index, v, g.host), g.host, v.name in static,
                                v.name in plain, with_lead)
        lines.append(text)
        headers |= need
    if open_pragma:
        lines.append(f"#pragma section {open_pragma} end")
    return lines, headers


def expected_sections(g: Group) -> dict[str, list[str]]:
    return {run_section(g.section, k): [v.name for v in run] for k, run in enumerate(g.runs)}


def read_relocs() -> list[tuple[str, int, int, str]]:
    """Every relocation: (module of the code or data it is in, from, to,
    module it points into)."""
    out = []
    for module in module_modules() + ["itcm", "dtcm"]:
        path = ROOT / data_inc.config_dir(module) / "relocs.txt"
        if not path.exists():
            continue
        for line in path.read_text().splitlines():
            m = re.match(r"from:(0x\w+) kind:\S+ to:(0x\w+)(?: add:\S+)? module:(\S+)", line)
            if m:
                out.append((module, int(m.group(1), 16), int(m.group(2), 16), data_inc.module_key(m.group(3))))
    return out


def static_names(module: Module, g: Group, relocs: list) -> set[str]:
    """Variables of a host file that only the file names: no other file
    declares them and every relocation to them is in the file's code."""
    if not g.host:
        return set()
    entry = next(e for e in module.entries if e.name == g.host)
    out = set()
    for v in g.variables:
        if v.placeholder or v.aliases:
            continue
        files = {d.file for d in module.index.decls.of(v.name)}
        if files - {g.host}:
            continue
        refs = [r for r in relocs if r[3] == module.name and v.addr <= r[2] < v.addr + v.size]
        if refs and all(r[0] == module.name and any(a <= r[1] < b for a, b in entry.sections.values())
                        for r in refs):
            out.add(v.name)
    return out


def new_file_text(lines: list[str], headers: set[str], run_header: bool) -> str:
    includes = ['#include "types.h"'] + [f'#include "{h}"' for h in sorted(headers)]
    if run_header:
        includes.append(f'#include "{RUN_HEADER}"')
    return "\n".join(includes) + "\n\n" + "\n".join(lines) + "\n"


def host_file_text(index: Index, g: Group, lines: list[str], headers: set[str], run_header: bool,
                   in_place: bool = True) -> str:
    """The host file with its `extern` statements for the group's variables
    taken out, with the comment just above each, and the definitions put
    where the first one was (after the last #include when it has none).
    `lines` must then carry those comments (body_lines with_lead). With
    in_place False, the `extern` statements stay and the definitions go at
    the end of the file."""
    text = (ROOT / g.host).read_text()
    spans = []
    for v in g.variables if in_place else []:
        for d in index.decls.of(v.name):
            if d.file == g.host:
                end = text.find("\n", d.end)
                spans.append((d.lead_start, len(text) if end < 0 else end + 1))
    spans.sort()
    includes = [m.end() for m in re.finditer(r"^#include[^\n]*\n", text, re.M)]
    include_end = includes[-1] if includes else 0
    new_includes = "".join(f'#include "{h}"\n' for h in sorted(headers) if f'#include "{h}"' not in text)
    if run_header and f'#include "{RUN_HEADER}"' not in text:
        new_includes += f'#include "{RUN_HEADER}"\n'
    block = "\n".join(lines) + "\n"
    # (position, order at one position, characters to drop, text to put in)
    edits = [(start, 2, end - start, "") for start, end in spans]
    if not in_place:
        edits.append((len(text), 1, 0, "\n" + block))
    else:
        edits.append((spans[0][0], 1, 0, block) if spans else (include_end, 1, 0, "\n" + block))
    edits.append((include_end, 0, 0, new_includes))
    for pos, _, drop, insert in sorted(edits, reverse=True):
        text = text[:pos] + insert + text[pos + drop:]
    return text


# ---------------------------------------------------------------- the config

def section_line(name: str, start: int, end: int) -> str:
    return f"    {name:<12}start:{start:#010x} end:{end:#010x}"


def edit_delinks(path: Path, groups: list[Group]) -> None:
    lines = path.read_text().split("\n")
    for g in groups:
        sec = section_line(g.section, g.start, g.end)
        if g.host:
            i = lines.index(f"{g.host}:")
            j = i + 1
            while j < len(lines) and lines[j].strip():
                j += 1
            lines.insert(j, sec)
            continue
        # After the last entry whose .bss or .sbss comes before this one, else at the end.
        block = ["", f"{g.path}:", "    complete", sec]
        at, best = None, -1
        cur_start = None
        for j, line in enumerate(lines):
            m = SECTION_LINE.match(line)
            if line and not line[0].isspace():
                cur_start = j
            elif m and m.group(1) in BSS_SECTIONS and cur_start is not None:
                start = int(m.group(2), 16)
                if best < start < g.start:
                    best = start
                    k = j
                    while k < len(lines) and lines[k].strip():
                        k += 1
                    at = k
        if at is None:
            while lines and not lines[-1].strip():
                lines.pop()
            lines += block[:] + [""]
        else:
            lines[at:at] = block
    path.write_text("\n".join(lines))


def edit_symbols(path: Path, variables: list[Var]) -> None:
    """Gives each variable its size, and each alias size 0, and adds the
    placeholders in address order. objdiff scores a unit's .bss 0% matched
    when an alias inside a variable has a size (docs/bss-tool.md, question 4)."""
    lines = path.read_text().split("\n")
    where = {}
    for i, line in enumerate(lines):
        m = SYMBOL_LINE.match(line)
        if m:
            where[m.group(1)] = (i, int(m.group(4), 16))
    added = []
    sized = [(v, v.name, v.sort) for v in variables]
    sized += [(None, name, 0) for v in variables for name, _ in v.aliases]
    for v, name, n in sized:
        size = f"kind:bss(size={n:#x})"
        if name in where:
            i = where[name][0]
            lines[i] = re.sub(r"kind:bss(\([^)]*\))?", size, lines[i], count=1)
        elif v is None:
            fail(f"alias {name} is not in {path}")
        else:
            added.append((v.addr, f"{v.name} {size} addr:{v.addr:#010x}"))
    rows = sorted(where.values())
    for addr, line in sorted(added, reverse=True):
        at = next((i for i, a in rows if a > addr), len(lines) - (lines[-1] == ""))
        lines.insert(at, line)
    path.write_text("\n".join(lines))


# ---------------------------------------------------------------- commands

def describe(v: Var) -> str:
    what = "placeholder" if v.placeholder else ("declared" if v.declared else "size from gap")
    extra = "".join(f", alias {n} +{o:#x}" for n, o in v.aliases)
    return f"    {v.addr:#010x} {v.size:#8x} {v.name} ({what}{extra}{', align 32' if v.align32 else ''})"


def summary(plan: Plan) -> str:
    variables = [v for g in plan.groups for v in g.variables]
    hosted = sum(1 for g in plan.groups if g.host)
    return (f"{plan.module.name}: {len(variables)} variables "
            f"({sum(not v.placeholder for v in variables)} named, {sum(v.placeholder for v in variables)} placeholders,"
            f" {sum(len(v.aliases) for v in variables)} aliases, {sum(v.align32 for v in variables)} aligned to 32), "
            f"{sum(v.size for v in variables):#x} bytes; runs: {plan.runs_before} before cutting filler,"
            f" {plan.runs_after} after; files: {len(plan.groups)} ({hosted} in existing files,"
            f" {len(plan.groups) - hosted} new)")


def cmd_scan(args) -> int:
    module = Module(args.module, Index())
    plan = make_plan(module)
    for gap in module.gaps:
        print(f"{gap.section} gap {gap.start:#010x}-{gap.end:#010x} ({gap.end - gap.start:#x} bytes)")
        if args.verbose:
            for k, run in enumerate(split_runs(gap.variables)):
                print(f"  run {k}: {len(run)} variables")
                for v in run:
                    print(describe(v))
    print(summary(plan))
    return 0


def print_plan(plan: Plan) -> None:
    for g in plan.groups:
        kind = "existing" if g.host else "new"
        print(f"{g.path} ({kind}): {g.start:#010x}-{g.end:#010x}, {len(g.runs)} run(s), "
              f"{len(g.variables)} variables")
    print(summary(plan))


def order_errors(elf, g: Group) -> list[str]:
    """What is wrong with a group's object: a run out of order in its
    section, or a variable of another size than the ROM gives it (a type
    from the wrong header)."""
    got = emitted_order(elf)
    errors = []
    for sec, names in expected_sections(g).items():
        ours = [n for n in got.get(sec, []) if n in set(names)]
        if ours != names:
            errors.append(f"the compiler emits {sec} as {ours}, expected {names}")
    sizes = {s["name"]: s["size"] for s in elf[1] if s["type"] == 1}
    for v in g.variables:
        if v.name in sizes and sizes[v.name] != v.sort:
            errors.append(f"{v.name} compiles to {sizes[v.name]:#x} bytes, expected {v.sort:#x}")
    return errors


def render(module: Module, g: Group, relocs: list) -> tuple[str | None, list[str]]:
    """The C for a group, checked with the compiler: it compiles, and each
    run comes out in its section in address order. A host file first gets
    its `extern` statements replaced; when that does not compile (a type
    defined below them), the definitions go at the end of the file instead.
    A new file first copies each declaration; when that does not compile,
    variables declared only in .c files become `u8` arrays."""
    index = module.index
    for v in g.variables:
        d = chosen_decl(index, v, g.host)
        if d is not None and d.file == g.host and decl_size(index, d) not in (None, v.sort):
            return None, [f"declares {d.body} ({decl_size(index, d):#x} bytes), but {v.name} takes {v.sort:#x}"]
    static = static_names(module, g, relocs)
    run_header = any(run_pragma(g.section, k) for k in range(len(g.runs)))
    attempts = []
    lines, headers = body_lines(index, g, static)
    if g.host:
        attempts.append(host_file_text(index, g, body_lines(index, g, static, with_lead=True)[0], headers,
                                       run_header))
        attempts.append(host_file_text(index, g, lines, headers, run_header, in_place=False))
    else:
        attempts.append(new_file_text(lines, headers, run_header))
        plain = {v.name for v in g.variables
                 if (d := chosen_decl(index, v, None)) and not d.file.startswith("include/")}
        attempts.append(new_file_text(*body_lines(index, g, static, plain), run_header))
    errors = []
    for text in attempts:
        elf = compile_probe(g.path, text)
        if elf is None:
            errors.append("does not compile: " + compile_errors.get((g.path, text), ""))
            continue
        errors += order_errors(elf, g)
        if not order_errors(elf, g):
            return text, []
    return None, errors


def group_text(module: Module, g: Group, relocs: list) -> str:
    text, errors = render(module, g, relocs)
    if text is None:
        fail(f"{g.path}: " + "; ".join(errors))
    return text


def compile_check(path: str, g: Group) -> None:
    """Compiles a written file and checks the order again."""
    elf = compile_probe(path, (ROOT / path).read_text())
    if elf is None:
        fail(f"{path} does not compile")
    errors = order_errors(elf, g)
    if errors:
        fail(f"{path}: " + "; ".join(errors))


def cmd_plan(args) -> int:
    lo, hi = parse_range(args.range) if args.range else (None, None)
    module = Module(args.module, Index())
    plan = make_plan(module, lo, hi)
    print_plan(plan)
    if args.show:
        relocs = read_relocs()
        for g in plan.groups:
            print(f"\n--- {g.path}: .bss start:{g.start:#010x} end:{g.end:#010x}")
            print(group_text(module, g, relocs), end="")
    if not args.write or not plan.groups:
        if args.write:
            print("nothing to write")
        return 0
    relocs = read_relocs()
    by_path: dict[str, list[Group]] = defaultdict(list)
    for g in plan.groups:
        by_path[g.path].append(g)
    texts = {}
    for path, groups in by_path.items():
        if len(groups) > 1:
            fail(f"{path} would get {len(groups)} separate .bss ranges")
        texts[path] = group_text(module, groups[0], relocs)
    for path, text in texts.items():
        out = ROOT / path
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(text)
    subprocess.run([sys.executable, "tools/format.py", *by_path], cwd=ROOT, check=True)
    for path, groups in by_path.items():
        compile_check(path, groups[0])
    edit_delinks(module.cfg / "delinks.txt", plan.groups)
    edit_symbols(module.cfg / "symbols.txt", [v for g in plan.groups for v in g.variables])
    for path in by_path:
        print(f"wrote {path}")
    build = subprocess.run(["make", "--no-print-directory"], cwd=ROOT, capture_output=True, text=True)
    if build.returncode:
        print(build.stdout[-4000:] + build.stderr[-4000:])
        fail("the build failed")
    check = subprocess.run(["make", "--no-print-directory", "check"], cwd=ROOT, capture_output=True, text=True)
    print(check.stdout + check.stderr, end="")
    if check.returncode:
        bad = [line for line in (check.stdout + check.stderr).splitlines() if "FAILED" in line or "failed" in line]
        fail("the ROM does not match: " + ("; ".join(bad) or "see above"))
    return 0


# ---------------------------------------------------------------- linker script

def lcf_insert_after(text: str, anchor: str, lines: list[str], where: str) -> str:
    """Puts `lines` after the line `anchor` of a linker script. A script
    that has them there already is returned unchanged."""
    anchor_line = f"        {anchor}\n"
    if text.count(anchor_line) != 1:
        fail(f"{anchor} is not in {where} exactly once")
    block = "".join(f"        {line}\n" for line in lines)
    if anchor_line + block in text:
        return text
    return text.replace(anchor_line, anchor_line + block)


def lcf_alias_lines(aliases: dict[str, tuple[str, int]]) -> list[str]:
    """`NAME = BASE + OFFSET;` for symbols inside an object that C cannot
    define. The linker takes BASE as 0 above the line that places it."""
    return [f"{name} = {base} + {offset:#x};" for name, (base, offset) in sorted(aliases.items())]


def lcf_object_lines(entry: Entry, section: str, symbols: list[Symbol]) -> tuple[str, list[str]]:
    """What a file with a .bss or .sbss range (`section`) needs in the
    linker script: its runs past the first, after that section, then its
    aliases."""
    stem = Path(entry.name).stem
    start, end = entry.sections[section]
    inside = sorted((s for s in symbols if start <= s.addr < end), key=lambda s: s.addr)
    variables = [Var(s.addr, round4(s.size), s.size, s.name) for s in inside if s.size]
    if not variables:
        return f"{stem}.o({section})", []
    aliases = {}
    for s in inside:
        if s.size:
            continue
        owner = next((v for v in variables if v.addr <= s.addr < v.addr + v.size), None)
        if owner is None:
            fail(f"{s.name} at {s.addr:#x} has no size and is inside no variable of {entry.name}")
        aliases[s.name] = (owner.name, s.addr - owner.addr)
    runs = split_runs(variables)
    lines = [f"{stem}.o({run_section(section, k)})" for k in range(1, len(runs))]
    return f"{stem}.o({section})", lines + lcf_alias_lines(aliases)


def cmd_lcf(args) -> int:
    """For each file with a .bss or .sbss range that holds sized symbols:
    place its sections `.bss.runN` (`.sbss.runN`) after its .bss (.sbss),
    and define its aliases. The ARM9 modules go in build/arm9.lcf, arm7 in
    build/arm7/arm7.lcf (--arm7). Every line names objects and symbols, no
    address, so ARM7's script works whether its parts sit at their real
    addresses or where tools/arm7.py moves them."""
    path = Path(args.lcf)
    text = path.read_text()
    modules = ["arm7"] if args.arm7 else [m for m in module_modules() if m != "arm7"] + ["itcm", "dtcm"]
    for module in modules:
        cfg = ROOT / data_inc.config_dir(module)
        _, entries = read_delinks(cfg / "delinks.txt")
        symbols = None
        for entry in entries:
            if not entry.complete:
                continue
            for section in BSS_SECTIONS:
                if section not in entry.sections:
                    continue
                symbols = symbols if symbols is not None else read_symbols(cfg / "symbols.txt")
                anchor, lines = lcf_object_lines(entry, section, symbols)
                if lines:
                    text = lcf_insert_after(text, anchor, lines, str(path))
    path.write_text(text)
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = parser.add_subparsers(required=True)
    p = sub.add_parser("scan", help="list variables, aliases, filler and runs")
    p.add_argument("module")
    p.add_argument("--verbose", action="store_true")
    p.set_defaults(func=cmd_scan)
    p = sub.add_parser("plan", help="the files it would write; --write writes them")
    p.add_argument("module")
    p.add_argument("--range", help="LO-HI: only the variables that start in it")
    p.add_argument("--show", action="store_true", help="print the C of each file (unformatted)")
    p.add_argument("--write", action="store_true")
    p.set_defaults(func=cmd_plan)
    p = sub.add_parser("lcf", help="place .bss.runN sections and aliases in a linker script")
    p.add_argument("lcf")
    p.add_argument("--arm7", action="store_true", help="ARM7's script: the arm7 module only")
    p.set_defaults(func=cmd_lcf)
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
