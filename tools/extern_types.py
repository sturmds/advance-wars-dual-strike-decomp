#!/usr/bin/env python3
"""List `extern` declarations of data objects whose type differs from the definition.

    extern_types.py                 one line per extern that differs, then a count
    extern_types.py --symbol NAME   the definition and every extern of NAME
    extern_types.py --count         the count only

A reader that declares a table `extern u8 X[]` while the definition is
`Foo X[]` still links and often still matches, but a change to `Foo` does
not reach that reader: it keeps a byte view of the old layout (issue 0087).
This lists, for every data object defined in C (src/ and the .bss files
`bss.py gen` writes into build/gen/bss/), each file-scope `extern` of it in
src/ and include/ whose type is not the definition's.

Two declarations agree when their element types are the same after
resolving typedefs (`u8` and `unsigned char`, `struct Tag` and its typedef
name) and ignoring `const` and `volatile`, and they have the same number of
array dimensions, with the same lengths past the first (the first may be
`[]` in either). An extern whose type is a struct the reading C file
defines for itself also differs: that copy of the layout does not follow
the definition. An extern of a name no C file defines (an alias inside a
larger object, a linker symbol) is left out, and so is a declaration this
scanner cannot read (a function pointer).

The parser reads the C written in this repo; it is not a full C parser.
Exits 0; the count is a measure (`shift_gaps.py` kind `extern_type`).
"""

import argparse
import re
import sys
from dataclasses import dataclass
from functools import cache
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)
STRING = re.compile(r'"(?:\\.|[^"\\\n])*"')
PREPROC = re.compile(r"^[ \t]*#(?:[^\n]*\\\n)*[^\n]*", re.M)
IGNORED = {"const", "volatile", "register", "struct", "union", "enum", "signed_"}
STORAGE = {"extern", "static", "inline", "__inline"}
BASE = {
    "unsigned char": "u8", "signed char": "s8",
    "unsigned short": "u16", "unsigned short int": "u16", "short": "s16", "short int": "s16",
    "signed short": "s16", "unsigned int": "u32", "unsigned": "u32", "int": "s32", "signed int": "s32",
    "signed": "s32", "unsigned long": "u32", "long": "s32", "signed long": "s32",
    "unsigned long long": "u64", "long long": "s64", "signed long long": "s64",
}


@dataclass
class Decl:
    name: str
    base: str        # element type as written, qualifiers and tag keywords removed
    stars: int       # pointer level of the element
    dims: list[str]  # array lengths as written ('' for [])
    path: str
    line: int
    extern: bool
    static: bool
    const: bool = False
    span: tuple[int, int] = (0, 0)  # the statement's offsets in the file, `;` included

    def text(self) -> str:
        return f"{self.base}{' ' + '*' * self.stars if self.stars else ''} {self.name}" + "".join(
            f"[{d}]" for d in self.dims)


def blank(text: str) -> str:
    """Comments, strings and preprocessor lines blanked, newlines kept."""
    sub = lambda m: re.sub(r"[^\n]", " ", m.group(0))  # noqa: E731
    return PREPROC.sub(sub, STRING.sub(sub, COMMENT.sub(sub, text)))


def statements(text: str):
    """(start, end, text) of each file-scope statement, end past its `;`,
    with the contents of braces cut out. A function body ends its statement."""
    depth, start, buf = 0, 0, []
    for i, c in enumerate(text):
        if c == "{":
            if depth == 0:
                buf.append("{}")
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                s = "".join(buf)
                if "(" in s and "=" not in s and not s.lstrip().startswith("typedef"):
                    buf, start = [], i + 1  # a function definition
        elif depth == 0:
            if c == ";":
                yield start, i + 1, "".join(buf)
                buf, start = [], i + 1
            else:
                if not buf and c.isspace():
                    start = i + 1
                    continue
                buf.append(c)


def split_top(text: str, sep: str = ",") -> list[str]:
    out, depth, cur = [], 0, []
    for c in text:
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        if c == sep and depth == 0:
            out.append("".join(cur))
            cur = []
        else:
            cur.append(c)
    out.append("".join(cur))
    return out


DECLARATOR = re.compile(r"^(\**)\s*([A-Za-z_]\w*)\s*((?:\[[^\]]*\]\s*)*)$")


