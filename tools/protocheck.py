#!/usr/bin/env python3
"""Find function declarations that disagree with the function's definition.

    python3 tools/protocheck.py [--all] [--pointers] [FILE.c ...]

A C file often declares the functions it calls at the top, copied from an
earlier guess. When the callee is decompiled later with more arguments, the
old declaration still compiles, and the only trace in the caller is a
register or scheduling difference. Four partials matched on 2026-10-11 once
such a declaration was fixed, and FS_LoadOverlayInfo after them.

For each started, unlinked C file (progress.unlinked()), or
only the FILEs given, this reports:

  count   a file-scope declaration with another number of arguments than
          the reference
  call    a call that passes fewer arguments than the reference (or more,
          when it is not variadic)
  width   another integer type for an argument or the return value (int
          and u8, s16 and u16, an enum and int): the caller may narrow or
          extend the value with other instructions
  ret     void against a value, or a pointer against an integer
  pointer another pointer type only; listed only with --pointers, since it
          does not change the code

Each one is compared with the definition in src/ (any file, complete or not; a
static definition counts only in its own file), or else a prototype in
include/. ARM7 files (src/arm7/) and ARM9 files only see definitions of their
own CPU. Types are compared after resolving typedefs (u32 and unsigned int
agree, `struct Foo` and Foo agree) and ignoring `const`; an enum and int
differ, since mwcc may give an enum a smaller type.

--all checks every C file, and also reports header prototypes that disagree
with the definition, and functions declared with different argument counts
in two files.

Exits 1 when it reports anything. The parser is a scanner for the C written
in this repo, not a full C parser: function pointers, K&R declarations and
block-scope declarations are skipped.
"""
import argparse
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
IDENT = re.compile(r"[A-Za-z_]\w*")
KEYWORDS = {"if", "for", "while", "switch", "return", "sizeof", "do", "else", "case", "goto",
            "default", "break", "continue", "typedef", "asm", "__asm", "defined"}
BUILTIN = {"void", "char", "short", "int", "long", "float", "double", "signed", "unsigned", "_Bool"}
QUALIFIERS = {"const", "volatile", "register", "restrict"}
STORAGE = {"extern", "static", "inline", "__inline", "asm", "__asm"}


@dataclass
class Sig:
    name: str
    ret: str
    params: list          # raw parameter types, names removed; None when unknown: `f()`
    variadic: bool
    kind: str             # "proto" or "def"
    static: bool
    path: str
    line: int

    def where(self) -> str:
        return f"{self.path}:{self.line}"


@dataclass
class Call:
    name: str
    nargs: int
    path: str
    line: int


@dataclass
class Unit:
    path: str
    sigs: list = field(default_factory=list)
    calls: list = field(default_factory=list)
    typedefs: dict = field(default_factory=dict)


# ---------------------------------------------------------------- scanning

def blank(text: str) -> str:
    """Comments, string and character literals and preprocessor lines become
    spaces; newlines stay, so offsets give the same line numbers."""
    out = list(text)
    i, n = 0, len(text)
    bol = True
    while i < n:
        c = text[i]
        if bol and c == "#" or (bol and c in " \t" and text[i:].lstrip(" \t").startswith("#")):
            j = i
            while j < n:
                if text[j] == "\n" and text[j - 1] != "\\":
                    break
                j += 1
            for k in range(i, j):
                if out[k] != "\n":
                    out[k] = " "
            i = j
            continue
        bol = c == "\n" or (bol and c in " \t")
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            for k in range(i + 1, min(j, n)):     # keep the quotes, blank the inside
                if out[k] != "\n":
                    out[k] = " "
            i = j + 1
            continue
        else:
            i += 1
            continue
        for k in range(i, min(j, n)):
            if out[k] != "\n":
                out[k] = " "
        i = j
    return "".join(out)


def close(s: str, i: int) -> int:
    """s[i] is ( [ or {; index just past its match (s is already blanked)."""
    pairs = {"(": ")", "[": "]", "{": "}"}
    stack = [pairs[s[i]]]
    i += 1
    while i < len(s) and stack:
        c = s[i]
        if c in pairs:
            stack.append(pairs[c])
        elif c == stack[-1]:
            stack.pop()
        i += 1
    return i


