#!/usr/bin/env python3
"""Count what still stops the source from being fully shiftable (issue 0087).

    shift_gaps.py                 the counts, as the Markdown PROGRESS.md shows
    shift_gaps.py --list KIND     one line per site of that kind (file:line: text)

Fully shiftable (issue 0086, decided by sturmds): no address, layout value or
size written as a number; every size, copy length, stride, offset and bound
comes from the structure (`sizeof`, fields, `offsetof`, element counts), so
that growing a function, a struct or a table rebuilds a working ROM. Each
kind below counts places where the source still writes such a number. Most
kinds will not reach 0: a hardware format (a 32-byte palette, a 0x800-byte
tile block) is a number on purpose. Issue 0087 says how each kind ends.

Source kinds read src/ and include/ with comments taken out, split into game
code (src/game/, include/game/) and SDK/library code (everything else):

  alloc     Mem_Alloc with a literal size
  copy      a copy, fill or clear call (Mem_Copy*, Mem_Fill*, the SDK's
            MI CPU and DMA functions, memcpy, memset) whose last argument,
            the length, is a literal
  offset    a byte offset into an object: `(u8 *)x + 0x10`, `(char *)x + 3`
  loop      a `for` loop whose condition compares with a literal other than 0
  array_u32, array_char, array_other
            an initialised array whose length is a literal (`u32 x[12] = {`);
            `[]` takes the length from the initializer
  extern    an extern array with a literal length
  placeholder
            a zeroed `u8 NAME[N];` definition standing for a variable whose
            type is not written, in src/ and in build/gen/bss/ (bss.py gen)

Kinds from the config and the build:

  alias     an alias in config/AWRE/aliases.txt whose offset the C type of
            its base does not give: a FIELD that is a byte count (`+0x1000`,
            a compiler base or a flat table) or ends in one
            (tools/alias_check.py, which `make release` runs, checks the rest)
  mixed_u32 a u32 table whose ROM values (extracted/AWRE/data) hold both
            pointers and numbers: a struct written as words
  scan_*    words in data and literal pools that look like addresses and have
            no relocation (tools/scan_pointers.py), by level, left out when
            shift_numbers.txt lists them or the C type makes them numbers;
            low is mostly words of u32 tables written from the ROM bytes

Also counted, as the measure that grows: `size_assert`, the structs whose
size a `typedef char X_size_check[...]` checks.

Needs a build for the scan and mixed_u32 rows (`make`; it runs
tools/fnindex.py when build/fnindex.json is missing). Without one those rows
show "-". The same tree gives the same counts.
"""

import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TOOLS = ROOT / "tools"
sys.path.insert(0, str(TOOLS))

import alias_check  # noqa: E402

NUM = r"(?:0x[0-9a-fA-F]+|\d+)[uUlL]*"
LITERAL = re.compile(rf"^\(?\s*{NUM}(?:\s*[-+*/|<>]+\s*{NUM})*\s*\)?$")
COPY = re.compile(r"\b(Mem_(?:Copy|Fill|Clear)\w*|MIi?_(?:Cpu|Dma)\w+|memcpy|memset)\s*\(")
ALLOC = re.compile(r"\bMem_Alloc\s*\(")
OFFSET = re.compile(rf"\(\s*(?:const\s+)?(?:u8|s8|char)\s*\*\s*\)\s*[&\w][\w.\->\[\]]*\s*\+\s*{NUM}\b")
FOR = re.compile(r"\bfor\s*\(")
BOUND = re.compile(rf"(?:<=?|>=?|!=)\s*\(?\s*({NUM})\s*\)?\s*$")
ARRAY_INIT = re.compile(r"^(?:static\s+)?(?:const\s+)?(?:struct\s+)?(\w+)\s+\**\s*\w+\s*\[\s*([^\]]*)\]"
                        r"(?:\s*\[[^\]]*\])*\s*=", re.M)
EXTERN = re.compile(r"^\s*extern\s+[^;(]*?\w+\s*\[\s*([^\]]*)\]", re.M)
PLACEHOLDER = re.compile(rf"^(?:static\s+)?u8\s+\w+\s*\[\s*{NUM}\s*\]\s*;", re.M)
ASSERT = re.compile(r"typedef\s+char\s+\w+_size_check\s*\[")
COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)
STRING = re.compile(r'"(?:\\.|[^"\\\n])*"')

KINDS = ("alias", "scan_layout", "scan_high", "scan_medium", "scan_low", "alloc", "copy", "offset", "loop",
         "array_u32", "array_char", "array_other", "mixed_u32", "extern", "placeholder")
AREAS = ("game", "SDK")