def parse(stmt: str, path: str, line: int, span: tuple[int, int] = (0, 0)) -> list[Decl]:
    """The objects one file-scope statement declares (none for a typedef,
    a prototype or a function pointer)."""
    s = " ".join(stmt.split())
    if not s or s.startswith(("typedef", "asm ", "__asm")):
        return []
    s = re.sub(r"\{\}", "", s)
    head = split_top(s, "=")[0] if "=" in s else s
    parts = split_top(head)
    first = parts[0].strip()
    # The type is everything before the first declarator: the last identifier
    # (with its stars and brackets) is the declarator.
    m = re.match(r"^(.*?[\w\s])\s*(\**\s*[A-Za-z_]\w*\s*(?:\[[^\]]*\]\s*)*)$", first)
    if not m or "(" in first:
        return []
    words = m.group(1).split()
    if not words or any(not re.fullmatch(r"\w+", w) for w in words):
        return []
    extern = "extern" in words
    static = "static" in words
    const = "const" in words
    words = [w for w in words if w not in STORAGE and w not in IGNORED]
    if not words or words[0] in ("return", "goto", "case"):
        return []
    base = " ".join(words)
    out = []
    for decl in [m.group(2), *parts[1:]]:
        d = DECLARATOR.match(decl.strip())
        if not d:
            continue
        dims = [x.strip() for x in re.findall(r"\[([^\]]*)\]", d.group(3))]
        out.append(Decl(d.group(2), base, len(d.group(1)), dims, path, line, extern, static, const, span))
    return out


def declarations(path: Path, rel: str) -> list[Decl]:
    text = blank(path.read_text(errors="replace"))
    out = []
    for start, end, stmt in statements(text):
        out += parse(stmt, rel, text.count("\n", 0, start) + 1, (start, end))
    return out


TYPEDEF = re.compile(r"^typedef\s+(?:(?:const|volatile)\s+)*(?:struct|union|enum)?\s*(\w+(?:\s+\w+)*?)\s*(\**)\s*(\w+)\s*$")


def typedefs(files: list[Path]) -> dict[str, str]:
    """Name -> what it stands for, from `typedef A B;` and `typedef struct
    Tag {...} B;` (Tag -> B). A name with two meanings is left out."""
    seen: dict[str, set[str]] = {}
    for p in files:
        text = blank(p.read_text(errors="replace"))
        for _, _, stmt in statements(text):
            s = " ".join(stmt.split())
            if not s.startswith("typedef") or "(" in s or "[" in s:
                continue
            tag = re.match(r"^typedef\s+(?:struct|union|enum)\s+(\w+)\s*\{\}\s*(\w+)$", s)
            if tag:
                if tag.group(1) != tag.group(2):
                    seen.setdefault(tag.group(1), set()).add(tag.group(2))
                continue
            if "{}" in s:
                continue
            m = TYPEDEF.match(s)
            if m and not m.group(2) and m.group(1) != m.group(3):
                words = [w for w in m.group(1).split() if w not in IGNORED]
                seen.setdefault(m.group(3), set()).add(" ".join(words))
    return {k: next(iter(v)) for k, v in seen.items() if len(v) == 1}


def resolve(base: str, aliases: dict[str, str]) -> str:
    base = BASE.get(base, base)
    for _ in range(8):
        nxt = BASE.get(aliases.get(base, base), aliases.get(base, base))
        if nxt == base:
            break
        base = nxt
    return base


DEFINE = re.compile(r"^[ \t]*#[ \t]*define[ \t]+([A-Za-z_]\w*)[ \t]+([^\n]*?)[ \t]*(?://[^\n]*|/\*[^\n]*)?$", re.M)


@cache
def defines() -> dict[str, str]:
    """`#define NAME VALUE` from include/, for array lengths."""
    out: dict[str, str] = {}
    for p in sorted((ROOT / "include").rglob("*.h")):
        for m in DEFINE.finditer(p.read_text(errors="replace")):
            out.setdefault(m.group(1), m.group(2))
    return out