def split_args(s: str) -> list:
    parts, depth, cur = [], 0, []
    for c in s:
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == "," and depth == 0:
            parts.append("".join(cur))
            cur = []
        else:
            cur.append(c)
    parts.append("".join(cur))
    return [p.strip() for p in parts]


def param_type(p: str) -> str:
    """`const FSFile *file` -> `const FSFile *`; `u32` stays; `int a[4]` -> `int *`."""
    p = " ".join(p.split())
    arr = re.search(r"\s*(\[[^\]]*\]\s*)+$", p)
    if arr:
        p = p[:arr.start()] + " *"
        return p.strip()
    m = re.match(r"^(.*?[\s\*])([A-Za-z_]\w*)$", p)
    if m and m.group(2) not in BUILTIN and m.group(2) not in QUALIFIERS:
        head = m.group(1).strip()
        if IDENT.search(head.replace("*", " ")) and any(w not in QUALIFIERS for w in IDENT.findall(head)):
            return head
    return p


def parse_head(head: str):
    """`static inline BOOL Foo` -> (name, return type, static), or None."""
    words = head.replace("*", " * ").split()
    if len(words) < 2 or "typedef" in words or "=" in head or any(c in head for c in "()[]"):
        return None
    name = words[-1]
    if not IDENT.fullmatch(name) or name in KEYWORDS or name in BUILTIN:
        return None
    static = "static" in words
    ret = [w for w in words[:-1] if w not in STORAGE]
    if not ret or not all(w == "*" or IDENT.fullmatch(w) for w in ret):
        return None
    return name, " ".join(ret).replace(" *", "*").replace("*", " *").strip(), static


def parse_params(inner: str, kind: str):
    s = " ".join(inner.split())
    if s == "":
        return ([] if kind == "def" else None), False
    if s == "void":
        return [], False
    parts = split_args(s)
    variadic = parts[-1] == "..."
    if variadic:
        parts = parts[:-1]
    if any("(" in p for p in parts):
        return None, variadic          # function pointer parameter: count only
    return [param_type(p) for p in parts], variadic


TYPEDEF_SIMPLE = re.compile(r"\btypedef\s+([^;{}()]+?)\s+\**\s*([A-Za-z_]\w*)\s*;")
TYPEDEF_POINTER = re.compile(r"\btypedef\s+[^;{}()]+?\*\s*([A-Za-z_]\w*)\s*;")
TYPEDEF_TAGGED = re.compile(r"\btypedef\s+(struct|union|enum)\b[^{;]*\{")


def typedefs(s: str) -> dict:
    out = {}
    for m in TYPEDEF_SIMPLE.finditer(s):
        stars = m.group(0).count("*")
        out[m.group(2)] = m.group(1).strip() + " *" * stars
    for m in TYPEDEF_TAGGED.finditer(s):
        end = close(s, m.end() - 1)
        while end < len(s) and s[end].isspace():
            end += 1
        n = IDENT.match(s, end)
        if n:
            out[n.group(0)] = ("enum " if m.group(1) == "enum" else "") + n.group(0)
    return out


def scan(path: str, text: str) -> Unit:
    s = blank(text)
    u = Unit(path, typedefs=typedefs(s))
    line_at = _line_index(s)
    i, n, seg = 0, len(s), 0
    while i < n:
        c = s[i]
        if c == ";":
            seg = i + 1
        elif c == "{":
            i = close(s, i)               # struct body or initialiser: same segment
            continue
        elif c == "(":
            end = close(s, i)
            j = end
            while j < n and s[j].isspace():
                j += 1
            nxt = s[j] if j < n else ""
            got = parse_head(s[seg:i]) if nxt in ";{" else None
            if got:
                name, ret, static = got
                kind = "def" if nxt == "{" else "proto"
                params, variadic = parse_params(s[i + 1:end - 1], kind)
                u.sigs.append(Sig(name, ret, params, variadic, kind, static, path, line_at(i)))
            if nxt == "{":
                body_end = close(s, j)
                if got:
                    u.calls += calls_in(s, j, body_end, path, line_at)
                i, seg = body_end, body_end
                continue
            i = end
            continue
        i += 1
    return u


CALL = re.compile(r"(?<![\w.>])([A-Za-z_]\w*)\s*\(")


