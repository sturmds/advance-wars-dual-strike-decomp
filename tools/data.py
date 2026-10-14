#!/usr/bin/env python3
"""Define a module's .data and .rodata in C, so that they link from files
marked `complete` (issue 0082, docs/data-tool.md).

    data.py scan MODULE [--verbose]                 objects, aliases, filler and runs; writes nothing
    data.py plan MODULE [--range LO-HI] [--show]    the files it would write, and where (--show: their C)
    data.py plan MODULE [--range LO-HI] --write     write them, build, and check the ROM and the report
    data.py relocs MODULE                           relocations in the module's data to check by hand

MODULE: main, dtcm, ovNNN or arm7. The counterpart of tools/bss.py, whose
rules it follows and whose code it reuses; what differs:

- Values: the bytes are game data and never enter git (tenet 9). Each
  object's initializer includes `data/<module>/<symbol>.inc.c`, which
  tools/data_inc.py writes from the builder's ROM, from a row that `plan
  --write` adds to config/AWRE/data_inc.txt. An object whose words are all
  pointers (relocations) or zero is written out instead, `{&a, &b, 0}`:
  that is names, not game data.
- Objects: one starts at each 4-aligned dsd symbol that is not inside the
  declared size of the one before (bss.build_items); every other symbol
  is an alias, defined in the linker script (`bss.py lcf`) and listed in
  config/AWRE/aliases.txt. An object's own size, which the compiler sorts
  by: the declared size; else, for a string (3 or more printable
  characters, a zero, then less than 4 zero bytes up to the next symbol),
  its length and the zero; else the size in symbols.txt, else the bytes up
  to the next symbol. The bytes from there up to the next multiple of 4
  must be zero (the linker's ALIGNALL(4) pad). symbols.txt gets each
  object's size as `kind:data(byte[N])` and each alias `byte[0]`.
- Filler: bytes no symbol covers become placeholders named by address, cut
  to keep runs few (bss.split_filler). Less than 32 zero bytes before a
  32-aligned object can be `__attribute__((aligned(32)))` on it instead.
- The type: the declaration's (bss.chosen_decl: the host's own, else a
  header's, else the most common one whose type a header defines). A
  struct's layout comes from build/dsindex.json, written as data_inc
  field types with `pad[N]` for the gaps, a union as its first member;
  bit fields, floats and unions with no name inside a struct stop it. With no usable declaration: `char` for a string, `void*` for
  pointers, `u32` when the size is a multiple of 4, `u16` of 2, else
  `u8`. A relocation must sit on a 4-byte field.
- const: an object in .rodata is defined `const` (a pointer array as
  `T* const`), one in .data never is: mwcc sends const objects to .rodata.
- Zero: mwcc puts an object whose initializer is all zero in .bss. The
  definitions of a block with such an object go between
  `#pragma explicit_zero_data on` and `reset`, which keeps them in .data.
- Order: mwcc sorts a file's initialised objects by size, starting from
  the reverse of the definition order (tools/mwcc_order.py), so a file
  holds a contiguous slice of one ascending run, as in .bss. A file whose
  object already has data sections (.data, .rodata) takes no slice; one
  with .bss may, if its .bss still comes out in the same order.
- Names the initializers point at are declared: a header that declares
  them, else `extern u8 NAME[];` or `void NAME(void);` before the
  definitions. An object of the same file that is used before its
  definition gets an `extern` copy of its own declaration first.

Slicing, hosts and file names follow bss.py (decision a in
docs/bss-tool.md), with `_data` and `_rodata` in place of `_bss`. Like
bss.py it needs build/fnindex.json and build/dsindex.json (`make fnindex
dsindex`) and the compiler, and the same inputs give the same output.
"""

import argparse
import dataclasses
import json
import math
import re
import shutil
import subprocess
import sys
import tempfile
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import bss  # noqa: E402
import configure  # noqa: E402
import data_inc  # noqa: E402
from bss import Entry, Group, Var  # noqa: E402
from mwcc_order import definition_order  # noqa: E402

ROOT = bss.ROOT
DATA_SECTIONS = bss.DATA_SECTIONS
REPORT = ROOT / "build/artifacts/AWRE_report/report.json"
ZERO_ON = "#pragma explicit_zero_data on"
ZERO_OFF = "#pragma explicit_zero_data reset"
FLOATS = {"float", "double", "f32", "f64"}


def fail(msg: str) -> None:
    sys.exit(f"data.py: error: {msg}")


class Refused(Exception):
    """An object the tool cannot write as asked; the message says why."""


def data_modules() -> list[str]:
    return [m for m in bss.module_modules() if m != "arm7"] + ["dtcm", "arm7"]


# ---------------------------------------------------------------- bytes and strings

def string_size(raw: bytes) -> int | None:
    """The size of a string object at the start of `raw` (the bytes up to
    the next symbol): 3 or more printable characters and a zero, followed
    by fewer than 4 zero bytes (the linker's pad). None otherwise."""
    end = raw.find(b"\0")
    if end < 3 or any(b < 0x20 or b > 0x7e for b in raw[:end]):
        return None
    rest = raw[end + 1:]
    if len(rest) >= 4 or any(rest):
        return None
    return end + 1


# ---------------------------------------------------------------- one module

class DataModule(bss.Module):
    """bss.Module over the .data and .rodata ranges, with the ROM's bytes."""

    def __init__(self, name: str, index: bss.Index, rom: data_inc.Rom | None = None) -> None:
        if name not in data_modules():
            fail(f"unknown module {name}; one of {', '.join(data_modules())}")
        self.name = name
        self.index = index
        self.rom = rom or data_inc.Rom()
        self.cfg = ROOT / data_inc.config_dir(name)
        self.header, self.entries = bss.read_delinks(self.cfg / "delinks.txt")
        self.symbols = bss.read_symbols(self.cfg / "symbols.txt")
        self.prefix = bss.symbol_prefix(name)
        self.errors: list[str] = []
        self.larger: dict[str, int] = {}  # objects whose declarations are too small: the declared size
        self.gaps = self._gaps()

    def read(self, addr: int, size: int) -> bytes:
        return self.rom.read(self.name, addr, size)

    def _gaps(self) -> list[bss.Gap]:
        gaps = []
        for sec, start, end in [h for h in self.header if h[0] in DATA_SECTIONS and h[2] > h[1]]:
            covered = sorted(e.sections[sec] for e in self.entries
                             if sec in e.sections and start <= e.sections[sec][0] < end)
            pos = start
            for a, b in covered + [(end, end)]:
                if a > pos:
                    gaps.append(self._gap(sec, pos, a))
                pos = max(pos, b)
        return gaps

    def _gap(self, sec: str, start: int, end: int) -> bss.Gap:
        syms = sorted((s for s in self.symbols if start <= s.addr < end), key=lambda s: s.addr)
        rows = []
        for i, s in enumerate(syms):
            nxt = syms[i + 1].addr if i + 1 < len(syms) else end
            size, elem = bss.declared_size(self.index, s.name, s)
            if size and s.addr % 4 == 0 and size < min(nxt, end) - s.addr and any(
                    self.read(s.addr + size, round4(size) - size)):
                # The pad after the declared size is not zero: the object is
                # larger than its declarations say.
                self.larger[s.name] = size
                size, elem = None, None
            if size is None and elem in (None, 1) and s.addr % 4 == 0:
                size = string_size(self.read(s.addr, nxt - s.addr)) or size
            rows.append((s.name, s.addr, size, elem))
        items, aliases, past = bss.build_items(rows, start, end, self.prefix)
        for v in items:
            if isinstance(v, Var) and v.name in self.larger:
                v.note = f"declared {self.larger[v.name]:#x} bytes, but the bytes after that are not zero"
        return bss.Gap(sec, start, end, items, aliases, past)

    def plan_variables(self, split: bool = True) -> None:
        for gap in self.gaps:
            gap.variables = fill(self, gap.items, split, gap.aliases)
            bss.attach_aliases(gap.variables, gap.aliases)