# What PROGRESS.md prints for each kind: (group, label).
LABELS = {
    "alias": ("Addresses", "Aliases whose offset the C type does not give (`alias_check.py`)"),
    "scan_layout": ("Addresses", "Unrelocated words, layout level (`scan_pointers.py`)"),
    "scan_high": ("Addresses", "Unrelocated words, high level"),
    "scan_medium": ("Addresses", "Unrelocated words, medium level"),
    "scan_low": ("Addresses", "Unrelocated words, low level (mostly in `u32` tables)"),
    "alloc": ("Sizes", "`Mem_Alloc` with a literal size"),
    "copy": ("Sizes", "Copy, fill and clear calls with a literal length"),
    "offset": ("Sizes", "Byte offsets into objects (`(u8 *)x + N`)"),
    "loop": ("Sizes", "`for` loops with a literal bound"),
    "array_u32": ("Tables", "Initialised `u32` arrays with a literal length"),
    "array_char": ("Tables", "Initialised `char` arrays with a literal length"),
    "array_other": ("Tables", "Other initialised arrays with a literal length"),
    "mixed_u32": ("Tables", "`u32` tables holding pointers and numbers (structs written as words)"),
    "extern": ("Tables", "Extern arrays with a literal length"),
    "placeholder": ("Tables", "`u8 NAME[N]` placeholders for zeroed variables"),
}


def area(path: str) -> str:
    """Game code, or SDK and library code. bss.py gen output counts as game:
    every file it writes stands in for a file under src/game/."""
    return "game" if path.startswith(("src/game/", "include/game/", "build/gen/")) else "SDK"


def strip(text: str) -> str:
    """Comments and string literals blanked, newlines kept (lines still count)."""
    blank = lambda m: re.sub(r"[^\n]", " ", m.group(0))  # noqa: E731
    return STRING.sub(blank, COMMENT.sub(blank, text))


def call_args(text: str, open_paren: int) -> list[str] | None:
    """The top-level arguments of the call whose '(' is at open_paren."""
    depth, start, args = 0, open_paren + 1, []
    for i in range(open_paren, len(text)):
        c = text[i]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
            if depth == 0:
                args.append(text[start:i])
                return [" ".join(a.split()) for a in args]
        elif c == "," and depth == 1:
            args.append(text[start:i])
            start = i + 1
    return None


def is_literal(expr: str) -> bool:
    return bool(LITERAL.match(expr.strip()))


def source_sites(path: str, text: str) -> list[tuple[str, int, str]]:
    """(kind, line, excerpt) for every source kind in one file."""
    text = strip(text)
    out = []
    line = lambda pos: text.count("\n", 0, pos) + 1  # noqa: E731
    excerpt = lambda pos: " ".join(text[pos:pos + 70].split())  # noqa: E731
    for kind, pattern, size_arg in (("alloc", ALLOC, 0), ("copy", COPY, -1)):
        for m in pattern.finditer(text):
            start = text.rfind("\n", 0, m.start()) + 1
            if re.match(r"[A-Za-z_][\w\s\*]*[\s\*]$", text[start:m.start()]):
                continue  # a prototype or definition: at file scope, after a type
            args = call_args(text, m.end() - 1)
            if args and len(args) > (size_arg if size_arg >= 0 else 0) and is_literal(args[size_arg]):
                out.append((kind, line(m.start()), excerpt(m.start())))
    for m in OFFSET.finditer(text):
        out.append(("offset", line(m.start()), excerpt(m.start())))
    for m in FOR.finditer(text):
        args = call_args(text, m.end() - 1)
        parts = args[0].split(";") if args else []
        if len(parts) == 3:
            b = BOUND.search(parts[1].strip())
            if b and int(re.sub(r"[uUlL]+$", "", b.group(1)), 0) != 0:
                out.append(("loop", line(m.start()), excerpt(m.start())))
    for m in ARRAY_INIT.finditer(text):
        elem, length = m.group(1), m.group(2).strip()
        if elem in ("return", "else") or not is_literal(length):
            continue
        kind = {"u32": "array_u32", "char": "array_char"}.get(elem, "array_other")
        out.append((kind, line(m.start()), excerpt(m.start())))
    for m in EXTERN.finditer(text):
        if is_literal(m.group(1)):
            out.append(("extern", line(m.start()), excerpt(m.start())))
    for m in PLACEHOLDER.finditer(text):
        out.append(("placeholder", line(m.start()), excerpt(m.start())))
    for m in ASSERT.finditer(text):
        out.append(("size_assert", line(m.start()), excerpt(m.start())))
    return out


def source_files() -> list[Path]:
    return sorted([*(ROOT / "src").rglob("*.c"), *(ROOT / "src").rglob("*.h"), *(ROOT / "include").rglob("*.h")])