def calls_in(s: str, start: int, end: int, path: str, line_at) -> list:
    out = []
    for m in CALL.finditer(s, start, end):
        name = m.group(1)
        if name in KEYWORDS or name in BUILTIN:
            continue
        before = s[max(start, m.start() - 40):m.start()].rstrip()
        prev = IDENT.findall(before[-40:])
        if before.endswith("*") or (prev and before.endswith(prev[-1]) and prev[-1] not in KEYWORDS):
            continue                       # a block-scope declaration, not a call
        e = close(s, m.end() - 1)
        inner = s[m.end():e - 1]
        nargs = 0 if not inner.strip() else len(split_args(inner))
        out.append(Call(name, nargs, path, line_at(m.start())))
    return out


def _line_index(s: str):
    starts = [0] + [m.end() for m in re.finditer("\n", s)]

    def line_at(off: int) -> int:
        lo, hi = 0, len(starts)
        while lo + 1 < hi:
            mid = (lo + hi) // 2
            if starts[mid] <= off:
                lo = mid
            else:
                hi = mid
        return lo + 1
    return line_at


# ---------------------------------------------------------------- comparing

def canon(t: str, tds: dict) -> str:
    words = []
    for w in t.replace("*", " * ").split():
        if w in QUALIFIERS - {"volatile"} or w in ("struct", "union"):
            continue
        words.append(w)
    out, seen = [], 0
    for w in words:
        while w in tds and seen < 20 and tds[w] != w:
            nw = tds[w]
            if nw.startswith("enum "):
                w = nw
                break
            sub = canon(nw, {k: v for k, v in tds.items() if k != w})
            w = sub
            seen += 1
        out.append(w)
    s = " ".join(out).split()
    base = [w for w in s if w in BUILTIN]
    if base and len(base) == len([w for w in s if w not in ("*", "volatile")]):
        unsigned = "unsigned" in base
        kind = next((w for w in ("char", "short", "float", "double", "void", "_Bool") if w in base), "int")
        if kind == "char" and "signed" in base and not unsigned:
            kind = "signed char"
        rest = [w for w in s if w not in BUILTIN]
        s = (["unsigned"] if unsigned else []) + [kind] + rest
    s = [w for w in s if w != "enum"]
    return " ".join(s).replace(" *", "*").replace("*", " *")


def arity(sig: Sig) -> str:
    if sig.params is None:
        return "?"
    return f"{len(sig.params)}{'+' if sig.variadic else ''}"


def compare(local: Sig, ref: Sig, tds: dict, ref_tds: dict) -> list:
    out = []
    if local.params is not None and ref.params is not None:
        if len(local.params) != len(ref.params) or local.variadic != ref.variadic:
            out.append(("count", f"{local.name} declared with {arity(local)} arguments, "
                                 f"{ref.kind} has {arity(ref)}"))
            return out
    ra, rb = canon(local.ret, tds), canon(ref.ret, ref_tds)
    if ra != rb:
        out.append((type_kind(ra, rb, True), f"{local.name} returns {local.ret}, {ref.kind} returns {ref.ret}"))
    if local.params and ref.params:
        for k, (a, b) in enumerate(zip(local.params, ref.params), 1):
            ca, cb = canon(a, tds), canon(b, ref_tds)
            if ca != cb:
                out.append((type_kind(ca, cb, False), f"{local.name} argument {k} is {a}, {ref.kind} has {b}"))
    return out


def type_kind(a: str, b: str, is_ret: bool) -> str:
    pa, pb = "*" in a, "*" in b
    if pa and pb:
        return "pointer"
    if is_ret and (pa or pb or "void" in (a, b)):
        return "ret"
    if pa or pb:
        return "ret" if is_ret else "width"
    return "width"


def cpu(path: str) -> str:
    return "arm7" if path.startswith("src/arm7/") else "arm9"


