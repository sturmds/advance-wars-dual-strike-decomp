#!/usr/bin/env python3
"""Merge neighbouring one-function C files into one file with a header.

    python3 tools/merge_files.py OUT.c FILE.c... [--header include/PATH.h] [--dry-run]
    python3 tools/merge_files.py --split FILE.c FUNC [--rest REST.c]

--split moves FUNC out of a merged FILE into a file named after it, for a
function that cannot share a file (a #pragma, a callee it declares
otherwise than the others do); the functions at higher addresses go to
REST.c. Each file keeps the declarations it uses; the header stays.


The FILEs must be `complete` entries of one delinks.txt holding only .text,
and together cover one run of addresses with no gap. The tool:

- writes OUT.c: the includes and file-scope declarations of every FILE
  once, then the functions in reverse address order, because mwccarm 2.0
  emits a file's functions last to first (src/game/main/func_02026868.c);
- writes the header (default: include/game/<directory under main or
  ovNNN>/<OUT stem>.h) with a prototype for each function, copied from its
  definition, in address order; a function that another header declares
  has that declaration moved here as it is;
- replaces the delinks.txt entries with one entry for OUT.c;
- removes the other C files' copies of the declarations and includes the
  header there instead (tools/decls.py declare, which leaves a copy where
  the include would clash, and lists it).

It stops before writing anything when two FILEs define one macro or declare
one name differently (unless one of them is the definition's), or defines
one type name differently, or has a static or an #if: those need a decision
by hand, or has a #pragma: mwcc applies an optimisation pragma to every
function of the file, so that function keeps its own file. Then run `make`, `make check` and
`tools/objcmp.py compare` (docs/readability.md).
"""
import argparse
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from mwcc_order import definition_order  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "config/AWRE"
IDENT = re.compile(r"[A-Za-z_]\w*")


@dataclass
class Item:
    kind: str     # "include", "define", "pp", "decl", "func"
    text: str     # the item as written, with its leading comment
    name: str = ""
    body: str = ""  # the item without its leading comment


def split_items(text: str) -> list:
    """Top-level items of a C file. A comment belongs to the item after it."""
    items = []
    i, n = 0, len(text)
    comment = ""
    while i < n:
        c = text[i]
        if c in " \t\n":
            i += 1
            continue
        if text.startswith("/*", i):
            j = text.index("*/", i) + 2
            comment += text[i:j] + "\n"
            i = j
            continue
        if text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            comment += text[i:j] + "\n"
            i = j
            continue
        if c == "#":
            j = i
            while True:
                k = text.find("\n", j)
                if k < 0:
                    k = n
                    break
                if text[k - 1] != "\\":
                    break
                j = k + 1
            body = text[i:k]
            m = re.match(r"#\s*(\w+)\s*([A-Za-z_]\w*)?", body)
            kind = {"include": "include", "define": "define"}.get(m.group(1), "pp")
            name = m.group(2) or "" if kind == "define" else ""
            items.append(Item(kind, comment + body, name, body))
            comment = ""
            i = k
            continue
        # Code up to `;` at depth 0, or a function body's closing brace.
        j, depth, paren = i, 0, 0
        is_func = False
        while j < n:
            c = text[j]
            if text.startswith("/*", j):
                j = text.index("*/", j) + 2
                continue
            if text.startswith("//", j):
                j = text.find("\n", j)
                continue
            if c in "\"'":
                k = j + 1
                while text[k] != c:
                    k += 2 if text[k] == "\\" else 1
                j = k + 1
                continue
            if c == "(":
                paren += 1
            elif c == ")":
                paren -= 1
            elif c == "{":
                if depth == 0 and paren == 0 and text[i:j].rstrip().endswith(")") \
                        and "=" not in text[i:j]:
                    is_func = True
                depth += 1
            elif c == "}":
                depth -= 1
                if depth == 0 and is_func:
                    j += 1
                    break
            elif c == ";" and depth == 0 and paren == 0:
                j += 1
                break
            j += 1
        body = text[i:j]
        head = body.split("{")[0] if is_func else body
        items.append(Item("func" if is_func else "decl", comment + body, func_name(head), body))
        comment = ""
        i = j
    return items


