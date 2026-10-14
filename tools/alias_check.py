#!/usr/bin/env python3
"""Check each alias in config/AWRE/aliases.txt against the C type of its base.

    alias_check.py             check every `real` alias; exit 1 on a difference
    alias_check.py --list      also print the aliases the C type does not give

A `real FIELD` alias (docs/bss-tool.md, "Aliases") writes an address as
BASE+OFFSET. FIELD names the place in BASE: a path of elements and fields
(`[64]`, `[0][1]`, `[0].pictures[2]`, `records`, `coRecords[0].setMode1`),
then an optional byte count (`+0x3c`). Text after the first space is a note.

For every base, the file that defines it (from the link maps) is compiled
again with mwccarm with one line per alias added:

    void *const alias_probe_3 = (void *)((char *)&(BASE[0].pictures[2]) + 0);

The relocation of each line gives BASE plus the offset the C type puts the
field at, which must equal OFFSET. A FIELD that does not compile is an
error too. So when a struct grows, or a field moves, the check names every
alias that went wrong (issue 0086, section 4).

An alias counts as computed from C when FIELD has a path and no byte count.
The others (only a byte count such as a compiler base `+0x1000`, or a path
plus bytes) are checked for the path part only; `--list` prints them,
and tools/shift_gaps.py counts them as the `alias` row.

Results are kept in build/alias_check.json, keyed by this tool, include/,
build/gen/, the defining file, its object in build/ and the probe lines, so
a run after an unchanged build compiles nothing. Needs a build (`make`).
"""

import argparse
import hashlib
import json
import re
import struct
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

import configure  # noqa: E402

ALIAS_LIST = ROOT / "config/AWRE/aliases.txt"
CACHE = ROOT / "build/alias_check.json"
MAPS = ((ROOT / "build/arm9.elf.xMAP", ROOT / "build/objects.txt"),
        (ROOT / "build/arm7/arm7.elf.xMAP", ROOT / "build/arm7/objects.txt"))
LINE = re.compile(r"(\w+) (\w+) = (\w+)\+(0x[0-9a-f]+) (unsorted|hidden|real (\S+).*)")
FIELD = re.compile(r"((?:\.?[A-Za-z_]\w*|\[[^\]\s]+\])*)(?:\+(0x[0-9a-fA-F]+|\d+))?")
MAP_ENTRY = re.compile(r"^\s+[0-9A-F]{8} [0-9A-F]{8} \.[\w.]+\s+(\w+)\t\((\S+)\.o\)", re.M)


@dataclass
class Alias:
    module: str
    name: str
    base: str
    offset: int
    line: int
    field: str | None  # None for a group other than `real`
    path: str = ""     # the element and field part of FIELD, as C
    extra: int = 0     # the byte count after it

    @property
    def from_c(self) -> bool:
        """The C type gives the whole offset."""
        return bool(self.path) and self.extra == 0


def read_aliases(path: Path = ALIAS_LIST) -> tuple[list[Alias], list[str]]:
    """The aliases, and the lines whose FIELD cannot be read."""
    out, errors = [], []
    for n, raw in enumerate(path.read_text().splitlines(), 1):
        if not raw.strip() or raw.startswith("#"):
            continue
        m = LINE.fullmatch(raw.strip())
        if not m:
            errors.append(f"{path.name}:{n}: cannot read the line")
            continue
        a = Alias(m[1], m[2], m[3], int(m[4], 16), n, m[6])
        if a.field is not None:
            f = FIELD.fullmatch(a.field)
            if not f:
                errors.append(f"{path.name}:{n}: {a.name}: FIELD `{a.field}` is no path of elements and fields")
                a.field = None
            else:
                p = f[1]
                a.path = p if not p or p[0] in ".[" else "." + p
                a.extra = int(f[2], 0) if f[2] else 0
        out.append(a)
    return out, errors


