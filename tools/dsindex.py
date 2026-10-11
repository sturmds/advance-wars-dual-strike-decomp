#!/usr/bin/env python3
"""Struct index for tools/ds.py (issue 0041).

    dsindex.py [--jobs N]          build or refresh build/dsindex.json

Layouts come from the compiler, not from parsing C: for each header in
include/, a probe file includes it and declares one pointer per type the
header defines, and mwccarm compiles it with -g. The DWARF in the object
(read with `objdump --dwarf=info`) gives every struct's size and each
member's offset, type, signedness and bit-field position, exactly as the
build lays them out. One probe per header, because some headers declare the
same symbol with different types and cannot share a translation unit.

Probes are cached by a hash of the header, the headers it includes and the
compiler command, so a refresh recompiles only what changed. The `extern`
declarations in include/ and src/ are read with a regex; they give each
data symbol its declared type.

Never runs ninja or dsd. Writes only build/dsindex.json, and probe files
in build/dsindex/ that it deletes after each compile.
"""

import argparse
import hashlib
import json
import re
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import configure  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
INCLUDE = ROOT / "include"
INDEX = ROOT / "build/dsindex.json"
PROBES = ROOT / "build/dsindex/probe"
VERSION = 1

# Type names a header defines: `} Name;` closing a top-level typedef,
# `struct Tag {`, and `typedef struct Tag Name;` (opaque, probed but usually
# empty).
COMMENT = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)
TYPEDEF_CLOSE = re.compile(r"\}\s*(\w+)\s*;")
TAG_OPEN = re.compile(r"^\s*(?:typedef\s+)?(struct|union|enum)\s+(\w+)\s*\{", re.M)
TYPEDEF_TAG = re.compile(r"^\s*typedef\s+(?:struct|union|enum)\s+\w+\s+(\w+)\s*;", re.M)
INCLUDE_LINE = re.compile(r'^\s*#\s*include\s+"([^"]+)"', re.M)
# Top-level `extern TYPE NAME;` or `extern TYPE NAME[N];`. Function
# prototypes and function pointers have a `(` and are skipped.
EXTERN = re.compile(r"^extern\s+([^;()]*?)\s*\b(\w+)\s*(\[[^\]]*\])?\s*;", re.M)

BASE_NAMES = {
    "unsigned char": "u8", "signed char": "s8", "char": "char",
    "unsigned short": "u16", "short": "s16", "signed short": "s16",
    "unsigned int": "u32", "int": "s32", "signed int": "s32",
    "unsigned long": "u32", "long": "s32",
    "unsigned long long": "u64", "long long": "s64",
}


def type_names(text: str) -> list[str]:
    """C type names to probe, as they can be written after the #include."""
    text = COMMENT.sub(" ", text)
    names: dict[str, None] = {}
    depth = 0
    for i, c in enumerate(text):
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            m = TYPEDEF_CLOSE.match(text, i)
            if depth == 0 and m:
                names.setdefault(m.group(1))
    for kind, tag in TAG_OPEN.findall(text):
        names.setdefault(f"{kind} {tag}")
    names.update(dict.fromkeys(TYPEDEF_TAG.findall(text)))
    return list(names)


def resolve_include(name: str, src: Path) -> Path | None:
    for base in (INCLUDE, src.parent):
        p = base / name
        if p.exists():
            return p
    return None


def include_closure(header: Path) -> list[Path]:
    seen, todo = [], [header]
    while todo:
        h = todo.pop()
        if h in seen:
            continue
        seen.append(h)
        for name in INCLUDE_LINE.findall(h.read_text(errors="replace")):
            p = resolve_include(name, h)
            if p:
                todo.append(p)
    return seen


def cache_key(header: Path) -> str:
    # This file is part of the key: a change to the probe text or the DWARF
    # reader redoes every probe.
    h = hashlib.sha1(f"{configure.CC} {configure.CFLAGS}\n".encode() + Path(__file__).read_bytes())
    for p in sorted(include_closure(header)):
        h.update(str(p.relative_to(ROOT)).encode() + b"\0" + p.read_bytes() + b"\0")
    return h.hexdigest()[:16]


# ---------------------------------------------------------------- DWARF

DIE = re.compile(r"^\s*<(\d+)><([0-9a-f]+)>: Abbrev Number: (\d+)(?: \((DW_TAG_\w+)\))?")
ATTR = re.compile(r"^\s*<[0-9a-f]+>\s+(DW_AT_\w+)\s*:\s*(.*?)\s*$")
REF = re.compile(r"<0x([0-9a-f]+)>")
LOC = re.compile(r"\(DW_OP_(?:const4u|plus_uconst|constu): (\d+)\)")