def func_name(head: str) -> str:
    """The name a function definition or prototype declares; "" for data
    and function pointers."""
    m = re.search(r"([A-Za-z_]\w*)\s*\(", head)
    if not m or re.search(r"\(\s*\*", head[: m.end() + 1]):
        return ""
    if re.match(r"\s*(typedef|struct|union|enum)\b", head) and "{" in head:
        return ""
    return m.group(1)


def norm(s: str) -> str:
    return re.sub(r"\s+", " ", re.sub(r"\s*([(),*\[\]])\s*", r"\1", s)).strip()


def defined_names(body: str) -> set:
    """Names a type definition defines: its tag, typedef name and enum constants."""
    out = set(re.findall(r"\b(?:struct|union|enum)\s+(\w+)\s*\{", body))
    m = re.search(r"\}\s*(\w+)\s*;\s*$", body)
    if m and body.lstrip().startswith("typedef"):
        out.add(m.group(1))
    for inner in re.findall(r"\benum\b[^{]*\{([^}]*)\}", body):
        out |= {c.strip().split("=")[0].strip() for c in inner.split(",") if c.strip()}
    return out


def sig_types(proto: str) -> str:
    """A prototype with parameter names taken out, normalised."""
    m = re.match(r"(.*?)\((.*)\)\s*;?\s*$", re.sub(r"^extern ", "", norm(proto)), re.S)
    if not m:
        return norm(proto)
    params = []
    for p in m.group(2).split(","):
        words = re.findall(r"[A-Za-z_]\w*|\*|\[\]", p)
        if len(words) > 1 and IDENT.fullmatch(words[-1]) and words[-1] not in ("void", "int", "char"):
            words = words[:-1]
        params.append(" ".join(words).replace(" *", "*"))
    return norm(m.group(1)) + "(" + ",".join(params) + ")"


_defs = None


def definitions() -> dict:
    """Function name -> prototype of its definition in src/ (non-static)."""
    global _defs
    if _defs is None:
        _defs = {}
        for p in sorted((ROOT / "src").rglob("*.c")):
            for it in split_items(p.read_text(errors="replace")):
                if it.kind == "func" and it.name and not re.match(r"\s*static\b", it.body):
                    _defs.setdefault(it.name, prototype(it))
    return _defs


def data_order(defs: list, symbols: Path) -> list:
    """Variable definitions in the order that makes mwccarm emit them at
    their addresses (tools/mwcc_order.py)."""
    if len(defs) < 2:
        return defs
    table = {}
    for line in symbols.read_text().splitlines():
        m = re.match(r"(\S+) kind:\w+\(size=0x(\w+)\) addr:0x(\w+)", line)
        if m:
            table[m.group(1)] = (int(m.group(3), 16), int(m.group(2), 16))
    missing = [decl_key(d) for d in defs if decl_key(d) not in table]
    if missing:
        raise ValueError(f"no address and size in {symbols.name}: {' '.join(missing)}")
    init = {"=" in d.body for d in defs}
    if len(init) > 1:
        raise ValueError("initialized and uninitialized variables together: order them by hand")
    defs = sorted(defs, key=lambda d: table[decl_key(d)][0])
    sizes = [table[decl_key(d)][1] for d in defs]
    try:
        order = definition_order(sizes, initialized=init.pop())
    except ValueError:
        raise ValueError("no definition order puts these variables at their addresses "
                         f"(sizes {sizes}): they come from more than one original file")
    return [defs[i] for i in order]


def same_array(a: Item, b: Item) -> bool:
    """Two declarations of one variable that differ only in an array length."""
    t = lambda it: norm(re.sub(r"\[[^\]]*\]", "[]", it.body))
    return not a.name and not b.name and t(a) == t(b)


def decl_key(item: Item) -> str:
    """Name a declaration declares, to find two declarations of one name."""
    if item.name:
        return item.name
    m = re.search(r"\(\s*\*\s*(?:const\s+|volatile\s+)*([A-Za-z_]\w*)", item.body)
    if m:  # a function pointer: void (*name[])(void)
        return m.group(1)
    names = IDENT.findall(re.sub(r"\[.*?\]|=.*", "", item.body))
    return names[-1] if names else item.body