class Index:
    def __init__(self, units: dict, headers: dict):
        self.units = units
        self.headers = headers
        self.header_tds = {}
        for h in headers.values():
            self.header_tds.update(h.typedefs)
        self.defs = {}                       # (cpu, name) -> [Sig]
        for u in units.values():
            for s in u.sigs:
                if s.kind == "def" and not s.static:
                    self.defs.setdefault((cpu(u.path), s.name), []).append(s)
        self.protos = {}                     # name -> [Sig] from include/
        for h in headers.values():
            for s in h.sigs:
                if s.kind == "proto":
                    self.protos.setdefault(s.name, []).append(s)

    def tds(self, path: str) -> dict:
        u = self.units.get(path) or self.headers.get(path)
        return {**self.header_tds, **(u.typedefs if u else {})}

    def reference(self, unit: Unit, name: str):
        own = [s for s in unit.sigs if s.kind == "def" and s.name == name]
        if own:
            return own[0]
        defs = self.defs.get((cpu(unit.path), name), [])
        if len(defs) == 1:
            return defs[0]
        if len(defs) > 1:
            return None                      # two definitions: leave it alone
        protos = [p for p in self.protos.get(name, []) if p.params is not None]
        return protos[0] if len(protos) == 1 else None

    def check(self, unit: Unit) -> list:
        out, seen = [], set()
        tds = self.tds(unit.path)
        for s in unit.sigs:
            if s.kind != "proto" or (s.name, s.line) in seen:
                continue
            seen.add((s.name, s.line))
            ref = self.reference(unit, s.name)
            if ref is None or ref is s:
                continue
            for kind, msg in compare(s, ref, tds, self.tds(ref.path)):
                out.append((s.where(), kind, msg, ref.where()))
        for c in unit.calls:
            ref = self.reference(unit, c.name)
            if ref is None or ref.params is None:
                continue
            n = len(ref.params)
            if c.nargs < n or (c.nargs > n and not ref.variadic):
                out.append((f"{c.path}:{c.line}", "call",
                            f"{c.name} called with {c.nargs} arguments, {ref.kind} has {arity(ref)}",
                            ref.where()))
        return out

    def headers_vs_defs(self) -> list:
        out = []
        for h in self.headers.values():
            for s in h.sigs:
                if s.kind != "proto":
                    continue
                for c in ("arm9", "arm7"):
                    defs = self.defs.get((c, s.name), [])
                    if len(defs) == 1:
                        for kind, msg in compare(s, defs[0], self.tds(h.path), self.tds(defs[0].path)):
                            out.append((s.where(), kind, msg, defs[0].where()))
        return out

    def counts_across_files(self) -> list:
        """The same function declared with different argument counts in two files."""
        by = {}
        for u in self.units.values():
            for s in u.sigs:
                if s.kind == "proto" and s.params is not None and not s.static:
                    by.setdefault((cpu(u.path), s.name), {}).setdefault(arity(s), s)
        out = []
        for (_, name), forms in sorted(by.items()):
            if len(forms) > 1:
                first, *rest = forms.values()
                for s in rest:
                    out.append((s.where(), "count", f"{name} declared with {arity(s)} arguments here and "
                                                     f"{arity(first)} in another file", first.where()))
        return out


# ---------------------------------------------------------------- main

def load(root: Path = ROOT):
    units = {}
    for p in sorted((root / "src").rglob("*.c")):
        rel = p.relative_to(root).as_posix()
        units[rel] = scan(rel, p.read_text(errors="replace"))
    headers = {}
    for p in sorted((root / "include").rglob("*.h")):
        rel = p.relative_to(root).as_posix()
        headers[rel] = scan(rel, p.read_text(errors="replace"))
    return Index(units, headers)


def partial_files() -> list:
    sys.path.insert(0, str(ROOT / "tools"))
    import progress
    return [path for path, _, _ in progress.unlinked()]


def report(index: Index, paths: list, everything: bool, pointers: bool = False) -> list:
    rows = []
    for p in paths:
        if p in index.units:
            rows += index.check(index.units[p])
    if everything:
        rows += index.headers_vs_defs()
        rows += index.counts_across_files()
    seen, out = set(), []
    for r in rows:
        if r not in seen and (pointers or r[1] != "pointer"):
            seen.add(r)
            out.append(r)
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("files", nargs="*")
    ap.add_argument("--all", action="store_true", help="every C file, headers, and cross-file counts")
    ap.add_argument("--pointers", action="store_true", help="also list pointer-only type differences")
    a = ap.parse_args()
    index = load()
    if a.files:
        paths = [Path(f).resolve().relative_to(ROOT).as_posix() for f in a.files]
    elif a.all:
        paths = sorted(index.units)
    else:
        paths = partial_files()
    rows = report(index, paths, a.all, a.pointers)
    for where, kind, msg, ref in rows:
        print(f"{where}: {kind}: {msg} ({ref})")
    return 1 if rows else 0


if __name__ == "__main__":
    sys.exit(main())