def parse_dies(text: str) -> dict[int, dict]:
    """objdump --dwarf=info text -> {offset: {tag, attrs, children, parent}}."""
    dies: dict[int, dict] = {}
    stack: list[int] = []
    cur = None
    for line in text.splitlines():
        m = DIE.match(line)
        if m:
            depth, off, tag = int(m.group(1)), int(m.group(2), 16), m.group(4)
            del stack[depth:]
            if tag is None:  # Abbrev Number: 0 closes a list of children
                cur = None
                continue
            cur = {"tag": tag[7:], "attrs": {}, "children": [], "parent": stack[-1] if stack else None}
            dies[off] = cur
            if stack:
                dies[stack[-1]]["children"].append(off)
            stack.append(off)
            continue
        m = ATTR.match(line)
        if m and cur is not None:
            cur["attrs"][m.group(1)[6:]] = m.group(2)
    return dies


def _int(v: str | None) -> int | None:
    if v is None:
        return None
    m = re.match(r"(0x[0-9a-f]+|\d+)", v)
    return int(m.group(1), 0) if m else None


class Types:
    """Type names, sizes and struct layouts from parsed DIEs."""

    def __init__(self, dies: dict[int, dict]):
        self.dies = dies
        # An anonymous struct takes the name of a typedef that points at it.
        self.alias: dict[int, str] = {}
        for d in dies.values():
            if d["tag"] == "typedef":
                t = self.ref(d)
                if t is not None and t in dies and "name" not in dies[t]["attrs"]:
                    self.alias.setdefault(t, d["attrs"]["name"])

    def ref(self, d: dict) -> int | None:
        m = REF.search(d["attrs"].get("type", ""))
        return int(m.group(1), 16) if m else None

    def strip(self, off: int | None) -> int | None:
        """Follow typedefs and qualifiers to the underlying type."""
        while off is not None and self.dies[off]["tag"] in ("typedef", "volatile_type", "const_type"):
            off = self.ref(self.dies[off])
        return off

    def name(self, off: int | None) -> str:
        if off is None:
            return "void"
        d = self.dies[off]
        tag, a = d["tag"], d["attrs"]
        if tag == "base_type":
            return BASE_NAMES.get(a.get("name"), a.get("name", "?"))
        if tag == "typedef":
            return a["name"]
        if tag in ("structure_type", "union_type", "enumeration_type"):
            kind = {"structure_type": "struct", "union_type": "union"}.get(tag, "enum")
            return a.get("name") or self.alias.get(off) or f"<anon {kind}>"
        if tag == "pointer_type":
            t = self.ref(d)
            if t is not None and self.dies[t]["tag"] == "subroutine_type":
                return self.func_name(t, "(*)")
            return self.name(t) + " *"
        if tag == "array_type":
            dims, t = [], off
            while t is not None and self.dies[t]["tag"] == "array_type":
                for c in self.dies[t]["children"]:
                    ub = _int(self.dies[c]["attrs"].get("upper_bound"))
                    dims.append("" if ub is None else str(ub + 1))
                t = self.ref(self.dies[t])
            return self.name(t) + "".join(f"[{n}]" for n in dims)
        if tag == "volatile_type":
            return "volatile " + self.name(self.ref(d))
        if tag == "const_type":
            return "const " + self.name(self.ref(d))
        if tag == "subroutine_type":
            return self.func_name(off, "")
        return f"<{tag}>"

    def func_name(self, off: int, mid: str) -> str:
        d = self.dies[off]
        args = [self.name(self.ref(self.dies[c])) for c in d["children"]
                if self.dies[c]["tag"] == "formal_parameter"]
        return f"{self.name(self.ref(d))} {mid}({', '.join(args) or 'void'})"

    def size(self, off: int | None) -> int:
        if off is None:
            return 0
        d = self.dies[off]
        if d["tag"] == "pointer_type":
            return 4
        if "byte_size" in d["attrs"]:
            return _int(d["attrs"]["byte_size"]) or 0
        if d["tag"] in ("typedef", "volatile_type", "const_type"):
            return self.size(self.ref(d))
        if d["tag"] == "array_type":
            n = 1
            for c in d["children"]:
                ub = _int(self.dies[c]["attrs"].get("upper_bound"))
                n *= 0 if ub is None else ub + 1
            return n * self.size(self.ref(d))
        return 0

    def signed(self, off: int | None) -> bool | None:
        """Signedness of a scalar: True, False, or None for anything else."""
        off = self.strip(off)
        if off is None:
            return None
        d = self.dies[off]
        if d["tag"] == "base_type":
            enc = _int(d["attrs"].get("encoding"))
            return {5: True, 6: True, 7: False, 8: False}.get(enc)
        if d["tag"] == "enumeration_type":
            return False
        if d["tag"] == "pointer_type":
            return False
        return None

    def element(self, off: int | None) -> tuple[int | None, int]:
        """(element type, count) of an array; (type, 1) for anything else."""
        s = self.strip(off)
        if s is None or self.dies[s]["tag"] != "array_type":
            return off, 1
        n = 1
        t = s
        while t is not None and self.dies[t]["tag"] == "array_type":
            for c in self.dies[t]["children"]:
                ub = _int(self.dies[c]["attrs"].get("upper_bound"))
                n *= 0 if ub is None else ub + 1
            t = self.strip(self.ref(self.dies[t]))
        return t, n

    def aggregate(self, off: int) -> dict:
        """Struct, union or enum record: name, kind, size, fields or values."""
        d = self.dies[off]
        kind = {"structure_type": "struct", "union_type": "union"}.get(d["tag"], "enum")
        rec = {"name": self.name(off), "kind": kind, "size": self.size(off)}
        if kind == "enum":
            rec["values"] = [[self.dies[c]["attrs"]["name"], _int(self.dies[c]["attrs"].get("const_value"))]
                             for c in d["children"]]
            return rec
        fields = [self.member(c) for c in d["children"] if self.dies[c]["tag"] == "member"]
        # mwccarm lists an unnamed struct or union member as `<anon>` and
        # repeats its members beside it; keep only the repeated copies.
        named = {f["name"] for f in fields}
        rec["fields"] = [f for f in fields if not (f["name"] == "<anon>" and f.get("members")
                                                   and all(m["name"] in named for m in f["members"]))]
        for f in rec["fields"]:
            if f["name"] == "<anon>":
                f["name"] = ""
        return rec

    def member(self, off: int) -> dict:
        d = self.dies[off]
        a = d["attrs"]
        t = self.ref(d)
        m = LOC.search(a.get("data_member_location", ""))
        field = {
            "name": a.get("name", ""),
            "offset": int(m.group(1)) if m else 0,
            "size": self.size(t),
            "type": self.name(t),
            "signed": self.signed(t),
        }
        if "bit_size" in a:
            width, size = _int(a["bit_size"]), _int(a.get("byte_size")) or field["size"]
            # DWARF 2 counts bit_offset from the most significant bit.
            field["bits"] = [8 * size - _int(a["bit_offset"]) - width, width]
        elem, count = self.element(t)
        if count > 1 or field["type"].endswith("]"):
            field["count"] = count
            field["elem_size"] = self.size(elem)
            field["elem_signed"] = self.signed(elem)
        s = self.strip(elem)
        if s is not None and self.dies[s]["tag"] in ("structure_type", "union_type"):
            if "name" in self.dies[s]["attrs"] or s in self.alias:
                field["struct"] = self.name(s)
            else:  # anonymous: keep its members inline
                field["members"] = self.aggregate(s)["fields"]
        return field