def read_delinks(path: Path) -> list:
    """Entries of a delinks.txt as (file, [lines]), the header block first."""
    entries, cur = [], None
    for line in path.read_text().splitlines():
        if re.match(r"^\S.*:$", line):
            cur = (line[:-1], [])
            entries.append(cur)
        elif cur is None:
            entries.append(("", [line]))
        else:
            cur[1].append(line)
    return entries


def write_delinks(path: Path, entries: list):
    out = []
    for name, lines in entries:
        if name:
            out.append(name + ":")
        out.extend(lines)
    path.write_text("\n".join(out) + "\n")


def find_delinks(rel: str) -> Path:
    for p in sorted(CONFIG.rglob("delinks.txt")):
        if f"\n{rel}:\n" in "\n" + p.read_text():
            return p
    sys.exit(f"error: {rel} is in no delinks.txt")


def header_path(out: str) -> str:
    parts = Path(out).with_suffix(".h").parts
    # src/game/<module>/<dirs>/X.c -> include/game/<dirs>/X.h
    if parts[:2] == ("src", "game") and len(parts) > 4:
        return str(Path("include/game", *parts[3:]))
    return str(Path("include", *parts[1:]))


def typedef_headers() -> dict:
    """Type name -> header in include/ that defines it (top-level typedefs
    and struct, union and enum tags only)."""
    found = {}
    for h in sorted((ROOT / "include").rglob("*.h")):
        for it in split_items(h.read_text(errors="replace")):
            if it.kind == "decl" and re.match(r"\s*(typedef|struct|union|enum)\b", it.body):
                names = set(re.findall(r"\b(?:struct|union|enum)\s+(\w+)\s*\{", it.body))
                if it.body.lstrip().startswith("typedef"):
                    m = re.search(r"(\w+)\s*(\[[^\]]*\])*\s*;\s*$", it.body) or \
                        re.search(r"\(\s*\*\s*(\w+)\s*\)", it.body)
                    if m:
                        names.add(m.group(1))
                for n in names:
                    found.setdefault(n, str(h.relative_to(ROOT / "include")))
    return found


def add_include(text: str, inc: str) -> str:
    """Add an #include after the ones at the top of the file (not those
    inside an initializer further down)."""
    pos = 0
    for m in re.finditer(r"^.*$\n?", text, re.M):
        line = m.group(0).strip()
        if line.startswith("#include"):
            pos = m.end()
        elif line and not line.startswith(("/*", "*", "//", "#if", "#endif", "#else")):
            break
    return text[:pos] + inc + "\n" + text[pos:]


def prototype(func: Item) -> str:
    head = func.body.split("{")[0].strip()
    return re.sub(r"\s+", " ", head) + ";"


def prune(items: list, funcs: list) -> list:
    """The non-function items that the functions (or other kept items) use."""
    code = "\n".join(f.body for f in funcs)
    kept, changed = list(items), True
    while changed:
        changed = False
        text = code + "\n".join(i.body for i in kept if i.kind == "define")
        for it in list(kept):
            if it.kind == "include":
                continue
            name = it.name if it.kind in ("define", "func") else decl_key(it) if it.kind == "decl" else ""
            names = defined_names(it.body) if re.match(r"\s*(typedef|struct|union|enum)\b", it.body) else {name}
            if it.kind == "pp" or not any(n and re.search(rf"\b{re.escape(n)}\b", text) for n in names):
                if it.kind != "pp":
                    kept.remove(it)
                    changed = True
    return kept