def definers() -> dict[str, str]:
    """{symbol: source file} for every symbol the link maps place."""
    out = {}
    for xmap, objects in MAPS:
        if not xmap.exists():
            continue
        by_stem: dict[str, list[str]] = {}
        for o in objects.read_text().split():
            o = o.strip('"')
            if "/src/" in o:
                src = "src/" + o.split("/src/", 1)[1].removesuffix(".o") + ".c"
                by_stem.setdefault(Path(o).stem, []).append(src)
        for sym, stem in MAP_ENTRY.findall(xmap.read_text(errors="replace")):
            srcs = by_stem.get(stem, [])
            if len(srcs) == 1:
                out.setdefault(sym, srcs[0])
    return out


def probe_line(i: int, expr: str) -> str:
    return f"void *const alias_probe_{i} = (void *)({expr});\n"


def compile_relocs(src: str, text: str) -> tuple[dict[str, tuple[str, int]] | None, str]:
    """{probe symbol: (target, addend)} of `text` compiled as `src`, or None
    and the compiler's message."""
    cc, flags = configure.compiler_for(src)
    # The layout does not depend on the compiler version; 2.0-72 runs natively.
    if cc.endswith(".py"):
        cc = configure.CC
    flags = flags.replace("-i include", f"-i include -i {Path(src).parent}")
    with tempfile.TemporaryDirectory(prefix="alias_check") as d:
        c, obj = Path(d) / "probe.c", Path(d) / "probe.o"
        c.write_text(text)
        r = subprocess.run([str(ROOT / cc), "-c", *flags.split(), "-o", str(obj), str(c)], cwd=ROOT,
                           capture_output=True, text=True)
        if r.returncode != 0 or not obj.exists():
            msg = [ln.rsplit("\\", 1)[-1] for ln in (r.stdout + r.stderr).splitlines()
                   if ln.strip() and "Errors caused tool to abort" not in ln]
            text = re.sub(r"(?<=.) ?probe\.c:\d+: ", " ", " ".join(" ".join(msg[:2]).split()))
            return None, text[:300] or "does not compile"
        return elf_relocs(obj.read_bytes()), ""


