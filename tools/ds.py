#!/usr/bin/env python3
"""Data-structure helper for the decompile loop (issue 0041, docs/workflow.md).

    ds.py show STRUCT [--uses]                 layout from the compiler: offset, size, type and
                                               name of each field, bit fields, the header, data
                                               symbols declared with it, and how many functions
                                               in src/ touch each field (--uses lists them)
    ds.py show data_ADDR|gName|0xADDR [--count N]
                                               module, kind, size to the next symbol, declared
                                               type, references (functions and tables), whether
                                               it lies inside another symbol, and its bytes
                                               from extract/ decoded with the declared type
    ds.py field STRUCT.FIELD                   each function in src/ that uses the field, with the
                                               original loads and stores at its offset (ldrh or
                                               ldrsh, ldrb or ldrsb, word), casts on it in our C,
                                               and whether the declared signedness agrees

Layouts come from build/dsindex.json (tools/dsindex.py), which ds.py
refreshes when a header or C file is newer. Field uses are found by name in
the C files that include the struct's header, so a field name shared by two
structs in one file shows both. Loads and stores are read from the original
bytes in build/fnindex.json, only for functions that already have C, and
only at the field's offset in this struct (an access through an outer
struct that embeds it is not counted).

Read-only: never runs ninja or dsd, never writes outside build/dsindex*.
"""

import argparse
import bisect
import re
import sys
from functools import cache
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import dsindex  # noqa: E402
import fn  # noqa: E402
import fnindex  # noqa: E402

ROOT = fn.ROOT
SRC = ROOT / "src"

BASE_SIZES = {"u8": 1, "s8": 1, "char": 1, "BOOL": 4, "u16": 2, "s16": 2, "u32": 4, "s32": 4,
              "int": 4, "fx32": 4, "u64": 8, "s64": 8, "fx16": 2, "short": 2, "long": 4}
BASE_SIGNED = {"s8": True, "s16": True, "s32": True, "int": True, "fx32": True, "fx16": True,
               "short": True, "long": True, "BOOL": True, "s64": True}


# ---------------------------------------------------------------- index

@cache
def index() -> dict:
    return dsindex.load()


def structs(name: str) -> list[tuple[str, dict]]:
    """(header, record) for every header that defines a type of this name."""
    return [(h, s) for h, rec in index()["headers"].items() for s in rec["structs"] if s["name"] == name]


def find_struct(name: str, prefer: str | None = None) -> tuple[str, dict] | None:
    hits = structs(name)
    for h, s in hits:
        if h == prefer:
            return h, s
    return hits[0] if hits else None


def flatten(fields: list[dict], base: int = 0, prefix: str = "", header: str | None = None,
            depth: int = 0) -> list[dict]:
    """Every field, with members of embedded structs as dotted paths."""
    out = []
    for f in fields:
        path = prefix + f["name"] if f["name"] else prefix.rstrip(".")
        g = dict(f, path=path, offset=base + f["offset"], depth=depth)
        out.append(g)
        inner = f.get("members")
        if inner is None and f.get("struct") and depth < 6 and f.get("count", 1) == 1:
            hit = find_struct(f["struct"], header)
            inner = hit[1].get("fields") if hit else None
        if inner:
            out += flatten(inner, base + f["offset"], path + "." if path else "", header, depth + 1)
    return out


def type_size(typ: str) -> int | None:
    t = typ.replace("const ", "").replace("volatile ", "").strip()
    if t.endswith("*"):
        return 4
    if t in BASE_SIZES:
        return BASE_SIZES[t]
    hit = find_struct(t.replace("struct ", "").replace("union ", ""))
    return hit[1]["size"] if hit else None


# ---------------------------------------------------------------- C sources