def split(path: str, func: str, rest: str) -> int:
    """Move FUNC out of a merged file into a file of its own; functions at
    higher addresses go to REST (one file covers one run of addresses)."""
    src = ROOT / path
    items = split_items(src.read_text())
    funcs = [it for it in items if it.kind == "func"]   # last to first
    if func not in [f.name for f in funcs]:
        sys.exit(f"error: {func} is not defined in {path}")
    syms = {}
    delinks = find_delinks(path)
    for line in (delinks.parent / "symbols.txt").read_text().splitlines():
        m = re.match(r"(\S+) kind:function\(.*?size=0x(\w+).*?addr:0x(\w+)", line)
        if m:
            syms[m.group(1)] = (int(m.group(3), 16), int(m.group(2), 16))
    order = sorted(funcs, key=lambda f: syms[f.name][0])
    i = [f.name for f in order].index(func)
    parts = [(path, order[:i]), (str(Path(path).with_name(snake(func) + ".c")), [order[i]]),
             (rest, order[i + 1:])]
    if not order[:i]:
        parts[2] = (path, order[i + 1:])  # nothing before FUNC: the rest keeps the file
    parts = [(p, fs) for p, fs in parts if fs]
    if order[:i] and order[i + 1:] and not rest:
        sys.exit(f"error: functions follow {func} in {path}: name their file with --rest")
    # A function's own #pragma lines sit between push and pop right above
    # it; they go to its file only, without push and pop.
    own_pragmas, others, pending = {}, [], []
    for it in items:
        if it.kind == "pp" and re.match(r"#\s*pragma\s+(push|pop)\b", it.body):
            continue
        if it.kind == "pp" and re.match(r"#\s*pragma\b", it.body):
            pending.append(it)
        elif it.kind == "func":
            own_pragmas[it.name] = pending
            pending = []
        else:
            others.append(it)
    entries = read_delinks(delinks)
    old = next(lines for name, lines in entries if name == path)
    body = [l for l in old if l.strip() and l.strip() != "complete"]
    if len(body) != 1:
        sys.exit(f"error: {path} holds more than .text; split it by hand")
    new_entries = []
    for p, fs in parts:
        start = syms[fs[0].name][0]
        nxt = order.index(fs[-1]) + 1
        end = syms[order[nxt].name][0] if nxt < len(order) else \
            int(re.search(r"end:0x(\w+)", body[0]).group(1), 16)
        new_entries.append((p, ["    complete", f"    .text       start:{start:#010x} end:{end:#010x}", ""]))
        kept = prune(others, fs)
        pragmas = [pp.text.strip() for f in fs for pp in own_pragmas.get(f.name, [])]
        if pragmas and len(fs) > 1:
            sys.exit(f"error: {p} would hold a function with #pragma lines and others")
        text = "\n".join(pragmas) + ("\n\n" if pragmas else "")
        text += "\n\n".join(it.text.strip() for it in kept if it.kind == "include")
        text = re.sub(r"\n\n", "\n", text) + "\n\n"
        rest_items = [it.text.strip() for it in kept if it.kind != "include"]
        if len(fs) == 1:
            rest_items = [t for t in rest_items if "reverse address order" not in t]
        text += "\n\n".join(rest_items) + ("\n\n" if rest_items else "")
        text += "\n\n".join(f.text.strip() for f in reversed(fs)) + "\n"
        (ROOT / p).write_text(text)
    out = []
    for name, lines in entries:
        if name == path:
            out += new_entries
        else:
            out.append((name, lines))
    write_delinks(delinks, out)
    subprocess.run(["git", "add", "-N", *[p for p, _ in parts]], cwd=ROOT, check=False)
    if (ROOT / "tools/format.py").exists():
        subprocess.run([sys.executable, ROOT / "tools/format.py", *[p for p, _ in parts]], cwd=ROOT,
                       check=False)
    for p, fs in parts:
        print(f"{p}: {' '.join(f.name for f in fs)}")
    return 0