def elf_relocs(d: bytes) -> dict[str, tuple[str, int]]:
    """{symbol: (target, addend)} for every object symbol whose first word
    has a relocation (REL or RELA), in a 32-bit little-endian ELF file."""
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", d, 0x2e)
    sh = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    symtab = next(i for i, s in enumerate(sh) if s[1] == 2)
    strings = sh[sh[symtab][6]][4]

    def cstr(off: int) -> str:
        return d[off:d.index(b"\0", off)].decode()

    syms = []
    for j in range(sh[symtab][5] // 16):
        name, value, size, info, _, shndx = struct.unpack_from("<IIIBBH", d, sh[symtab][4] + j * 16)
        syms.append((cstr(strings + name), value, shndx))
    by_place = {(shndx, value): name for name, value, shndx in syms if name.startswith("alias_probe_")}
    out = {}
    for s in sh:
        if s[1] not in (4, 9):  # SHT_RELA, SHT_REL
            continue
        target_sec, size = s[7], 12 if s[1] == 4 else 8
        for j in range(s[5] // size):
            r_off, r_info = struct.unpack_from("<II", d, s[4] + j * size)
            probe = by_place.get((target_sec, r_off))
            if probe is None:
                continue
            if s[1] == 4:
                addend, = struct.unpack_from("<i", d, s[4] + j * size + 8)
            else:
                addend, = struct.unpack_from("<i", d, sh[target_sec][4] + r_off)
            tname, _, tshndx = syms[r_info >> 8]
            out[probe] = (tname or f"section {tshndx}", addend)
    return out


def check_file(src: str, aliases: list[Alias]) -> dict[str, dict]:
    """{alias: {"offset": N} or {"error": text}} for the aliases whose base
    `src` defines."""
    text = (ROOT / src).read_text(errors="replace") + "\n"
    exprs = [f"(char *)&({a.base}{a.path})" for a in aliases]
    bases = sorted({a.base for a in aliases})
    lines = "".join(probe_line(f"b{k}", f"&{b}") for k, b in enumerate(bases))
    lines += "".join(probe_line(i, e) for i, e in enumerate(exprs))
    relocs, msg = compile_relocs(src, text + lines)
    out = {}
    if relocs is None:
        if len(aliases) == 1:
            return {aliases[0].name: {"error": msg}}
        for a in aliases:
            out.update(check_file(src, [a]))
        return out
    for i, a in enumerate(aliases):
        base = relocs.get(f"alias_probe_b{bases.index(a.base)}")
        got = relocs.get(f"alias_probe_{i}")
        if base is None or got is None or base[0] != got[0]:
            out[a.name] = {"error": f"no relocation against {a.base} for {a.base}{a.path}"}
        else:
            out[a.name] = {"offset": got[1] - base[1]}
    return out


def compute(aliases: list[Alias], use_cache: bool = True) -> dict[str, dict]:
    """{alias name: {"offset": N} or {"error": text}} for the `real` aliases."""
    where = definers()
    groups: dict[str, list[Alias]] = {}
    out: dict[str, dict] = {}
    for a in aliases:
        if a.field is None:
            continue
        src = where.get(a.base)
        if src is None or not (ROOT / src).exists():
            out[a.name] = {"error": f"no C file defines {a.base} (run `make`?)"}
        else:
            groups.setdefault(src, []).append(a)
    cache = {}
    if use_cache and CACHE.exists():
        try:
            cache = json.loads(CACHE.read_text())
        except ValueError:
            cache = {}

    # Every header and generated file: a header change that leaves the
    # object as it was (a pad split into fields) can still move an alias.
    common = hashlib.sha256(Path(__file__).read_bytes())
    for d in ("include", "build/gen"):
        for f in sorted((ROOT / d).rglob("*")):
            if f.is_file():
                common.update(f.relative_to(ROOT).as_posix().encode() + b"\0" + f.read_bytes())

    def key(src: str, group: list[Alias]) -> str:
        h = common.copy()
        h.update((ROOT / src).read_bytes())
        obj = ROOT / configure.obj_of(src)
        h.update(obj.read_bytes() if obj.exists() else b"")
        h.update("\n".join(f"{a.name} {a.base}{a.path}" for a in group).encode())
        return h.hexdigest()

    keys = {src: key(src, g) for src, g in groups.items()}
    todo = [src for src in groups if cache.get(src, {}).get("key") != keys[src]]
    with ThreadPoolExecutor() as pool:
        for src, res in zip(todo, pool.map(lambda s: check_file(s, groups[s]), todo)):
            cache[src] = {"key": keys[src], "results": res}
    for src in groups:
        out.update(cache[src]["results"])
    if use_cache and CACHE.parent.exists():
        new = json.dumps({s: cache[s] for s in sorted(groups)}, indent=1, sort_keys=True) + "\n"
        if not CACHE.exists() or CACHE.read_text() != new:
            CACHE.write_text(new)
    return out


def check(aliases: list[Alias], results: dict[str, dict]) -> list[str]:
    """One line per alias whose FIELD does not compile or gives another offset."""
    errors = []
    for a in aliases:
        r = results.get(a.name)
        if r is None:
            continue
        where = f"aliases.txt:{a.line}: {a.name} = {a.base}+{a.offset:#x} `{a.field}`"
        if "error" in r:
            errors.append(f"{where}: {r['error']}")
        elif r["offset"] + a.extra != a.offset:
            errors.append(f"{where}: the C type puts it at {a.base}+{r['offset'] + a.extra:#x}")
    return errors


def not_from_c(aliases: list[Alias]) -> list[Alias]:
    """The aliases whose offset the C type does not give in full."""
    return [a for a in aliases if not a.from_c]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--list", action="store_true", help="print the aliases not computed from C")
    ap.add_argument("--no-cache", action="store_true")
    args = ap.parse_args()
    aliases, errors = read_aliases()
    errors += check(aliases, compute(aliases, use_cache=not args.no_cache))
    if args.list:
        for a in not_from_c(aliases):
            print(f"aliases.txt:{a.line}: {a.name} = {a.base}+{a.offset:#x} {a.field or 'not real'}")
    if errors:
        print("alias_check: aliases whose FIELD and C type disagree:\n  " + "\n  ".join(errors))
        return 1
    rest = len(not_from_c(aliases))
    print(f"alias_check: {len(aliases)} aliases, {len(aliases) - rest} computed from C, {rest} not")
    return 0


if __name__ == "__main__":
    sys.exit(main())