def alias_sites(path: Path | None = None) -> list[tuple[str, str, int, str]]:
    """The aliases whose FIELD is not a path the C type computes in full."""
    path = path or alias_check.ALIAS_LIST
    lines = path.read_text().splitlines()
    aliases, _ = alias_check.read_aliases(path)
    return [("alias", "config/AWRE/aliases.txt", a.line, lines[a.line - 1].strip())
            for a in alias_check.not_from_c(aliases)]


def mixed_u32_sites() -> list[tuple[str, str, int, str]] | None:
    """u32 arrays in src/ whose .inc.c holds a pointer and a number."""
    data = ROOT / "extracted/AWRE/data"
    if not data.is_dir():
        return None
    include = re.compile(r'^\s*(?:static\s+)?(?:const\s+)?u32\s+(\w+)\s*\[[^\]]*\]\s*=\s*\{\s*#include\s+"([^"]+)"', re.M)
    out = []
    for p in sorted((ROOT / "src").rglob("*.c")):
        text = p.read_text()
        for m in include.finditer(text):
            inc = ROOT / "extracted/AWRE" / m.group(2)
            if not inc.exists():
                continue
            body = COMMENT.sub("", inc.read_text())
            values = [v.strip() for v in body.split(",") if v.strip()]
            pointers = sum(1 for v in values if "&" in v or re.match(r"^\(?[A-Za-z_]", v))
            if pointers and pointers < len(values):
                rel = p.relative_to(ROOT).as_posix()
                out.append(("mixed_u32", rel, text.count("\n", 0, m.start()) + 1,
                            f"{m.group(1)}: {pointers} of {len(values)} words are pointers"))
    return out


def scan_sites() -> list[tuple[str, str, int, str]] | None:
    """scan_pointers.py's candidates, or None without a build."""
    sys.path.insert(0, str(TOOLS))
    import modmap
    import scan_pointers

    if not (ROOT / "build/arm9.elf").exists():
        return None
    index = ROOT / "build/fnindex.json"
    if not index.exists():
        subprocess.run([sys.executable, str(TOOLS / "fnindex.py")], check=True, stdout=subprocess.DEVNULL)
    rows, _ = scan_pointers.candidates(modmap.load(), index)
    return [(f"scan_{level}", path or mod, 0, f"{mod} {section} 0x{addr:08x} ({holder}) = 0x{value:08x}")
            for level, mod, section, addr, value, _, _, _, holder, path in rows]


def collect() -> tuple[dict, list[tuple[str, str, int, str]], set[str]]:
    """counts[kind][area], every site as (kind, path, line, text), and the
    kinds that could not be measured."""
    sites = []
    for p in source_files():
        rel = p.relative_to(ROOT).as_posix()
        sites += [(kind, rel, n, text) for kind, n, text in source_sites(rel, p.read_text())]
    gen = ROOT / "build/gen/bss"
    if gen.is_dir():
        for p in sorted(gen.glob("*.inc.c")):
            rel = p.relative_to(ROOT).as_posix()
            sites += [(kind, rel, n, text) for kind, n, text in source_sites(rel, p.read_text())
                      if kind == "placeholder"]
    sites += alias_sites()
    missing = set()
    for kinds, got in (({"mixed_u32"}, mixed_u32_sites()), ({f"scan_{lv}" for lv in ("layout", "high", "medium", "low")}, scan_sites())):
        if got is None:
            missing |= kinds
        else:
            sites += got
    counts = {k: dict.fromkeys(AREAS, 0) for k in (*KINDS, "size_assert")}
    for kind, path, _, _ in sites:
        counts[kind][area(path)] += 1
    return counts, sites, missing


def markdown(counts: dict, missing: set[str]) -> list[str]:
    lines = [
        "| Group | Gap | Game | SDK | Total |",
        "|---|---|---:|---:|---:|",
    ]
    for kind in KINDS:
        group, label = LABELS[kind]
        if kind in missing:
            lines.append(f"| {group} | {label} | - | - | - |")
            continue
        g, s = counts[kind]["game"], counts[kind]["SDK"]
        if kind == "alias":  # aliases.txt does not say which code a base belongs to
            lines.append(f"| {group} | {label} | - | - | {g + s} |")
            continue
        lines.append(f"| {group} | {label} | {g} | {s} | {g + s} |")
    n = counts["size_assert"]
    lines += ["", f"Structs with a size assert: {n['game'] + n['SDK']}."]
    return lines


def main() -> int:
    args = sys.argv[1:]
    if args and (args[0] != "--list" or len(args) != 2 or args[1] not in (*KINDS, "size_assert")):
        sys.exit(__doc__)
    counts, sites, missing = collect()
    if args:
        if args[1] in missing:
            sys.exit(f"shift_gaps.py: {args[1]} needs a build; run `make`")
        for kind, path, n, text in sites:
            if kind == args[1]:
                print(f"{path}:{n}: {text}" if n else f"{path}: {text}")
        return 0
    print("\n".join(markdown(counts, missing)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