def fill(module: DataModule, items: list, split: bool, aliases: list[tuple[str, int]]) -> list[Var]:
    """bss.fill with bytes: filler becomes placeholders, cut to keep the runs
    few, and an alignment to 32 replaces it only when its bytes are zero."""
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
                   and not any(item.addr <= a < nxt.addr for _, a in aliases)
                   and not any(module.read(item.addr, item.size)))
        cut = bss.split_filler(item.size, s, t, aligned and split)[1] if split else [item.size]
        if cut is None:
            nxt.align32 = True
            nxt.note = (nxt.note + "; " if nxt.note else "") + "aligned to 32: zero bytes no symbol covers before it"
            continue
        addr = item.addr
        for j, n in enumerate(cut):
            note = ""
            if j == 0:
                note = (f"{item.size:#x} bytes no code names, before {item.before}"
                        + (f", cut in {len(cut)} to keep the sizes ascending" if len(cut) > 1 else ""))
            out.append(Var(addr, n, n, f"{module.prefix}{addr:08x}", placeholder=True, note=note))
            addr += n
    return out


# ---------------------------------------------------------------- the type of an object

@dataclass
class Shape:
    """How one object is written."""
    base: str                   # element type, as declared (`GfxLayer`, `u16*`, `char`)
    dims: list[str]             # array dimensions; [] for a scalar
    row: str | None = None      # data_inc element type, or None for a written-out initializer
    values: list[str] | None = None  # the written-out initializer
    headers: set[str] = field(default_factory=set)
    notes: list[str] = field(default_factory=list)
    function_pointer: str = ""  # the declarator of a function pointer array (`(*NAME[3])(void)`)

    def declarator(self, name: str) -> str:
        dims = "".join(f"[{d}]" for d in self.dims)
        if self.function_pointer:
            return self.function_pointer.replace("NAME", name).replace("DIMS", dims)
        return f"{self.base} {name}{dims}"

    def forward(self, name: str, section: str) -> str:
        """An `extern` declaration that agrees with the definition, which
        is const in .rodata only (const_base)."""
        dims = "".join(f"[{d}]" for d in ([""] + self.dims[1:] if self.dims else []))
        if self.function_pointer:
            return "extern " + self.function_pointer.replace("NAME", name).replace("DIMS", dims) + ";"
        return f"extern {const_base(self.base, section)} {name}{dims};"


BASIC_CAST = {(1, False): "u8", (1, True): "s8", (2, False): "u16", (2, True): "s16",
              (4, False): "u32", (4, True): "s32"}
SIGNED_NAMES = {"s8", "s16", "s32", "signed char", "short", "int", "long", "fx16", "fx32", "BOOL"}


def basic_cast(index: bss.Index, base: str) -> str | None:
    """The data_inc element type for a basic type, an enum or a pointer."""
    t = index.base_type(base)
    if "*" in t:
        return "void*"
    if t in FLOATS:
        return None
    size = bss.BASIC_SIZES.get(t) or index.type_size(t)
    if t not in bss.BASIC_SIZES and t in index.struct_size:
        return None
    if size not in (1, 2, 4):
        return None
    return BASIC_CAST[(size, t in SIGNED_NAMES)]


def struct_leaves(index: bss.Index, name: str, at: int = 0) -> list[tuple[int, str, int]]:
    """The fields of a struct as data_inc leaves: (offset, type, count; 0
    for a single value), nested structs taken apart. Raises Refused for a
    bit field, a float or an unknown member; a union gives the fields of its
    first member, a union inside a struct with no name of its own is
    refused."""
    if name not in index.struct_fields or name not in index.struct_size:
        raise Refused(f"no single layout for struct {name} in build/dsindex.json")
    fields = index.struct_fields[name]
    if name in index.unions:
        # An initializer sets a union's first member. Every member starts
        # at 0, and the fields of an unnamed struct come one by one, so the
        # first member ends before the next field at offset 0.
        later = [i for i, f in enumerate(fields) if i and f["offset"] == 0]
        fields = fields[:later[0]] if later else fields
    return fields_leaves(index, fields, at, name)


def fields_leaves(index: bss.Index, fields: list[dict], at: int, what: str) -> list[tuple[int, str, int]]:
    out = []
    for f in fields:
        if "bits" in f:
            raise Refused(f"{what}.{f['name']} is a bit field")
        off = at + f["offset"]
        count = f.get("count")
        inner = f.get("struct")
        members = f.get("members")
        if inner or members:
            n = count or 1
            size = f["elem_size"] if count else f["size"]
            for k in range(n):
                if members:
                    out += fields_leaves(index, members, off + k * size, f"{what}.{f['name']}")
                else:
                    out += struct_leaves(index, inner, off + k * size)
            continue
        t = f["type"].split("[")[0].strip()
        if "(" in f["type"] or "*" in t:
            cast, width = "void*", 4
        else:
            width = f["elem_size"] if count else f["size"]
            signed = f.get("elem_signed") if count else f.get("signed")
            if t in FLOATS or width not in (1, 2, 4):
                raise Refused(f"{what}.{f['name']} has type {f['type']}")
            cast = BASIC_CAST[(width, bool(signed))]
        if count:
            out.append((off, cast, count))
        else:
            out.append((off, cast, 0))
    return out


def leaf_width(cast: str) -> int:
    return 4 if cast.endswith("*") else data_inc.SIZES[cast]