def snake(name: str) -> str:
    return re.sub(r"(?<=[a-z0-9])(?=[A-Z])", "_", name).lower()


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    if sys.argv[1:2] == ["--split"]:
        sp = argparse.ArgumentParser()
        sp.add_argument("--split", nargs=2, metavar=("FILE.c", "FUNC"))
        sp.add_argument("--rest")
        a = sp.parse_args()
        return split(a.split[0], a.split[1], a.rest)
    ap.add_argument("out")
    ap.add_argument("files", nargs="+")
    ap.add_argument("--header")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    rels = [str(Path(f)) for f in args.files]
    delinks = find_delinks(rels[0])
    entries = read_delinks(delinks)
    spans, sections = {}, {}
    for name, lines in entries:
        if name in rels:
            body = [l.strip() for l in lines if l.strip()]
            if not body or body[0] != "complete":
                sys.exit(f"error: {name} is not complete")
            for l in body[1:]:
                m = re.match(r"(\.\w+)\s+start:0x(\w+) end:0x(\w+)$", l)
                if not m:
                    sys.exit(f"error: {name}: cannot read {l!r}")
                sections.setdefault(m.group(1), []).append(
                    (int(m.group(2), 16), int(m.group(3), 16), name))
            text = [s for s in sections.get(".text", []) if s[2] == name]
            if not text:
                sys.exit(f"error: {name} has no .text")
            spans[name] = text[0][:2]
    missing = [r for r in rels if r not in spans]
    if missing:
        sys.exit(f"error: not in {delinks.relative_to(ROOT)}: {' '.join(missing)}")
    rels.sort(key=lambda r: spans[r][0])
    for a, b in zip(rels, rels[1:]):
        if spans[a][1] != spans[b][0]:
            sys.exit(f"error: gap between {a} (ends {spans[a][1]:#x}) and {b} (starts {spans[b][0]:#x})")
    # Other sections must also form one run each; mwccarm then decides the
    # order of the data inside it (docs/workflow.md, "Several data objects in
    # one file"), which make check confirms.
    merged = []
    for sec, parts in sections.items():
        parts.sort()
        for (_, e, a), (s, _, b) in zip(parts, parts[1:]):
            if e != s:
                sys.exit(f"error: {sec} of {a} and {b} are not neighbours")
        merged.append(f"    {sec:<12}start:{parts[0][0]:#010x} end:{parts[-1][1]:#010x}")

    header = args.header or header_path(args.out)
    guard = re.sub(r"\W", "_", str(Path(header).relative_to("include")).upper())

    includes, decls, defines, funcs, problems, notes = [], {}, {}, [], [], []
    types, pragmas = [], {}
    strip = lambda t: re.sub(r"/\*.*?\*/|//[^\n]*|\\\n", " ", t, flags=re.S)
    for rel in rels:
        for it in split_items((ROOT / rel).read_text()):
            if it.kind == "include":
                if it.body not in includes:
                    includes.append(it.body)
            elif it.kind == "define":
                old = defines.get(it.name)
                if old and norm(strip(old.body)) != norm(strip(it.body)):
                    problems.append(f"{rel}: {it.name} defined differently:\n  {old.body}\n  {it.body}")
                defines.setdefault(it.name, it)
            elif it.kind == "pp" and re.match(r"#\s*pragma\b", it.body):
                # mwcc applies an optimisation #pragma to every function of
                # the file, even between push and pop: the function keeps its
                # own file.
                problems.append(f"{rel}: {it.body.strip()} (a file with a #pragma is not merged)")
            elif it.kind == "pp":
                problems.append(f"{rel}: {it.body.strip()}")
            elif it.kind == "func":
                if re.match(r"\s*static\b", it.body):
                    problems.append(f"{rel}: static function {it.name}")
                funcs.append((rel, it))
            elif re.match(r"\s*(typedef|struct|union|enum)\b", it.body):
                mine = defined_names(it.body)
                same = [t for t in types if defined_names(t.body) & mine]
                if any(norm(strip(t.body)) != norm(strip(it.body)) for t in same):
                    problems.append(f"{rel}: {' '.join(sorted(mine))} defined differently")
                elif not same:
                    types.append(it)
            else:
                if re.match(r"\s*static\b", it.body):
                    problems.append(f"{rel}: {it.body.splitlines()[0]}")
                key = decl_key(it)
                old = decls.get(key)
                if old and sig_types(old.body) != sig_types(it.body) and not same_array(old, it):
                    old_ext = old.body.lstrip().startswith("extern")
                    new_ext = it.body.lstrip().startswith("extern")
                    want = definitions().get(key) if it.name else None
                    if old_ext and not new_ext and not it.name:
                        decls[key] = it  # a variable's definition replaces its extern
                    elif not old_ext and new_ext and not it.name:
                        pass
                    elif want:
                        # The definition's prototype; objcmp.py shows a caller it changes.
                        keep = split_items(want)[0]
                        notes.append(f"{key}: declared as defined, {want}; dropped "
                                     f"{old.body.strip()} and {it.body.strip()}")
                        decls[key] = keep
                    else:
                        problems.append(f"{rel}: {key} declared differently:\n  {old.body}\n  {it.body}")
                decls.setdefault(key, it)
    names = [f.name for _, f in funcs]
    # A function another header declares keeps that declaration, moved to
    # this header as it is by decls.py.
    elsewhere = set()
    for h in sorted((ROOT / "include").rglob("*.h")):
        if str(h.relative_to(ROOT)) == header:
            continue
        for it in split_items(h.read_text(errors="replace")):
            if it.kind == "decl" and it.name in names:
                elsewhere.add(it.name)
    if problems:
        print("\n".join(problems), file=sys.stderr)
        return 1

    # The header: one prototype per function, in address order.
    tdh = typedef_headers()
    needed = []
    for _, f in funcs:
        if f.name in elsewhere:
            continue
        for word in IDENT.findall(prototype(f)):
            h = tdh.get(word)
            if h and h not in needed and h != "types.h":
                needed.append(h)
    hdr = [f"#ifndef {guard}", f"#define {guard}", "", '#include "types.h"']
    hdr += [f'#include "{h}"' for h in sorted(needed)]
    hdr += [""] + [prototype(f) for _, f in funcs if f.name not in elsewhere] + ["", "#endif", ""]

    # The C file: includes, the header, declarations, then the functions
    # last to first.
    inc = f'#include "{Path(header).relative_to("include")}"'
    c = [i for i in includes] + ([inc] if inc not in includes else []) + [""]
    c += ["/* The functions are in reverse address order: mwccarm emits a file's",
          " * functions last to first. */", ""]
    c += [d.text for d in defines.values()]
    if defines:
        c.append("")
    c += [t.text for t in types] + ([""] if types else [])
    externs = [d for d in decls.values() if not d.name and d.body.lstrip().startswith("extern")]
    defs = [d for d in decls.values() if not d.name and d not in externs]
    protos = [d for d in decls.values() if d.name and d.name not in names]
    try:
        defs = data_order(defs, delinks.parent / "symbols.txt")
    except ValueError as e:
        sys.exit(f"error: {e}")
    for group in (externs, defs, protos):
        c += [d.text for d in group] + ([""] if group else [])
    c.append("")
    for rel, f in reversed(funcs):
        if rel in pragmas:
            c += ["#pragma push"] + [p.text.strip() for p in pragmas[rel]]
            c += [f.text.strip(), "#pragma pop", ""]
        else:
            c += [f.text.strip(), ""]

    print(f"{args.out}: {len(funcs)} functions from {len(rels)} files, "
          f"{spans[rels[0]][0]:#x}-{spans[rels[-1]][1]:#x}")
    print(f"{header}: {len(funcs)} prototypes")
    for d in notes:
        print(f"merged: {d}")
    if args.dry_run:
        return 0

    (ROOT / header).parent.mkdir(parents=True, exist_ok=True)
    (ROOT / header).write_text("\n".join(hdr))
    (ROOT / args.out).write_text(re.sub(r"\n{3,}", "\n\n", "\n".join(c)).rstrip() + "\n")
    first = True
    kept = []
    for name, lines in entries:
        if name in rels:
            if first:
                kept.append((args.out, ["    complete", *merged, ""]))
                first = False
            continue
        kept.append((name, lines))
    write_delinks(delinks, kept)
    for rel in rels:
        if rel != args.out:
            subprocess.run(["git", "rm", "-q", "-f", rel], cwd=ROOT, check=True)
    # Callers lose their copies and include the header (decls.py declare,
    # which leaves a copy where the include would clash, and says so).
    import decls
    decls.ROOT, decls.INCLUDE = ROOT, ROOT / "include"
    decls.EXCEPTIONS = ROOT / "config/AWRE/local_decls.txt"
    decls.declare(str(Path(header).relative_to("include")), names)
    if (ROOT / "tools/format.py").exists():
        changed = subprocess.run(["git", "diff", "--name-only", "--", "src", "include"], cwd=ROOT,
                                 capture_output=True, text=True).stdout.split()
        subprocess.run([sys.executable, ROOT / "tools/format.py", args.out, header,
                        *[f for f in changed if f.endswith((".c", ".h")) and (ROOT / f).exists()]],
                       cwd=ROOT, check=False)
    return 0


if __name__ == "__main__":
    sys.exit(main())