def dwarf_types(objdump_text: str) -> list[dict]:
    """Every named struct, union and enum in an object's DWARF."""
    types = Types(parse_dies(objdump_text))
    out = []
    for off, d in types.dies.items():
        if d["tag"] in ("structure_type", "union_type", "enumeration_type"):
            if "name" in d["attrs"] or off in types.alias:
                rec = types.aggregate(off)
                if rec["size"] or rec.get("fields") or rec.get("values"):
                    out.append(rec)
    return out


# ---------------------------------------------------------------- probes

def compile_probe(src: Path, obj: Path) -> tuple[str, str]:
    """Compile src with -g; return (objdump --dwarf=info text, error text)."""
    cmd = [str(ROOT / configure.CC), "-c", *configure.CFLAGS.split(), "-g", "-o", str(obj), str(src)]
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    if r.returncode != 0 or not obj.exists():
        return "", (r.stdout + r.stderr).strip()
    d = subprocess.run(["arm-none-eabi-objdump", "--dwarf=info", str(obj)],
                       capture_output=True, text=True, check=True)
    return d.stdout, ""


def probe_header(header: Path, key: str) -> dict:
    rel = header.relative_to(INCLUDE)
    names = type_names(header.read_text(errors="replace"))
    # types.h first: a few headers use u8 and friends without including it.
    lines = ['#include "types.h"', f'#include "{rel.as_posix()}"']
    lines += [f"{n} *ds_probe_{i};" for i, n in enumerate(names)]
    PROBES.mkdir(parents=True, exist_ok=True)
    src, obj = PROBES / f"{key}.c", PROBES / f"{key}.o"
    src.write_text("\n".join(lines) + "\n")
    text, err = compile_probe(src, obj)
    obj.unlink(missing_ok=True)
    src.unlink(missing_ok=True)
    if err:
        return {"key": key, "structs": [], "error": err}
    wanted = {n.split()[-1] for n in names}
    structs = [s for s in dwarf_types(text) if s["name"] in wanted]
    return {"key": key, "structs": structs, "error": ""}