def struct_row(index: bss.Index, name: str) -> tuple[str, list[int]]:
    """The data_inc type of a struct (`{u8,pad[3],u32}`) and the offsets of
    its 4-byte leaves, where relocations may sit."""
    leaves = struct_leaves(index, name)
    size = index.struct_size[name]
    parts, pos, words = [], 0, []
    for off, cast, count in sorted(leaves):
        if off < pos:
            raise Refused(f"struct {name} has overlapping members (a union?) at {off:#x}")
        if off > pos:
            parts.append(f"pad[{off - pos}]")
        width = leaf_width(cast)
        parts.append(f"{cast}[{count}]" if count else cast)
        if width == 4:
            words += [off + 4 * k for k in range(max(count, 1))]
        pos = off + width * max(count, 1)
    if pos > size:
        raise Refused(f"struct {name}: its members end at {pos:#x}, past its size {size:#x}")
    if pos < size:
        parts.append(f"pad[{size - pos}]")
    return "{" + ",".join(parts) + "}", words


DECL_SHAPE = re.compile(r"(?P<base>.*?)\b(?P<name>\w+)\s*(?P<dims>(?:\[[^\]]*\]\s*)*)$")
FUNC_PTR = re.compile(r"(?P<ret>.*?)\(\s*\*\s*(?P<name>\w+)\s*(?P<dims>(?:\[[^\]]*\]\s*)*)\)\s*(?P<args>\(.*\))$")


def object_words(module: DataModule, v: Var) -> list[tuple[int, int, tuple | None]]:
    """(offset, value, relocation or None) for each 4-byte word of an
    object, when it is 4-aligned and a multiple of 4 long."""
    raw = module.read(v.addr, v.sort)
    relocs = module.rom.reloc_table(module.name)
    return [(o, int.from_bytes(raw[o:o + 4], "little"), relocs.get(v.addr + o)) for o in range(0, len(raw) - 3, 4)]


def relocs_in(module: DataModule, v: Var) -> list[int]:
    """Offsets of the relocations inside an object."""
    relocs = module.rom.reloc_table(module.name)
    return [a - v.addr for a in sorted(relocs) if v.addr <= a < v.addr + v.sort]


def pointer_values(module: DataModule, v: Var, cast: str) -> list[str] | None:
    """The written-out initializer of an object whose words are all
    pointers or zero, at least one of them a pointer; None otherwise."""
    if v.sort % 4 or v.addr % 4:
        return None
    words = object_words(module, v)
    if not any(r for _, _, r in words) or any(val and not r for _, val, r in words):
        return None
    out = []
    for o, _, r in words:
        if r is None:
            out.append("0")
            continue
        kind, to, target = r
        if kind != "load" or target.startswith("overlays("):
            return None
        name, offset = module.rom.target(data_inc.module_key(target), to, f"{v.name}+{o:#x}")
        ref = f"((char*)&{name} + {offset:#x})" if offset else f"&{name}"
        out.append(f"({cast}){ref}" if offset or cast != "void*" else ref)
    return out


def check_relocs(module: DataModule, v: Var, words: list[int]) -> None:
    """Every relocation in the object sits on a 4-byte field."""
    allowed = set(words)
    for o in relocs_in(module, v):
        if o not in allowed:
            raise Refused(f"relocation at {v.name}+{o:#x} is not on a 4-byte field")


def shape_of(module: DataModule, v: Var, decl: bss.Decl | None, host: str | None) -> Shape:
    """How to write an object: from its declaration, or with none (decl
    None) from its bytes. Raises Refused when the declaration cannot be."""
    index = module.index
    if decl is None:
        return plain_shape(module, v)
    m = FUNC_PTR.fullmatch(decl.body)
    if m and m.group("name") == v.name:
        dims = re.findall(r"\[\s*(\w*)\s*\]", m.group("dims"))
        shape = Shape("void*", [], function_pointer=f"{m.group('ret').strip()} (*NAME DIMS){m.group('args')}")
        shape.dims = fit_dims(dims, 4, v)
        return pointer_shape(module, v, shape, "void*")
    m = DECL_SHAPE.fullmatch(decl.body)
    if not m or m.group("name") != v.name:
        raise Refused(f"cannot read the declaration `{decl.body}`")
    base = " ".join(m.group("base").split())
    dims = re.findall(r"\[\s*(\w*)\s*\]", m.group("dims"))
    t = index.base_type(base)
    headers = set()
    if decl.file.startswith("include/"):
        headers = {decl.file.removeprefix("include/")}
    elif decl.file != host:
        need = bss.needed_headers(index, decl)
        if need is None:
            raise Refused(f"declared in {decl.file}, with a type no header defines")
        headers = need
    if "*" in t:
        shape = Shape(base, [], headers=headers)
        shape.dims = fit_dims(dims, 4, v)
        if re.fullmatch(r"\w+\s*\*+", t):
            return pointer_shape(module, v, shape, t.replace(" ", ""))
        return pointer_shape(module, v, shape, "void*")
    if function_pointer_type(index, t, host):
        shape = Shape(base, [], headers=headers)
        shape.dims = fit_dims(dims, 4, v)
        return pointer_shape(module, v, shape, t)
    if t in index.struct_fields and t in index.struct_size and t not in bss.BASIC_SIZES:
        row, words = struct_row(index, t)
        size = index.struct_size[t]
        shape = Shape(base, fit_dims(dims, size, v), row=row, headers=headers)
        count = v.sort // size
        check_relocs(module, v, [k * size + w for k in range(count) for w in words])
        return shape
    cast = basic_cast(index, base)
    if cast is None or cast == "void*":
        raise Refused(f"type {base} is not one the tool can write")
    width = data_inc.SIZES[cast]
    shape = Shape(base, fit_dims(dims, width, v), headers=headers)
    columns = 1
    for d in shape.dims[1:]:
        columns *= int(d, 0)
    shape.row = cast + (f"[{columns}]" if len(shape.dims) > 1 else "")
    check_relocs(module, v, list(range(0, v.sort, 4)) if width == 4 else [])
    return shape


def fit_dims(dims: list[str], elem: int, v: Var) -> list[str]:
    """The declared dimensions, with a missing first count filled in from
    the object's size; the sizes must agree. Only the first count may be a
    macro."""
    inner = 1
    for d in dims[1:]:
        if not re.fullmatch(r"0x[0-9a-fA-F]+|\d+", d):
            raise Refused(f"dimension [{d}] is not a number")
        inner *= int(d, 0)
    if v.sort % (elem * inner):
        raise Refused(f"{v.sort:#x} bytes are not whole elements of {elem * inner:#x}")
    count = v.sort // (elem * inner)
    if not dims:
        if count != 1:
            raise Refused(f"declared as one element of {elem:#x} bytes, but takes {v.sort:#x}")
        return []
    # A count given by a macro stays; the compile check compares the size.
    if re.fullmatch(r"0x[0-9a-fA-F]+|\d+", dims[0]) and int(dims[0], 0) != count:
        raise Refused(f"declared with {dims[0]} elements, but takes {count}")
    return [str(count) if not dims[0] else dims[0]] + dims[1:]