def length(dim: str) -> str:
    """An array length as a number when the defines give one, else as written."""
    text = dim.replace(" ", "")
    for _ in range(8):
        new = re.sub(r"[A-Za-z_]\w*", lambda m: f"({defines().get(m.group(0), m.group(0))})", text)
        if new == text:
            break
        text = new
    text = re.sub(r"(0[xX][0-9a-fA-F]+|\d+)[uUlL]*", r"\1", text)
    if re.fullmatch(r"[\d\sxXa-fA-F()+\-*/<>|&]+", text):
        try:
            return str(int(eval(text.replace("/", "//"), {"__builtins__": {}})))
        except Exception:
            pass
    return dim.replace(" ", "")


def agree(a: Decl, b: Decl, aliases: dict[str, str]) -> bool:
    if resolve(a.base, aliases) != resolve(b.base, aliases) or a.stars != b.stars:
        return False
    if len(a.dims) != len(b.dims):
        return False
    return all(length(x) == length(y) for x, y in zip(a.dims[1:], b.dims[1:]))


def files() -> tuple[list[Path], list[Path]]:
    """C sources that define data (src/ and build/gen/bss/), and the files
    that declare externs (src/ and include/)."""
    src = sorted((ROOT / "src").rglob("*.c"))
    gen = sorted((ROOT / "build/gen/bss").glob("*.inc.c")) if (ROOT / "build/gen/bss").is_dir() else []
    headers = sorted([*(ROOT / "include").rglob("*.h"), *(ROOT / "src").rglob("*.h")])
    return src + gen, src + headers


@cache
def collect() -> tuple[dict[str, list[Decl]], dict[str, list[Decl]], dict[str, str]]:
    """Definitions and externs by name, and the typedef map."""
    defining, declaring = files()
    defs: dict[str, list[Decl]] = {}
    exts: dict[str, list[Decl]] = {}
    defining_set, declaring_set = set(defining), set(declaring)
    for p in sorted(defining_set | declaring_set):
        rel = p.relative_to(ROOT).as_posix()
        for d in declarations(p, rel):
            if d.extern:
                if p in declaring_set:
                    exts.setdefault(d.name, []).append(d)
            elif not d.static and p in defining_set and p.suffix == ".c":
                defs.setdefault(d.name, []).append(d)
    return defs, exts, typedefs(declaring)


@cache
def file_types(path: str) -> frozenset[str]:
    return frozenset(local_types((ROOT / path).read_text(errors="replace")))


def local_copy(e: Decl, d: Decl) -> bool:
    """The extern's type is a struct the reading C file defines itself while
    the object is defined in another file: a copy of the layout that a
    change to the definition's type does not reach."""
    return e.path != d.path and e.path.endswith(".c") and e.base in file_types(e.path)


def mismatches() -> list[tuple[Decl, Decl]]:
    """(extern, definition) for each extern whose type differs, or that uses
    a local copy of the type."""
    defs, exts, aliases = collect()
    out = []
    for name, ds in sorted(exts.items()):
        if name not in defs:
            continue
        d = defs[name][0]
        out += [(e, d) for e in ds if not agree(e, d, aliases) or local_copy(e, d)]
    return sorted(out, key=lambda x: (x[0].path, x[0].line))


BUILTIN = {"u8", "u16", "u32", "u64", "s8", "s16", "s32", "s64", "BOOL", "fx32", "char", "void", "float",
           "double", *BASE}
TYPE_NAME = re.compile(r"\}\s*(\w+)\s*;|^typedef\b[^;{}()]*?\b(\w+)\s*;|^typedef\b[^;{}]*\(\s*\*\s*(\w+)\s*\)", re.M)


@cache
def type_headers() -> dict[str, str]:
    """Type name -> the header under include/ that defines it, as an
    #include path; a name two headers define is left out."""
    seen: dict[str, set[str]] = {}
    for p in sorted((ROOT / "include").rglob("*.h")):
        text = blank(p.read_text(errors="replace"))
        rel = p.relative_to(ROOT / "include").as_posix()
        for m in TYPE_NAME.finditer(text):
            seen.setdefault(next(g for g in m.groups() if g), set()).add(rel)
    return {k: next(iter(v)) for k, v in seen.items() if len(v) == 1}


def declaration(e: Decl, d: Decl) -> str:
    """The extern e rewritten with the definition's type. An outer length
    the extern names (not a number) stays; else the outer length is `[]`."""
    dims = list(d.dims)
    if dims:
        keep = e.dims[0] if e.dims and len(e.dims) == len(d.dims) and not re.fullmatch(r"[\dxXa-fA-F]*", e.dims[0]) else ""
        dims[0] = keep
    stars = " " + "*" * d.stars if d.stars else ""
    return f"extern {'const ' if d.const else ''}{d.base}{stars} {d.name}" + "".join(f"[{x}]" for x in dims) + ";"