def externs() -> dict[str, list[dict]]:
    out: dict[str, list[dict]] = {}
    files = sorted(INCLUDE.rglob("*.h")) + sorted((ROOT / "src").rglob("*.c"))
    for p in files:
        text = p.read_text(errors="replace")
        for m in EXTERN.finditer(text):
            typ, name, dim = m.group(1), m.group(2), m.group(3)
            typ = re.sub(r"\s+", " ", typ.replace("*", " * ")).strip().replace(" *", "*").replace("*", " *")
            count = None
            if dim:
                inner = dim[1:-1].strip()
                count = int(inner, 0) if re.fullmatch(r"0x[0-9a-fA-F]+|\d+", inner) else (inner or "")
            line = text.count("\n", 0, m.start()) + 1
            out.setdefault(name, []).append({"type": typ, "count": count,
                                             "file": str(p.relative_to(ROOT)), "line": line})
    return out


def build(jobs: int = 2, old: dict | None = None) -> dict:
    old_headers = (old or {}).get("headers", {})
    headers = sorted(INCLUDE.rglob("*.h"))
    keys = {h: cache_key(h) for h in headers}
    result: dict[str, dict] = {}
    todo = []
    for h in headers:
        rel = str(h.relative_to(ROOT))
        prev = old_headers.get(rel)
        if prev and prev["key"] == keys[h]:
            result[rel] = prev
        else:
            todo.append(h)
    with ThreadPoolExecutor(max_workers=max(1, jobs)) as ex:
        for h, rec in zip(todo, ex.map(lambda h: probe_header(h, keys[h]), todo)):
            result[str(h.relative_to(ROOT))] = rec
    return {"version": VERSION, "headers": result, "externs": externs(), "probed": len(todo)}


def write(path: Path = INDEX, jobs: int = 2) -> dict:
    old = None
    if path.exists():
        try:
            old = json.loads(path.read_text())
            if old.get("version") != VERSION:
                old = None
        except ValueError:
            old = None
    index = build(jobs, old)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(".tmp")
    tmp.write_text(json.dumps(index, separators=(",", ":")))
    tmp.replace(path)
    return index


def stale(path: Path = INDEX) -> bool:
    """True when a header is newer than the index (src/ externs included)."""
    if not path.exists():
        return True
    t = path.stat().st_mtime
    return any(p.stat().st_mtime > t for p in INCLUDE.rglob("*.h")) or \
        any(p.stat().st_mtime > t for p in (ROOT / "src").rglob("*.c"))


def load(path: Path = INDEX, jobs: int = 2) -> dict:
    if stale(path):
        t = time.time()
        index = write(path, jobs)
        print(f"dsindex: refreshed {path.relative_to(ROOT)} ({index['probed']} probes, "
              f"{time.time() - t:.1f} s)", file=sys.stderr)
        return index
    index = json.loads(path.read_text())
    if index.get("version") != VERSION:
        return write(path, jobs)
    return index


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--jobs", type=int, default=2, help="parallel probe compiles (default 2)")
    ap.add_argument("--out", type=Path, default=INDEX)
    args = ap.parse_args()
    t = time.time()
    index = write(args.out, args.jobs)
    structs = sum(len(h["structs"]) for h in index["headers"].values())
    errors = [p for p, h in index["headers"].items() if h["error"]]
    print(f"dsindex: {len(index['headers'])} headers ({index['probed']} probed), {structs} types, "
          f"{len(index['externs'])} externs -> {args.out.relative_to(ROOT)} ({time.time() - t:.1f} s)")
    for p in errors:
        first = index["headers"][p]["error"].splitlines()[0]
        print(f"  {p}: probe failed: {first}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