def pointer_shape(module: DataModule, v: Var, shape: Shape, cast: str) -> Shape:
    """A pointer object: written out when its words are all pointers or
    zero, else values from the ROM (where a non-zero word with no
    relocation stops data_inc). `cast` is the element type: a pointer, or
    the name of a function pointer type, which data_inc writes as void*."""
    values = pointer_values(module, v, cast)
    if values is not None:
        shape.values = values
    elif not any(module.read(v.addr, v.sort)):
        shape.values = ["0"] * (v.sort // 4)
    else:
        shape.row = cast if cast.endswith("*") else "void*"
    return shape


FUNC_PTR_TYPEDEF = r"\btypedef\b[^;{{}}]*\(\s*\*\s*{}\s*\)\s*\("
_typedef_texts: dict[str, str] = {}


def function_pointer_type(index: bss.Index, t: str, host: str | None) -> bool:
    """Whether `t` names a function pointer type (`typedef BOOL
    (*TaskCommandFunc)(Task*);`) in a header or in the host file."""
    if not re.fullmatch(r"\w+", t):
        return False
    files = [f if f.startswith(("include/", "src/")) else f"include/{f}"
             for f in index.decls.type_headers.get(t, [])] + ([host] if host else [])
    pattern = re.compile(FUNC_PTR_TYPEDEF.format(re.escape(t)))
    for f in files:
        if f not in _typedef_texts:
            path = ROOT / f
            _typedef_texts[f] = path.read_text() if path.exists() else ""
        if pattern.search(_typedef_texts[f]):
            return True
    return False


def plain_shape(module: DataModule, v: Var) -> Shape:
    """An object with no declaration to copy, from its bytes."""
    raw = module.read(v.addr, v.sort)
    if string_size(raw + b"\0" * (round4(v.sort) - v.sort)) == v.sort and not relocs_in(module, v):
        return Shape("char", [str(v.sort)], row="u8")
    if v.sort % 4 == 0:
        values = pointer_values(module, v, "void*")
        if values is not None:
            return Shape("void*", [str(len(values))] if len(values) > 1 else [], values=values)
        check_relocs(module, v, list(range(0, v.sort, 4)))
        n = v.sort // 4
        return Shape("u32", [str(n)] if n > 1 else [], row="u32")
    if relocs_in(module, v):
        raise Refused(f"{v.name} has relocations but its size {v.sort:#x} is not a multiple of 4")
    if v.sort % 2 == 0:
        return Shape("u16", [str(v.sort // 2)], row="u16")
    return Shape("u8", [str(v.sort)], row="u8")


def round4(n: int) -> int:
    return (n + 3) & ~3


# ---------------------------------------------------------------- definitions

def const_base(base: str, section: str) -> str:
    """The element type with the object const in .rodata and not in .data."""
    t = base.strip()
    if "*" in t:
        t = re.sub(r"\s*\bconst\s*$", "", t)
        return f"{t} const" if section == ".rodata" else t
    t = re.sub(r"^const\s+", "", t)
    return f"const {t}" if section == ".rodata" else t


def inc_path(module: str, name: str) -> str:
    return f"data/{module}/{name}.inc.c"


@dataclass
class Written:
    """One object's definition and what it needs."""
    var: Var
    shape: Shape
    text: str
    zero: bool


def definition(module: DataModule, v: Var, shape: Shape, section: str, static: bool, decl: bss.Decl | None,
               host: str | None, with_lead: bool) -> Written:
    # bss.build_items notes how it sized an object with no declared size;
    # here the bytes are there to see, so only other notes are kept.
    own = [n for n in v.note.split("; ") if n and not n.startswith(("size: up to", "count: as many"))]
    notes = ([decl.comment] if decl is not None and decl.comment else []) + shape.notes + own
    lead = ("static " if static else "") + (bss.ALIGN32 if v.align32 else "")
    base = const_base(shape.base, section)
    if shape.function_pointer:
        fp = shape.function_pointer
        fp = fp.replace("(*NAME", "(* const NAME") if section == ".rodata" else fp
        declarator = Shape(base, shape.dims, function_pointer=fp).declarator(v.name)
    else:
        declarator = Shape(base, shape.dims).declarator(v.name)
    if shape.values is not None:
        init = shape.values[0] if not shape.dims else "{" + ", ".join(shape.values) + "}"
        code = f"{lead}{declarator} = {init};"
    else:
        code = f'{lead}{declarator} = {{\n#include "{inc_path(module.name, v.name)}"\n}};'
    zero = not any(module.read(v.addr, v.sort))
    above = decl.lead + "\n" if with_lead and decl is not None and decl.file == host and decl.lead else ""
    if notes:
        # After a one-line definition when it fits, else on the line above.
        note = f"/* {'; '.join(notes)} */"
        code = f"{code} {note}" if "\n" not in code and len(code) + 1 + len(note) <= 80 else f"{note}\n{code}"
    return Written(v, shape, above + code, zero)


def shapes_for(module: DataModule, g: Group, plain: bool = False) -> tuple[list[tuple[Var, Shape, bss.Decl | None]], list[str]]:
    """Each object's shape and the declaration it copies. A declaration the
    tool cannot write stops a host (it must replace it) and turns into a
    plain shape in a new file, with a note. With `plain`, a new file
    ignores declarations from .c files."""
    out, errors = [], []
    for v in g.variables:
        decl = None if v.placeholder else bss.chosen_decl(module.index, v, g.host)
        if decl is not None and plain and not g.host and not decl.file.startswith("include/"):
            decl = None
        try:
            shape = shape_of(module, v, decl, g.host)
        except Refused as e:
            if g.host and decl is not None and decl.file == g.host:
                errors.append(f"{v.name}: {e}")
                continue
            try:
                shape = shape_of(module, v, None, g.host)
            except Refused as e2:
                errors.append(f"{v.name}: {e2}")
                continue
            if decl is not None:
                shape.notes.append(f"declared `{decl.body}` in {Path(decl.file).name}: {e}")
            decl = None
        out.append((v, shape, decl))
    return out, errors


def inc_rows(module: DataModule, g: Group, shapes: list[tuple[Var, Shape, bss.Decl | None]]) -> list[tuple[data_inc.Row, str]]:
    """The data_inc.txt rows of the objects written from the ROM, each with
    its element type as the row gives it."""
    rows = []
    for v, shape, _ in shapes:
        if shape.row is not None:
            rows.append((data_inc.Row(f"{module.name} {v.name} {v.addr:#010x} {v.sort:#x} {shape.row}",
                                      f"{g.path or g.host}: {v.name}"), shape.row))
    return rows


def ordered(written: list[Written]) -> list[Written]:
    """The definitions in the order that makes mwccarm emit them by address
    (tools/mwcc_order.py; all initialised)."""
    by_addr = sorted(written, key=lambda w: w.var.addr)
    return [by_addr[i] for i in definition_order([w.var.sort for w in by_addr], initialized=True)]


def block_lines(written: list[Written], forwards: list[str]) -> list[str]:
    lines = list(forwards)
    if forwards:
        lines.append("")
    zero = any(w.zero for w in written)
    if zero:
        lines.append(ZERO_ON)
    lines += [w.text for w in ordered(written)]
    if zero:
        lines.append(ZERO_OFF)
    return lines


# ---------------------------------------------------------------- compiling

_compiled: dict = {}
compile_errors: dict = {}


class Probe:
    """Writes the .inc.c files of planned rows into a directory the compiles
    include first, so the C can be checked before data_inc.txt changes."""

    def __init__(self, rom: data_inc.Rom) -> None:
        self.rom = rom
        self.dir = Path(tempfile.mkdtemp(prefix="data_probe"))
        self.written: dict[str, str] = {}

    def add(self, rows: list[tuple[data_inc.Row, str]]) -> list[str]:
        """Writes the rows' files; returns the errors data_inc stops with."""
        errors = []
        for row, _ in rows:
            if row.rel in self.written:
                continue
            try:
                text = data_inc.render(row, self.rom)
            except SystemExit as e:
                errors.append(str(e.code))
                continue
            path = self.dir / "data" / row.rel
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text)
            self.written[row.rel] = text
        return errors

    def compile(self, src: str, text: str):
        key = (src, text)
        if key not in _compiled:
            cc, flags = configure.compiler_for(src)
            cmd = [sys.executable, str(ROOT / cc)] if cc.endswith(".py") else [str(ROOT / cc)]
            flags = flags.replace("-i include", f"-i {self.dir} -i include -i {Path(src).parent}")
            with tempfile.TemporaryDirectory(prefix="data_probe_c") as d:
                c, obj = Path(d) / "probe.c", Path(d) / "probe.o"
                c.write_text(text)
                r = subprocess.run([*cmd, "-c", *flags.split(), "-o", str(obj), str(c)], cwd=ROOT,
                                   capture_output=True, text=True)
                _compiled[key] = bss.elf_read(obj) if r.returncode == 0 and obj.exists() else None
                if _compiled[key] is None:
                    compile_errors[key] = " ".join((r.stdout + r.stderr).split())
        return _compiled[key]

    def close(self) -> None:
        shutil.rmtree(self.dir, ignore_errors=True)


def emitted_data(elf) -> dict[str, list[str]]:
    """Object symbols in .data and .rodata sections, by section name, in the
    order the linker places them."""
    sections, symbols = elf
    found = sorted((s["shndx"], s["name"]) for s in symbols
                   if s["type"] == 1 and 0 < s["shndx"] < len(sections)
                   and sections[s["shndx"]]["type"] == 1 and sections[s["shndx"]]["name"] in DATA_SECTIONS)
    out: dict[str, list[str]] = defaultdict(list)
    for shndx, name in found:
        out[sections[shndx]["name"]].append(name)
    return out


def data_errors(elf, g: Group, bss_before: dict | None) -> list[str]:
    """What is wrong with the object: the group's objects out of order or in
    another section, a size other than the ROM's, other data in the file,
    or (for a host) its .bss in another order than before."""
    got = emitted_data(elf)
    names = [v.name for v in g.variables]
    errors = []
    if got.get(g.section, []) != names:
        errors.append(f"the compiler emits {g.section} as {got.get(g.section, [])}, expected {names}")
    for sec, objs in got.items():
        if sec != g.section and objs:
            errors.append(f"the file also has {sec}: {objs}")
    sizes = {s["name"]: s["size"] for s in elf[1] if s["type"] == 1}
    aligns = {s["name"]: elf[0][s["shndx"]]["align"] for s in elf[1]
              if s["type"] == 1 and 0 < s["shndx"] < len(elf[0])}
    for v in g.variables:
        if v.name in sizes and sizes[v.name] != v.sort:
            errors.append(f"{v.name} compiles to {sizes[v.name]:#x} bytes, expected {v.sort:#x}")
        if v.align32 and aligns.get(v.name, 32) < 32:
            errors.append(f"{v.name}: section aligned to {aligns[v.name]}, needs 32")
    if bss_before is not None and bss.emitted_order(elf) != bss_before:
        errors.append("the file's .bss comes out in another order")
    return errors


# -msgstyle gcc can put the file and line again inside the message.
UNDEFINED = re.compile(r"undefined identifier\s+(?:\S+:\d+:\s*)?'(\w+)'")
PROTOTYPE = re.compile(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;{}()]*\)\s*;", re.M)


class Targets:
    """Where the names that initializers point at are declared."""

    def __init__(self, index: bss.Index) -> None:
        self.index = index
        self.kinds: dict[str, str] = {}
        for module in data_modules() + ["itcm"]:
            for s in bss.read_symbols(ROOT / data_inc.config_dir(module) / "symbols.txt"):
                self.kinds.setdefault(s.name, s.kind)
        self.prototypes: dict[str, str] = {}
        for path in sorted((ROOT / "include").rglob("*.h")):
            text = re.sub(r"/\*.*?\*/", "", path.read_text(errors="replace"), flags=re.S)
            for m in PROTOTYPE.finditer(text):
                self.prototypes.setdefault(m.group(1), path.relative_to(ROOT / "include").as_posix())

    def declare(self, name: str, near: str) -> tuple[str | None, str | None]:
        """(header to include, or a declaration to write) for a name."""
        if self.kinds.get(name) == "function":
            header = self.prototypes.get(name)
            return (header, None) if header else (None, f"void {name}(void);")
        headers = [d.file for d in self.index.decls.of(name) if d.file.startswith("include/")]
        if headers:
            return bss.pick_header(headers, near), None
        return None, f"extern u8 {name}[];"


def compose(module: DataModule, g: Group, written: list[Written], headers: set[str], forwards: list[str],
            in_place: bool = True) -> str:
    lines = block_lines(written, forwards)
    if g.host:
        return bss.host_file_text(module.index, g, lines, headers, False, in_place)
    return bss.new_file_text(lines, headers, False)


def render(module: DataModule, g: Group, relocs: list, probe: Probe,
           targets: Targets) -> tuple[str | None, list[str], list[tuple[data_inc.Row, str]]]:
    """The C for a group and its data_inc.txt rows, checked with the compiler: it compiles, its
    objects come out in address order in the group's section with the
    ROM's sizes, and a host's .bss keeps its order. A host's `extern`
    statements are replaced (else the definitions go at the end of the
    file); a new file first copies declarations from .c files, then
    leaves them out."""
    index = module.index
    if module.errors:
        return None, module.errors, []
    attempts = [False, True] if not g.host else [False]
    bss_before = None
    if g.host:
        before = probe.compile(g.host, (ROOT / g.host).read_text())
        if before is None:
            return None, ["the host does not compile as it is"], []
        bss_before = bss.emitted_order(before)
    static = bss.static_names(module, g, relocs)
    errors: list[str] = []
    for plain in attempts:
        shapes, errs = shapes_for(module, g, plain)
        if errs:
            errors += errs
            continue
        rows = inc_rows(module, g, shapes)
        errs = probe.add(rows)
        if errs:
            errors += errs
            continue
        headers = set().union(*(s.headers for _, s, _ in shapes)) if shapes else set()
        written = [definition(module, v, s, g.section, v.name in static, d, g.host, True) for v, s, d in shapes]
        defined = {v.name: s for v, s, _ in shapes}
        for in_place in ([True, False] if g.host else [True]):
            forwards: list[str] = []
            extra: set[str] = set()
            text = ""
            # mwcc names one undefined identifier per run, and a table can
            # point at a hundred functions; each pass declares a new name or
            # stops, so the loop ends.
            while True:
                text = compose(module, g, written, headers | extra, forwards, in_place)
                elf = probe.compile(g.path or g.host, text)
                if elf is not None:
                    break
                missing = sorted(set(UNDEFINED.findall(compile_errors[(g.path or g.host, text)])))
                new = False
                for name in missing:
                    if name in defined:
                        line = defined[name].forward(name, g.section)
                    else:
                        header, line = targets.declare(name, g.host or "")
                        if header and header not in extra:
                            extra.add(header)
                            new = True
                            continue
                    if line and line not in forwards:
                        forwards.append(line)
                        new = True
                if not new:
                    break
            if elf is None:
                errors.append("does not compile: " + compile_errors.get((g.path or g.host, text), "")[:400])
                continue
            errs = data_errors(elf, g, bss_before)
            if not errs:
                return text, [], rows
            errors += errs
    return None, errors, []


# ---------------------------------------------------------------- planning

def data_host_allowed(module: DataModule, path: str, addr: int, probe: Probe) -> bool:
    """Whether `path` can take a slice at `addr`: complete, with its code in
    the same part of the module, and no data of its own (.bss is fine)."""
    entry = next((e for e in module.entries if e.name == path), None)
    if entry is None or not entry.complete or not (ROOT / entry.name).exists():
        return False
    if any(s in entry.sections for s in DATA_SECTIONS):
        return False
    if any(module.segment(start) != module.segment(addr) for start, _ in entry.sections.values()):
        return False
    elf = probe.compile(entry.name, (ROOT / entry.name).read_text())
    if elf is None:
        return False
    return all(s["name"] in bss.CODE_SECTIONS or s["name"].split(".run")[0] in bss.BSS_SECTIONS
               for s in elf[0] if s["flags"] & 2)  # SHF_ALLOC


def name_groups(module: DataModule, groups: list[Group]) -> None:
    taken = bss.all_basenames()
    lead = "" if module.name == "main" else f"{module.name}_"
    for g in groups:
        if g.host:
            g.path = g.host
            continue
        kind = g.section.lstrip(".")
        topic = Path(g.directory).name
        stem = f"{topic}_{kind}" if g.directory != bss.module_src_dir(module.name) else f"{lead}{kind}_{g.start:08x}"
        if stem in taken:
            stem = f"{topic}_{kind}_{g.start:08x}"
        taken.add(stem)
        g.path = f"{g.directory}/{stem}.c"


@dataclass
class Plan:
    module: DataModule
    groups: list[Group]
    runs_before: int
    runs_after: int
    past: list = field(default_factory=list)
    refused: list = field(default_factory=list)


def make_plan(module: DataModule, probe: Probe, targets: Targets, lo: int | None = None, hi: int | None = None,
              listed: dict | None = None, refuse: bool = True) -> Plan:
    def in_range(v: Var) -> bool:
        return (lo is None or v.addr >= lo) and (hi is None or v.addr < hi)

    module.plan_variables(split=False)
    runs_before = sum(len(bss.split_runs([v for v in g.variables if in_range(v)])) for g in module.gaps)
    module.plan_variables(split=True)
    if refuse:
        listed = bss.read_alias_list() if listed is None else listed
        refused = bss.refused_aliases(module, [v for g in module.gaps for v in g.variables if in_range(v)], listed)
        if refused:
            return Plan(module, [], runs_before, 0, [], refused)
    users = bss.Users(module)
    relocs = bss.read_relocs()
    index = module.index
    cache: dict[tuple[str, str], bool] = {}

    def allowed(path: str, v: Var) -> bool:
        key = (path, v.name)
        if key not in cache:
            cache[key] = data_host_allowed(module, path, v.addr, probe) and bss.declarations_fit(index, path, [v])
        return cache[key]

    groups: list[Group] = []
    past = []
    runs_after = 0
    for gap in module.gaps:
        past += [a for a in gap.past if (lo is None or a.var_addr >= lo) and (hi is None or a.var_addr < hi)]
        for run in bss.split_runs([v for v in gap.variables if in_range(v)]):
            for host, variables in bss.slice_run(module, users, run, allowed):
                groups.append(Group([variables], "", host, section=gap.section, run=runs_after))
            runs_after += 1
    if past:
        return Plan(module, [], runs_before, runs_after, past)
    for g in groups:
        g.directory = bss.users_directory(module, users, g.variables) or bss.module_src_dir(module.name)
    name_groups(module, groups)  # a path for the compile checks of hostless groups

    def check(m, g: Group, _relocs) -> tuple[str | None, list[str]]:
        return render(module, g, relocs, probe, targets)[:2]

    groups = bss.assign_hosts(module, groups, relocs, check=check, per_file=True)
    for g in groups:
        if not g.host:
            g.directory = bss.users_directory(module, users, g.variables) or bss.module_src_dir(module.name)
            g.path = ""
    bss.add_tails(module, groups)
    name_groups(module, groups)
    return Plan(module, groups, runs_before, runs_after, past)


# ---------------------------------------------------------------- the config

SECTION_ORDER = [".text", ".init", ".rodata", ".ctor", ".data", ".sbss", ".bss"]


def edit_delinks(path: Path, groups: list[Group]) -> None:
    lines = path.read_text().split("\n")
    for g in groups:
        sec = bss.section_line(g.section, g.start, g.end + g.tail)
        rank = SECTION_ORDER.index(g.section)
        if g.host:
            i = lines.index(f"{g.host}:")
            j = i + 1
            while j < len(lines) and lines[j].strip():
                m = bss.SECTION_LINE.match(lines[j])
                if m and m.group(1) in SECTION_ORDER and SECTION_ORDER.index(m.group(1)) > rank:
                    break
                j += 1
            lines.insert(j, sec)
            continue
        # After the last entry whose range in this section comes before it, else at the end.
        block = ["", f"{g.path}:", "    complete", sec]
        at, best, cur = None, -1, None
        for j, line in enumerate(lines):
            m = bss.SECTION_LINE.match(line)
            if line and not line[0].isspace():
                cur = j
            elif m and m.group(1) == g.section and cur is not None:
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
            lines += block + [""]
        else:
            lines[at:at] = block
    path.write_text("\n".join(lines))


def edit_symbols(path: Path, variables: list[Var]) -> None:
    """Each object gets its size, `kind:data(byte[N])`, each alias size 0,
    and placeholders are added in address order."""
    lines = path.read_text().split("\n")
    where = {}
    for i, line in enumerate(lines):
        m = bss.SYMBOL_LINE.match(line)
        if m:
            where[m.group(1)] = (i, int(m.group(4), 16))
    added = []
    sized = [(v, v.name, v.sort) for v in variables] + [(None, n, 0) for v in variables for n, _ in v.aliases]
    for v, name, n in sized:
        kind = f"kind:data(byte[{n}])"
        if name in where:
            i = where[name][0]
            lines[i] = re.sub(r"kind:data(\([^)]*\))?", kind, lines[i], count=1)
        elif v is None:
            fail(f"alias {name} is not in {path}")
        else:
            added.append((v.addr, f"{v.name} {kind} addr:{v.addr:#010x}"))
    rows = sorted(where.values())
    for addr, line in sorted(added, reverse=True):
        at = next((i for i, a in rows if a > addr), len(lines) - (lines[-1] == ""))
        lines.insert(at, line)
    path.write_text("\n".join(lines))


def row_text(row: data_inc.Row, raw_type: str) -> str:
    return f"{row.module:<9} {row.symbol:<27} {row.addr:#010x}  {row.size:<#6x} {raw_type}".rstrip()


def edit_data_inc(rows: list[tuple[data_inc.Row, str]]) -> None:
    """Adds rows to config/AWRE/data_inc.txt after the last row of the same
    module (at the end when it has none), in address order. A row for a
    symbol already listed replaces it."""
    path = ROOT / data_inc.LIST
    lines = path.read_text().rstrip("\n").split("\n")
    names = {r.symbol for r, _ in rows}
    lines = [ln for ln in lines if not (ln.strip() and not ln.startswith("#") and ln.split()[1:2]
                                        and ln.split()[1] in names)]
    for row, raw in sorted(rows, key=lambda r: r[0].addr, reverse=True):
        last = max((i for i, ln in enumerate(lines)
                    if ln.strip() and not ln.startswith("#") and ln.split()[0] == row.module
                    and int(ln.split()[2], 16) < row.addr), default=None)
        if last is None:
            same = [i for i, ln in enumerate(lines) if ln.strip() and not ln.startswith("#") and ln.split()[0] == row.module]
            last = (same[0] - 1) if same else len(lines) - 1
        lines.insert(last + 1, row_text(row, raw))
    path.write_text("\n".join(lines) + "\n")


# ---------------------------------------------------------------- report

def report_errors(paths: list[str]) -> list[str]:
    """Units of these files whose data the objdiff report scores below 100%
    matched, or that it does not list."""
    if not REPORT.exists():
        return [f"{REPORT.relative_to(ROOT)} is missing"]
    report = json.loads(REPORT.read_text())
    units = {u.get("metadata", {}).get("source_path"): u for u in report["units"]}
    out = []
    for p in paths:
        u = units.get(p)
        if u is None:
            out.append(f"{p}: not in the report")
            continue
        m = u["measures"]
        if int(m.get("total_data", 0)) != int(m.get("matched_data", 0)):
            out.append(f"{p}: data {m.get('matched_data', 0)} / {m.get('total_data', 0)} bytes matched")
    return out


# ---------------------------------------------------------------- commands

def describe(v: Var) -> str:
    what = "placeholder" if v.placeholder else ("declared" if v.declared else "size from gap")
    extra = "".join(f", alias {n} +{o:#x}" for n, o in v.aliases)
    return f"    {v.addr:#010x} {v.sort:#8x} {v.name} ({what}{extra}{', align 32' if v.align32 else ''})"


def summary(plan: Plan) -> str:
    variables = [v for g in plan.groups for v in g.variables]
    hosted = sum(1 for g in plan.groups if g.host)
    return (f"{plan.module.name}: {len(variables)} objects "
            f"({sum(not v.placeholder for v in variables)} named, {sum(v.placeholder for v in variables)} placeholders,"
            f" {sum(len(v.aliases) for v in variables)} aliases, {sum(v.align32 for v in variables)} aligned to 32), "
            f"{sum(v.size for v in variables) + sum(g.tail for g in plan.groups):#x} bytes; runs: {plan.runs_before}"
            f" before cutting filler, {plan.runs_after} after; files: {len(plan.groups)} ({hosted} in existing files,"
            f" {len(plan.groups) - hosted} new)")


def tools_for(module_name: str) -> tuple[DataModule, Probe, Targets]:
    index = bss.Index()
    module = DataModule(module_name, index)
    return module, Probe(module.rom), Targets(index)


def cmd_scan(args) -> int:
    module, probe, targets = tools_for(args.module)
    try:
        for e in module.errors:
            print(f"warning: {e}")
        plan = make_plan(module, probe, targets, refuse=False)
        if plan.past:
            print("warning: " + bss.past_report(module.index, plan.past))
        for gap in module.gaps:
            print(f"{gap.section} gap {gap.start:#010x}-{gap.end:#010x} ({gap.end - gap.start:#x} bytes)")
            if args.verbose:
                for k, run in enumerate(bss.split_runs(gap.variables)):
                    print(f"  run {k}: {len(run)} objects")
                    for v in run:
                        print(describe(v))
        print(summary(plan))
    finally:
        probe.close()
    return 0


def cmd_plan(args) -> int:
    lo, hi = bss.parse_range(args.range) if args.range else (None, None)
    module, probe, targets = tools_for(args.module)
    try:
        return plan_and_write(module, probe, targets, lo, hi, args)
    finally:
        probe.close()


def plan_and_write(module: DataModule, probe: Probe, targets: Targets, lo, hi, args) -> int:
    if module.errors:
        fail("; ".join(module.errors))
    plan = make_plan(module, probe, targets, lo, hi)
    if plan.past:
        print(bss.past_report(module.index, plan.past))
        fail(f"{len(plan.past)} aliases lie past the declared size of their object: declare the objects "
             "with the size that covers them, then run again")
    if plan.refused:
        print(bss.refused_report(plan.refused))
        fail(f"{len(plan.refused)} new aliases refused: check what each one is, fix the object's size or list "
             f"the alias in {bss.ALIAS_LIST} (docs/bss-tool.md, \"Aliases\"), then run again")
    relocs = bss.read_relocs()
    texts, rows = {}, []
    for g in plan.groups:
        kind = "existing" if g.host else "new"
        print(f"{g.path} ({kind}): {g.section} {g.start:#010x}-{g.end + g.tail:#010x}, {len(g.variables)} objects")
    print(summary(plan))
    for g in plan.groups:
        text, errors, group_rows = render(module, g, relocs, probe, targets)
        if text is None:
            fail(f"{g.path}: " + "; ".join(errors))
        texts[g.path] = text
        rows += group_rows
        if args.show:
            print(f"\n--- {g.path}: {g.section} start:{g.start:#010x} end:{g.end + g.tail:#010x}")
            print(text, end="")
    if not args.write or not plan.groups:
        if args.write:
            print("nothing to write")
        return 0
    if len(texts) != len(plan.groups):
        fail("two slices would go into one file")
    for path, text in texts.items():
        out = ROOT / path
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(text)
    subprocess.run([sys.executable, "tools/format.py", *texts], cwd=ROOT, check=True)
    edit_delinks(module.cfg / "delinks.txt", plan.groups)
    edit_symbols(module.cfg / "symbols.txt", [v for g in plan.groups for v in g.variables])
    edit_data_inc(rows)
    for path in texts:
        print(f"wrote {path}")
    bss.build_and_check()
    rep = subprocess.run(["make", "--no-print-directory", "decomp-dev-report"], cwd=ROOT, capture_output=True,
                         text=True)
    if rep.returncode:
        print(rep.stdout[-4000:] + rep.stderr[-4000:])
        fail("make decomp-dev-report failed")
    errors = report_errors(list(texts))
    if errors:
        fail("the report scores these units below 100%: " + "; ".join(errors))
    print(f"the report scores the data of all {len(texts)} files 100% matched")
    return 0


# ---------------------------------------------------------------- relocations to check

def cmd_relocs(args) -> int:
    """Relocations in the module's data that may be wrong: dsd takes some
    words that hold two 16-bit values for pointers (docs/data-tool.md
    section 4). Lists a relocation when it is not on a 4-byte boundary,
    when its target is not aligned, when the declared type of the object
    (other than a byte array) puts a field of another size there, or when
    its target is in .bss or in another module and nothing else (no
    function, no other relocation) refers to the target's symbol. Each comes with who reads
    the object; check each against that code (fn.py show, ds.py field)
    before deleting it from relocs.txt."""
    index = bss.Index()
    module = DataModule(args.module, index)
    users = bss.Users(module)
    all_relocs = bss.read_relocs()
    relocs = module.rom.reloc_table(module.name)
    ranges = [(s, a, b) for s, a, b in module.header if s in DATA_SECTIONS and b > a]
    syms = sorted(module.symbols, key=lambda s: s.addr)
    kinds = Targets(index).kinds
    tables: dict[str, list[bss.Symbol]] = {}
    found = 0
    for frm in sorted(relocs):
        sec = next((s for s, a, b in ranges if a <= frm < b), None)
        if sec is None:
            continue
        kind, to, target = relocs[frm]
        tmod = data_inc.module_key(target)
        owner = max((s for s in syms if s.addr <= frm), key=lambda s: s.addr, default=None)
        name = f"{owner.name}+{frm - owner.addr:#x}" if owner else f"{frm:#x}"
        reasons = []
        if frm % 4:
            reasons.append("not on a 4-byte boundary")
        if to % 2:
            reasons.append(f"odd target {to:#x}")
        tdesc = "no symbol"
        if not tmod.startswith("overlays("):
            if tmod not in tables:
                tables[tmod] = sorted(bss.read_symbols(ROOT / data_inc.config_dir(tmod) / "symbols.txt"),
                                      key=lambda s: s.addr)
            below = [s for s in tables[tmod] if s.addr <= to]
            tsym = below[-1] if below else None
            if tsym is not None:
                tdesc = tsym.name + (f"+{to - tsym.addr:#x}" if to != tsym.addr else "")
                is_func = kinds.get(tsym.name) == "function" and to - tsym.addr in (0, 1)
                theader = bss.read_delinks(ROOT / data_inc.config_dir(tmod) / "delinks.txt")[0]
                in_bss = any(s in bss.BSS_SECTIONS and a <= to < b for s, a, b in theader)
                if not is_func and (in_bss or tmod != module.name):
                    nxt = next((s.addr for s in tables[tmod] if s.addr > tsym.addr), tsym.addr + 4)
                    others = [r for r in all_relocs if r[3] == tmod and tsym.addr <= r[2] < nxt
                              and not (r[0] == module.name and r[1] == frm)]
                    if not others:
                        reasons.append(f"nothing else refers to {tsym.name}")
        decl = next(iter(index.decls.of(owner.name)), None) if owner else None
        if decl is not None:
            base = DECL_SHAPE.fullmatch(decl.body)
            if not (base and index.base_type(base.group("base")) in ("u8", "s8", "char", "unsigned char")):
                try:
                    v = Var(owner.addr, 0, frm - owner.addr + 4, owner.name)
                    shape_of(module, v, decl, None)
                except Refused as e:
                    if "not on a 4-byte field" in str(e):
                        reasons.append(f"`{decl.body}` has no 4-byte field there")
        if not reasons:
            continue
        found += 1
        readers = users.of(Var(owner.addr, 4, 4, owner.name)) if owner else []
        print(f"{frm:#010x} {sec} {name} -> {to:#010x} {target} ({tdesc}): {'; '.join(reasons)}")
        print(f"    read by: {', '.join(readers) or 'no function'}")
    print(f"{module.name}: {found} relocations to check")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = parser.add_subparsers(required=True)
    p = sub.add_parser("scan", help="list objects, aliases, filler and runs")
    p.add_argument("module")
    p.add_argument("--verbose", action="store_true")
    p.set_defaults(func=cmd_scan)
    p = sub.add_parser("plan", help="the files it would write; --write writes them")
    p.add_argument("module")
    p.add_argument("--range", help="LO-HI: only the objects that start in it")
    p.add_argument("--show", action="store_true", help="print the C of each file (unformatted)")
    p.add_argument("--write", action="store_true")
    p.set_defaults(func=cmd_plan)
    p = sub.add_parser("relocs", help="relocations in the module's data to check by hand")
    p.add_argument("module")
    p.set_defaults(func=cmd_relocs)
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