COMMENT_OR_STRING = re.compile(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'|^[ \t]*#(?:[^\n]*\\\n)*[^\n]*", re.S | re.M)
FUNC_HEAD = re.compile(r"(\w+)\s*\([^;{}]*\)\s*$", re.S)


def blank(text: str) -> str:
    """Comments, string literals and preprocessor lines replaced by spaces,
    offsets kept."""
    return COMMENT_OR_STRING.sub(lambda m: re.sub(r"[^\n]", " ", m.group(0)), text)


def functions_in(text: str) -> list[tuple[str, int, int]]:
    """(name, body start, body end) of each top-level function definition."""
    clean = blank(text)
    out, depth, last, start = [], 0, 0, 0
    for i, c in enumerate(clean):
        if c == "{":
            if depth == 0:
                m = FUNC_HEAD.search(clean, last, i)
                head = clean[last:i]
                name = m.group(1) if m and "=" not in head else None
                start = i
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                if name:
                    out.append((name, start, i + 1))
                last = i + 1
        elif c == ";" and depth == 0:
            last = i + 1
    return out


@cache
def includers(header: str) -> frozenset[str]:
    """Headers (paths relative to include/) that include `header` directly
    or through other headers, `header` itself included."""
    rel = str(Path(header).relative_to("include"))
    graph: dict[str, set[str]] = {}
    for h in (ROOT / "include").rglob("*.h"):
        for name in dsindex.INCLUDE_LINE.findall(h.read_text(errors="replace")):
            graph.setdefault(name, set()).add(str(h.relative_to(ROOT / "include")))
    seen, todo = {rel}, [rel]
    while todo:
        for parent in graph.get(todo.pop(), ()):
            if parent not in seen:
                seen.add(parent)
                todo.append(parent)
    return frozenset(seen)


@cache
def src_files() -> list[tuple[Path, str]]:
    return [(p, p.read_text(errors="replace")) for p in sorted(SRC.rglob("*.c"))]


def files_including(header: str) -> list[tuple[Path, str]]:
    want = includers(header)
    return [(p, t) for p, t in src_files() if want & set(dsindex.INCLUDE_LINE.findall(t))]


def field_token(path: str) -> re.Pattern:
    """`->a.b` or `.a.b` for path a.b."""
    return re.compile(r"(?:->|\.)\s*" + r"\s*\.\s*".join(map(re.escape, path.split("."))) + r"\b")


def uses(header: str, field: str) -> list[tuple[Path, str, str]]:
    """(file, function, body) for each function that names the field."""
    tok = field_token(field)
    out = []
    for p, text in files_including(header):
        if not tok.search(text):
            continue
        clean = blank(text)
        for name, s, e in functions_in(text):
            if tok.search(clean, s, e):
                out.append((p, name, text[s:e]))
    return out


CAST = r"\(\s*(?:volatile\s+)?(s8|u8|s16|u16|s32|u32|int)\s*\*\s*\)\s*&[^;]*?(?:->|\.)\s*{}\b"


# ---------------------------------------------------------------- original code

MEM_OP = re.compile(r"^(ldr|str)(.*)$")
CONDS = "eq|ne|cs|hs|cc|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al"
WIDTH = re.compile(rf"^(?:{CONDS})?(sb|sh|b|h|d|t|bt)?(?:{CONDS})?$")
ADDR = re.compile(r"\[(r\d+|sb|sl|fp|ip|sp|lr)(?:,\s*#(-?(?:0x[0-9a-f]+|\d+)))?\](!?)(,\s*#?-?\w+)?")


def mem_access(mnemonic: str, operands: str) -> tuple[str, int, bool, str, int] | None:
    """(ldr|str, width, signed, base register, offset) of an immediate-offset
    load or store, or None. Post-indexed accesses read at offset 0."""
    m = MEM_OP.match(mnemonic)
    if not m:
        return None
    w = WIDTH.match(m.group(2))
    if not w:
        return None
    suffix = w.group(1) or ""
    a = ADDR.search(operands)
    if not a or "[pc" in operands or re.search(r"\[\w+,\s*-?r\d", operands):
        return None
    width = {"": 4, "t": 4, "b": 1, "bt": 1, "sb": 1, "h": 2, "sh": 2, "d": 8}[suffix]
    off = int(a.group(2), 0) if a.group(2) and not a.group(4) else 0
    return m.group(1), width, suffix.startswith("s"), a.group(1), off


@cache
def fn_index() -> fnindex.Index:
    return fnindex.load()


def original(name: str, path: Path) -> dict | None:
    rel = str(path.relative_to(ROOT))
    hits = fn_index().by_name.get(name, [])
    return next((f for f in hits if f["file"] == rel), hits[0] if hits else None)


def accesses(f: dict, lo: int, hi: int) -> list[tuple[int, str, str, tuple]]:
    """Loads and stores in f's original code at offsets lo..hi-1."""
    raw = bytes.fromhex(f["raw"])
    thumb = f["mode"] == "thumb"
    dis = fnindex.disassemble(raw, f["addr"], f["addr"], f["addr"] + f["pool"], thumb)
    out = []
    for addr in sorted(dis):
        mn, ops = dis[addr]
        acc = mem_access(mn, ops)
        if acc and lo <= acc[4] < hi:
            out.append((addr, mn, ops, acc))
    return out


# ---------------------------------------------------------------- symbols

ANY_SYM = re.compile(r"^(\S+) kind:(\w+)(\S*) addr:(0x[0-9a-f]+)")


@cache
def symbol_table() -> dict[str, list[tuple[int, str, str]]]:
    """module -> sorted (addr, name, kind)."""
    out = {}
    for name, d in fn.module_dirs():
        rows = []
        for line in (d / "symbols.txt").read_text().splitlines():
            m = ANY_SYM.match(line)
            if m:
                rows.append((int(m.group(4), 16), m.group(1), m.group(2)))
        out[name] = sorted(rows)
    return out


def find_symbol(key: str) -> list[tuple[str, int, str, str]]:
    want = None
    m = re.fullmatch(r"(?:data_|0x)?(?:ov\d{3}_)?([0-9a-fA-F]{8})", key)
    if m:
        want = int(m.group(1), 16)
    out = []
    for mod, rows in symbol_table().items():
        for addr, name, kind in rows:
            if name == key or (want is not None and addr == want and kind != "function"):
                out.append((mod, addr, name, kind))
    return out


def size_of(mod: str, addr: int) -> int | None:
    rows = symbol_table()[mod]
    i = bisect.bisect_right(rows, (addr, "\xff"))
    return rows[i][0] - addr if i < len(rows) else None


def name_of(value: int) -> str | None:
    for rows in symbol_table().values():
        i = bisect.bisect_left(rows, (value,))
        if i < len(rows) and rows[i][0] == value:
            return rows[i][1]
    return None


DEFINE = re.compile(r"^\s*#\s*define\s+(\w+)\s+\(?\s*(0x[0-9a-fA-F]+|\d+)\s*\)?\s*(?:/[*/].*)?$", re.M)


@cache
def constants() -> dict[str, int]:
    """Enum values from the index and integer #defines in include/."""
    out = {}
    for h in (ROOT / "include").rglob("*.h"):
        for name, v in DEFINE.findall(h.read_text(errors="replace")):
            out.setdefault(name, int(v, 0))
    for rec in index()["headers"].values():
        for s in rec["structs"]:
            for name, v in s.get("values", []):
                if v is not None:
                    out.setdefault(name, v)
    return out


def array_count(count) -> int | None:
    """An extern's array length: a number, a constant, or a sum or product
    of those. None for `[]` or anything else."""
    if isinstance(count, int) or not count:
        return count or None
    expr = re.sub(r"\b[A-Za-z_]\w*\b", lambda m: str(constants().get(m.group(0), "x")), count)
    if re.fullmatch(r"[\d\s+*()-]+", expr):
        return eval(expr)  # digits and + - * only
    return None


def declared_size(d: dict) -> int | None:
    size = type_size(d["type"])
    if size is None:
        return None
    if d["count"] is None:
        return size
    n = array_count(d["count"])
    return size * n if n else None


def containing(mod: str, addr: int) -> list[str]:
    """Symbols whose declared size covers addr (data_xxx inside gTable)."""
    out = []
    for a, name, kind in reversed(symbol_table()[mod]):
        if a >= addr:
            continue
        if addr - a > 0x10000:
            break
        for d in index()["externs"].get(name, []):
            size = declared_size(d)
            if size and a + size > addr:
                dim = "" if d["count"] is None else f"[{d['count']}]"
                out.append(f"{name}+{addr - a:#x} (declared {d['type']} {name}{dim} in {d['file']})")
                break
    return out


def references(mod: str, addr: int, size: int | None) -> tuple[list[str], list[str], list[str]]:
    """Functions that load the address, data that points at it, and
    relocations that reach inside it (Sym+N)."""
    loaders, tables, inside = set(), set(), set()
    end = addr + (size or 1)
    for m in fn.modules().values():
        for frm, kind, to, dest in m.relocs:
            if not (addr <= to < end) or mod not in fn.target_module(dest):
                continue
            g = next((g for g in m.funcs if g.addr <= frm < g.end), None)
            where = g.name if g else None
            if where is None:
                rows = symbol_table()[m.name]
                i = bisect.bisect_right(rows, (frm, "\xff")) - 1
                where = f"{rows[i][1]}+{frm - rows[i][0]:#x} ({m.name})" if i >= 0 else f"0x{frm:08x}"
            if to != addr:
                inside.add(f"+{to - addr:#x} {where}")
            elif g:
                loaders.add(where)
            else:
                tables.add(where)
    return sorted(loaders), sorted(tables), sorted(inside)


# ---------------------------------------------------------------- decode

def decode_scalar(data: bytes, off: int, size: int, signed: bool | None, typ: str) -> str:
    if off + size > len(data) or size not in (1, 2, 4):
        return "?"
    v = int.from_bytes(data[off:off + size], "little", signed=bool(signed))
    if typ.endswith("*") or "(*)" in typ:
        n = name_of(v) if v else None
        return f"0x{v:08x}" + (f" {n}" if n else "")
    return str(v) if abs(v) < 10 else f"{v} ({v & ((1 << 8 * size) - 1):#x})"


def decode(data: bytes, base: int, fields: list[dict], indent: str) -> list[str]:
    out = []
    for f in flatten(fields):
        off = base + f["offset"]
        if (f.get("struct") or f.get("members")) and "count" in f:
            es = f.get("elem_size") or 1
            vals = [data[off + i * es:off + (i + 1) * es].hex() for i in range(min(f["count"], 8))]
            more = " ..." if f["count"] > 8 else ""
            out.append(f"{indent}{f['offset']:#06x} {f['path']} = {{{', '.join(vals)}{more}}} (hex)")
            continue
        if f.get("struct") or f.get("members"):
            continue
        if f.get("bits"):
            lsb, width = f["bits"]
            word = int.from_bytes(data[off:off + f["size"]], "little")
            v = (word >> lsb) & ((1 << width) - 1)
            out.append(f"{indent}{f['offset']:#06x} {f['path']} = {v}")
        elif "count" in f:
            es = f.get("elem_size") or 1
            vals = [decode_scalar(data, off + i * es, es, f.get("elem_signed"), f["type"].split("[")[0])
                    for i in range(min(f["count"], 16))]
            more = " ..." if f["count"] > 16 else ""
            if f["type"].startswith("char["):
                raw = data[off:off + f["count"]]
                vals, more = [repr(raw.split(b"\0")[0].decode("latin-1"))], ""
            out.append(f"{indent}{f['offset']:#06x} {f['path']} = {{{', '.join(vals)}{more}}}")
        else:
            out.append(f"{indent}{f['offset']:#06x} {f['path']} = "
                       f"{decode_scalar(data, off, f['size'], f['signed'], f['type'])}")
    return out


def hexdump(data: bytes, addr: int, limit: int = 64) -> list[str]:
    return [f"  {addr + i:08x}  {data[i:i + 16].hex(' ')}" for i in range(0, min(len(data), limit), 16)]


# ---------------------------------------------------------------- commands

def field_line(f: dict) -> str:
    bits = f":{f['bits'][0]}+{f['bits'][1]}" if f.get("bits") else ""
    pad = "  " * f["depth"]
    return f"  {f['offset']:#06x} {f['size']:>5x}  {pad}{f['type']:<28} {f['path']}{bits}"


def show_struct(name: str, list_uses: bool) -> int:
    hits = structs(name)
    for header, s in hits:
        if s["kind"] == "enum":
            print(f"enum {name}  {header}  size {s['size']}")
            for k, v in s["values"]:
                print(f"  {v:>6}  {k}")
            continue
        flat = flatten(s["fields"], header=header)
        top = s["fields"]
        unk = sum(f["name"].startswith("unk_") for f in top)
        pad = sum(f["name"].startswith("pad_") for f in top)
        print(f"{s['kind']} {name}  {header}  size {s['size']:#x} ({s['size']})  "
              f"{len(top)} fields, {unk} unk_, {pad} pad_")
        decl = [(sym, d) for sym, ds in index()["externs"].items() for d in ds
                if re.sub(r"\b(const|volatile|struct|union)\b|\*", "", d["type"]).strip() == name]
        seen = set()
        decl = [x for x in decl if not (x[0] in seen or seen.add(x[0]))]
        print("data:    " + (", ".join(f"{sym} ({d['type']}{'[]' if d['count'] is not None else ''})"
                                       for sym, d in decl[:12]) + (" ..." if len(decl) > 12 else "")
                             if decl else "none declared"))
        files = files_including(header)
        counts = {}
        for f in s["fields"]:
            if f["name"]:
                tok = field_token(f["name"])
                counts[f["name"]] = sum(1 for _, t in files if tok.search(t))
        print(f"  offset  size  type{' ' * 25}name   (files using it, of {len(files)} that include the header)")
        for f in flat:
            n = counts.get(f["name"]) if f["depth"] == 0 else None
            print(field_line(f) + (f"   ({n})" if n else ""))
        gap_end = 0
        holes = []
        for f in sorted((f for f in flat if f["depth"] == 0), key=lambda f: f["offset"]):
            if f["offset"] > gap_end:
                holes.append(f"{gap_end:#x}-{f['offset']:#x}")
            gap_end = max(gap_end, f["offset"] + f["size"])
        if gap_end < s["size"] and s["kind"] == "struct":
            holes.append(f"{gap_end:#x}-{s['size']:#x}")
        if holes:
            print("padding: " + ", ".join(holes))
        if list_uses:
            for f in top:
                if not f["name"]:
                    continue
                us = uses(header, f["name"])
                if us:
                    print(f"  {f['name']}: " + ", ".join(sorted({u[1] for u in us})))
        if len(hits) > 1:
            print()
    return 0


def show_symbol(key: str, count: int) -> int:
    hits = find_symbol(key)
    if not hits:
        sys.exit(f"error: no struct or symbol {key} (structs come from include/, symbols from symbols.txt)")
    bins = None
    for mod, addr, name, kind in hits:
        size = size_of(mod, addr)
        print(f"{name}  {mod} 0x{addr:08x}  {kind}  size {f'{size:#x}' if size is not None else '?'} "
              f"(to the next symbol)")
        decl = index()["externs"].get(name, [])
        for d in decl[:4]:
            dim = "" if d["count"] is None else f"[{d['count']}]"
            print(f"type:    {d['type']} {name}{dim}  ({d['file']}:{d['line']})")
        if not decl:
            print("type:    not declared in include/ or src/")
        if len({d["type"] for d in decl}) > 1:
            print("note:    declared with different types")
        for c in containing(mod, addr):
            print(f"inside:  {c}")
        full = max((declared_size(d) or 0 for d in decl), default=0)
        if full:
            rows = symbol_table()[mod]
            within = [f"{n} (+{a - addr:#x})" for a, n, k in rows if addr < a < addr + full]
            print(f"declared size {full:#x}" + (f"; covers {len(within)} later symbols: "
                                               + ", ".join(within[:8]) + (" ..." if len(within) > 8 else "")
                                               if within else ""))
            size = full
        loaders, tables, inside = references(mod, addr, size)
        print("loaded:  " + (", ".join(loaders[:10]) + (" ..." if len(loaders) > 10 else "") or "by no function"))
        if tables:
            print("tables:  " + ", ".join(tables[:8]) + (" ..." if len(tables) > 8 else ""))
        if inside:
            print("inside refs: " + ", ".join(inside[:10]) + (" ..." if len(inside) > 10 else ""))
        if kind == "bss" or size is None:
            continue
        bins = bins or fnindex.module_bins()
        if mod not in bins:
            continue
        data, base = bins[mod]
        blob = data[addr - base:addr - base + size]
        typ = decl[0]["type"] if decl else ""
        hit = find_struct(typ) if typ and not typ.endswith("*") else None
        if hit and hit[1].get("fields") and hit[1]["size"]:
            esz = hit[1]["size"]
            n = len(blob) // esz
            print(f"bytes:   {len(blob)} = {n} x {typ} ({esz:#x})" + ("" if len(blob) % esz == 0
                                                                     else f" + {len(blob) % esz} left over"))
            for i in range(min(n, count)):
                print(f"  [{i}]")
                print("\n".join(decode(blob, i * esz, hit[1]["fields"], "    ")))
            if n > count:
                print(f"  ... {n - count} more (--count)")
        else:
            print("bytes:")
            print("\n".join(hexdump(blob, addr, max(64, count * 16))))
    return 0


def cmd_show(args) -> int:
    if structs(args.key):
        return show_struct(args.key, args.uses)
    return show_symbol(args.key, args.count)


def cmd_field(args) -> int:
    if "." not in args.key:
        sys.exit("error: give STRUCT.FIELD")
    sname, path = args.key.split(".", 1)
    hits = structs(sname)
    if not hits:
        sys.exit(f"error: no struct {sname} in include/")
    for header, s in hits:
        flat = flatten(s.get("fields", []), header=header)
        f = next((x for x in flat if x["path"] == path), None)
        if f is None:
            names = ", ".join(x["path"] for x in flat if x["depth"] == 0)
            print(f"{sname} ({header}) has no field {path}; fields: {names}")
            continue
        es = f.get("elem_size") or f["size"]
        signed = f.get("elem_signed") if "count" in f else f["signed"]
        print(f"{sname}.{path}  {header}  offset {f['offset']:#x}  size {f['size']:#x}  type {f['type']}"
              + (f"  bits {f['bits'][0]}+{f['bits'][1]}" if f.get("bits") else ""))
        us = uses(header, path)
        cast = re.compile(CAST.format(re.escape(path.split(".")[-1])))
        tally = {"signed": 0, "unsigned": 0, "word": 0, "store": 0, "other": 0}
        casts = []
        for p, name, body in us:
            rel = p.relative_to(ROOT)
            for m in cast.finditer(body):
                if m.group(1) != f["type"]:
                    casts.append(f"{rel}: {name}: ({m.group(1)} *)")
            orig = original(name, p)
            if orig is None:
                print(f"  {name:<36} {rel}  (not in the function index)")
                continue
            acc = accesses(orig, f["offset"], f["offset"] + max(f["size"], 1))
            shown = []
            for addr, mn, ops, (op, width, sgn, reg, off) in acc:
                if op == "str":
                    tally["store"] += 1
                elif width != es and not f.get("bits"):
                    tally["other"] += 1
                elif width == 4:
                    tally["word"] += 1
                else:
                    tally["signed" if sgn else "unsigned"] += 1
                shown.append(mn if off == f["offset"] else f"{mn} +{off - f['offset']:#x}")
            ops = ", ".join(f"{k} x{shown.count(k)}" if shown.count(k) > 1 else k for k in dict.fromkeys(shown))
            print(f"  {name:<36} {orig['module']} 0x{orig['addr']:08x}  {rel}: "
                  f"{ops or 'no access at this offset in the original'}")
        if not us:
            print("  no function in src/ names this field")
        for c in casts:
            print(f"  cast: {c}")
        loads = tally["signed"] + tally["unsigned"]
        summary = ", ".join(f"{v} {k}" for k, v in tally.items() if v)
        print(f"accesses: {summary or 'none found'}")
        if loads and es in (1, 2) and signed is not None and not f.get("bits"):
            if not signed and tally["signed"] == loads:
                print(f"check:   declared unsigned; every load is signed (ldrs{'b' if es == 1 else 'h'})")
            elif signed and tally["unsigned"] == loads:
                print(f"check:   declared signed; every load is unsigned (ldr{'b' if es == 1 else 'h'})")
            elif tally["signed"] and tally["unsigned"]:
                print("check:   mixed signed and unsigned loads")
            else:
                print("check:   loads agree with the declared type")
        if casts:
            print("check:   our C casts this field to another type; check its declared type")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("show")
    p.add_argument("key")
    p.add_argument("--uses", action="store_true", help="list the functions that touch each field")
    p.add_argument("--count", type=int, default=4, help="entries of a table to decode (default 4)")
    p.set_defaults(func=cmd_show)
    p = sub.add_parser("field")
    p.add_argument("key", help="STRUCT.FIELD, with dots for embedded structs")
    p.set_defaults(func=cmd_field)
    args = ap.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