def local_types(text: str) -> set[str]:
    return {next(g for g in m.groups() if g) for m in TYPE_NAME.finditer(blank(text))}


# Extern element types that give no layout: an extern written with one of
# these is a byte or word view, and --fix gives it the definition's type.
PLAIN = {"u8", "s8", "char", "u16", "s16", "u32", "s32", "void"}


def fix(only: set[str] | None) -> tuple[int, list[str]]:
    """Rewrites each extern that differs with the definition's type, and
    includes the header of a type the file does not define. Returns the
    number rewritten and the sites left, with the reason."""
    defs, exts, aliases = collect()
    by_file: dict[str, list[tuple[Decl, Decl]]] = {}
    for e, d in mismatches():
        if only is None or e.path in only:
            by_file.setdefault(e.path, []).append((e, d))
    done, left = 0, []
    for path, rows in sorted(by_file.items()):
        f = ROOT / path
        text = f.read_text()
        mine = local_types(text)
        includes = set()
        edits = []
        for e, d in rows:
            if resolve(e.base, aliases) not in PLAIN:
                left.append(f"{e.path}:{e.line}: {e.name}: the extern has a type of its own ({e.base}); "
                            f"the definition ({d.text()}) may be the one to change")
                continue
            if len(defs[e.name]) > 1 and any(not agree(d, o, aliases) for o in defs[e.name][1:]):
                left.append(f"{e.path}:{e.line}: {e.name}: two definitions with different types")
                continue
            if d.base not in BUILTIN and d.base not in mine:
                header = type_headers().get(d.base)
                if header is None:
                    left.append(f"{e.path}:{e.line}: {e.name}: type {d.base} is not defined in one header")
                    continue
                includes.add(header)
            if any(o.span == e.span for o, _ in edits):
                left.append(f"{e.path}:{e.line}: {e.name}: declared with other names in one statement")
                continue
            edits.append((e, declaration(e, d)))
        for e, new in sorted(edits, key=lambda x: -x[0].span[0]):
            text = text[:e.span[0]] + new + text[e.span[1]:]
            done += 1
        includes = {h for h in includes if f'#include "{h}"' not in text and path != f"include/{h}"}
        if includes:
            lines = text.split("\n")
            # The last #include before the first declaration (an #include
            # inside an initializer does not count).
            first = next(statements(blank(text)), (len(text),))[0]
            top = text.count("\n", 0, first)
            last = max((i for i, ln in enumerate(lines[:top]) if ln.startswith("#include")), default=-1)
            add = [f'#include "{h}"' for h in sorted(includes)]
            if last < 0:
                guard = next((i for i, ln in enumerate(lines) if ln.startswith("#define")), -1)
                lines[guard + 1:guard + 1] = ["", *add]
            else:
                lines[last + 1:last + 1] = add
            text = "\n".join(lines)
        f.write_text(text)
    return done, left


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--symbol")
    ap.add_argument("--count", action="store_true")
    ap.add_argument("--fix", nargs="*", metavar="FILE",
                    help="rewrite the externs that differ (in FILEs, or everywhere) with the definition's type")
    args = ap.parse_args()
    if args.fix is not None:
        done, left = fix(set(args.fix) if args.fix else None)
        print("\n".join(left))
        print(f"{done} externs rewritten, {len(left)} left")
        return 0
    if args.symbol:
        defs, exts, aliases = collect()
        for d in defs.get(args.symbol, []):
            print(f"{d.path}:{d.line}: {d.text()}")
        for e in exts.get(args.symbol, []):
            ok = all(agree(e, d, aliases) for d in defs.get(args.symbol, []))
            print(f"{e.path}:{e.line}: extern {e.text()}{'' if ok else '   (differs)'}")
        return 0
    rows = mismatches()
    if not args.count:
        for e, d in rows:
            print(f"{e.path}:{e.line}: extern {e.text()}; defined {d.text()} ({d.path}:{d.line})")
    print(f"{len(rows)} externs differ from their definition")
    return 0


if __name__ == "__main__":
    sys.exit(main())
