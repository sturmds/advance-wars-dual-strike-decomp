#!/usr/bin/env python3
"""Search source-level variants of one function for a closer match.

    python3 tools/permute.py FILE.c NAME [--budget-seconds N] [--axes decl,stmt,types,flags]
                                         [--seed N] [--jobs N] [--thumb] [--apply]

FILE.c is a started C file (it has a delinks.txt entry and `make` has run, so
build/delinks/<FILE>.o holds the original bytes; build/arm7/delinks/<FILE>.o
for a file under src/arm7/, which compiles with the ARM7 flags). The tool compiles variants
with the build's compiler command and scores each by the number of
instructions that differ from the original, ignoring relocation words:

  decl    order of the leading local declarations. Initialisers are split off
          first (`int a = 0;` becomes `int a;` and `a = 0;`). Up to 120 orders
          are tried exhaustively; beyond that, random sampling and hill climbing.
  stmt    swaps of adjacent simple statements that touch different variables
          and make no calls.
  types   the type of each local: int u32 s32 u16 s16 u8 s8.
  flags   diagnostic only: compiles the best variant with other optimisation
          flags (-O4,s -O3,p ...) and prints their scores. The build's flags
          never change.

The best five variants are printed. The best goes to build/permute/NAME.best.c.
The source file is replaced only with --apply, and only when the best variant
matches in full. Tidy the result by hand and confirm with `fn.py diff`.

Based on khdays-decomp tools/declperm.py, tools/park_permute.py and
tools/allopt.py (CC0 1.0, commit ba2de370; see docs/third-party/khdays-decomp.md).
"""
import argparse
import difflib
import hashlib
import itertools
import os
import random
import re
import resource
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CC = ROOT / ".cache/tools/metroskrew/bin/mwccarm-2.0-72.exe"
BASE_FLAGS = ["-proc", "arm946e", "-interworking", "-nostdinc", "-i", "include", "-msgstyle", "gcc"]
BUILD_OPT = "-O4,p"
DIAG_OPTS = ["-O4,s", "-O3,p", "-O3,s", "-O2,p", "-O4", "-O1,p"]
DELINKS = ROOT / "build/delinks"


def use_arm7_flags() -> None:
    """Switch to ARM7's flags and objects (tools/configure.py, CFLAGS7)."""
    global BASE_FLAGS, BUILD_OPT, DIAG_OPTS, DELINKS
    BASE_FLAGS = ["-proc", "arm7tdmi", "-interworking", "-nostdinc", "-i", "include", "-msgstyle", "gcc"]
    BUILD_OPT = "-O4,s"
    DIAG_OPTS = ["-O4,p", "-O3,s", "-O2,s", "-O1,s"]
    DELINKS = ROOT / "build/arm7/delinks"
TYPES = ["int", "u32", "s32", "u16", "s16", "u8", "s8"]
KEYWORDS = {"return", "goto", "else", "case", "default", "break", "continue", "if", "for", "while",
            "do", "switch", "sizeof", "typedef"}


# ---------------------------------------------------------------- ELF reading

class Elf:
    """Just enough of a 32-bit little-endian ELF object: .text, symbols, relocations."""

    def __init__(self, data: bytes):
        self.data = data
        shoff, = struct.unpack_from("<I", data, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2e)
        secs = []
        for i in range(shnum):
            secs.append(struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize))
        names = secs[shstrndx]
        self.secs = {}
        for i, s in enumerate(secs):
            n = self._cstr(names[4] + s[0])
            self.secs.setdefault(n, (i, s))
        self.text = b""
        self.text_idx = -1
        if ".text" in self.secs:
            self.text_idx, s = self.secs[".text"]
            self.text = data[s[4]:s[4] + s[5]]
        sym = self.secs.get(".symtab")
        self.symbols = []
        if sym:
            _, s = sym
            strtab = secs[s[6]]
            for off in range(s[4], s[4] + s[5], 16):
                nm, val, size, info, other, shndx = struct.unpack_from("<IIIBBH", data, off)
                self.symbols.append((self._cstr(strtab[4] + nm), val, size, shndx))
        self.relocs = {}
        for rname in (".rel.text", ".rela.text"):
            if rname in self.secs:
                _, s = self.secs[rname]
                step = s[9] or 8
                for off in range(s[4], s[4] + s[5], step):
                    o, info = struct.unpack_from("<II", data, off)
                    self.relocs[o] = (info & 0xff, self.symbols[info >> 8][0])

    def _cstr(self, off):
        end = self.data.index(b"\0", off)
        return self.data[off:end].decode()

    def function(self, name):
        for n, val, size, shndx in self.symbols:
            if n == name and shndx == self.text_idx and size:
                return val & ~1, size
        return None


def function_words(elf: Elf, name: str, unit: int):
    """Words of one function; relocated words become ('R', type, symbol)."""
    f = elf.function(name)
    if not f:
        return None
    start, size = f
    out = []
    i = 0
    while i < size:
        o = start + i
        hit = None
        for k in range(unit):
            if o + k in elf.relocs:
                hit = elf.relocs[o + k]
        if o in elf.relocs or hit:
            out.append(("R",) + (hit or elf.relocs[o]))
        else:
            out.append(elf.text[o:o + unit])
        i += unit
    return out


def score_words(orig, ours):
    """(differing, total): unmatched words on the larger side of an alignment."""
    if ours is None:
        return None
    sm = difflib.SequenceMatcher(None, orig, ours, autojunk=False)
    matched = sum(b.size for b in sm.get_matching_blocks())
    total = max(len(orig), len(ours))
    return total - matched, total


# ---------------------------------------------------------------- C text scanning

def skip_trivia(s, i):
    n = len(s)
    while i < n:
        if s[i].isspace():
            i += 1
        elif s.startswith("/*", i):
            j = s.find("*/", i + 2)
            i = n if j < 0 else j + 2
        elif s.startswith("//", i):
            j = s.find("\n", i)
            i = n if j < 0 else j + 1
        else:
            break
    return i


def skip_literal(s, i):
    q = s[i]
    i += 1
    while i < len(s) and s[i] != q:
        i += 2 if s[i] == "\\" else 1
    return i + 1


def match_close(s, i):
    """i at an opening ( [ {; returns the index just past the matching close."""
    pairs = {"(": ")", "[": "]", "{": "}"}
    stack = [pairs[s[i]]]
    i += 1
    while i < len(s) and stack:
        c = s[i]
        if s.startswith("/*", i) or s.startswith("//", i):
            i = skip_trivia(s, i)
            continue
        if c in "\"'":
            i = skip_literal(s, i)
            continue
        if c in pairs:
            stack.append(pairs[c])
        elif c == stack[-1]:
            stack.pop()
        i += 1
    return i


def stmt_end(s, i, limit):
    """Index just past the statement starting at i (non-trivia), or -1 when unsure."""
    m = re.compile(r"[A-Za-z_]\w*").match(s, i)
    word = m.group(0) if m else ""
    if s[i] == "{":
        return match_close(s, i)
    if word in ("if", "while", "for", "switch"):
        j = skip_trivia(s, m.end())
        if j >= limit or s[j] != "(":
            return -1
        j = skip_trivia(s, match_close(s, j))
        if j >= limit:
            return -1
        if word == "while" and s[j] == ";":
            return j + 1
        e = stmt_end(s, j, limit)
        if e < 0:
            return -1
        if word == "if":
            k = skip_trivia(s, e)
            if re.compile(r"else\b").match(s, k):
                k = skip_trivia(s, k + 4)
                e2 = stmt_end(s, k, limit)
                return e2
        return e
    if word == "do":
        j = skip_trivia(s, m.end())
        e = stmt_end(s, j, limit)
        if e < 0:
            return -1
        k = skip_trivia(s, e)
        if not s.startswith("while", k):
            return -1
        k = skip_trivia(s, k + 5)
        k = match_close(s, k)
        k = skip_trivia(s, k)
        return k + 1 if k < limit and s[k] == ";" else -1
    while i < limit:
        c = s[i]
        if c == ";":
            return i + 1
        if c in "([{":
            i = match_close(s, i)
            continue
        if c in "\"'":
            i = skip_literal(s, i)
            continue
        if s.startswith("/*", i) or s.startswith("//", i):
            i = skip_trivia(s, i)
            continue
        i += 1
    return -1


def split_top(text, sep=","):
    parts, depth, cur = [], 0, ""
    for c in text:
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == sep and depth == 0:
            parts.append(cur)
            cur = ""
        else:
            cur += c
    parts.append(cur)
    return parts


DECL_PART = re.compile(r"^\s*(?P<stars>\*+\s*)?(?P<name>[A-Za-z_]\w*)(?P<arr>(\s*\[[^\]]*\])*)\s*(=\s*(?P<init>.+?))?\s*$", re.S)
DECL_FIRST = re.compile(r"^(?P<type>[A-Za-z_][\w ]*?)\s*(?P<rest>\*+\s*[A-Za-z_].*|[A-Za-z_]\w*(?:\s*\[.*|\s*=.*|\s*,.*)?)$", re.S)


class Decl:
    def __init__(self, indent, type_, stars, name, arr, init):
        self.indent, self.type, self.stars, self.name, self.arr, self.init = indent, type_, stars, name, arr, init

    def line(self):
        stars = self.stars.replace(" ", "")
        sp = " " if not stars else " "
        return f"{self.indent}{self.type}{sp}{stars}{self.name}{self.arr};\n"

    def assign(self):
        return f"{self.indent}{self.name} = {self.init};\n"


def parse_decl_line(line):
    """List of Decl for one declaration line, or None when it is not one."""
    m = re.match(r"^(\s+)(.*);\s*$", line, re.S)
    if not m or "\n" in m.group(2):
        return None
    indent, body = m.group(1), m.group(2)
    first = body
    mt = re.match(r"^((?:(?:const|register|static|volatile)\s+)*[A-Za-z_]\w*(?:\s+[A-Za-z_]\w*)*?)\s+(\**\s*[A-Za-z_]\w*.*)$", body, re.S)
    # `unsigned int x` : the last word of the leading words is the name unless it is a keyword type word
    if not mt:
        return None
    type_, rest = mt.group(1), mt.group(2)
    words = type_.split()
    if words[0] in KEYWORDS:
        return None
    out = []
    for part in split_top(rest):
        pm = DECL_PART.match(part)
        if not pm:
            return None
        init = pm.group("init")
        if init and init.lstrip().startswith("{"):
            return None
        out.append(Decl(indent, type_, (pm.group("stars") or "").strip(), pm.group("name"),
                        (pm.group("arr") or "").replace(" ", ""), init))
    if any(re.search(r"[=(]", p) is None and False for p in [first]):
        return None
    return out


class Func:
    """Positions of one function inside a source text."""

    def __init__(self, text, name):
        self.text = text
        m = None
        for cand in re.finditer(r"^[A-Za-z_][^;{}()]*?\b" + re.escape(name) + r"\s*\(", text, re.M):
            m = cand
            break
        if not m:
            raise SystemExit(f"no definition of {name} in the file")
        j = match_close(text, text.index("(", m.end() - 1))
        j = skip_trivia(text, j)
        if text[j] != "{":
            raise SystemExit(f"{name}: expected a body")
        self.open = j
        self.close = match_close(text, j) - 1
        # leading declaration block
        pos = text.index("\n", j) + 1 if "\n" in text[j:j + 80] else j + 1
        self.decl_start = pos
        self.decls = []
        k = pos
        while k < self.close:
            eol = text.find("\n", k)
            if eol < 0 or eol > self.close:
                break
            line = text[k:eol + 1]
            if not line.strip():
                k = eol + 1
                continue
            ds = parse_decl_line(line)
            if ds is None:
                break
            self.decls.extend(ds)
            k = eol + 1
            self.decl_end = k
        if not self.decls:
            self.decl_end = pos
        # trailing blank lines are left to the rest of the body
        self.body_start = self.decl_end


def render_decls(f: Func, order, types=None):
    ds = [f.decls[i] for i in order]
    out = ""
    for d in ds:
        t = d.type
        if types and d.name in types:
            t = types[d.name]
        out += Decl(d.indent, t, d.stars, d.name, d.arr, None).line()
    inits = "".join(d.assign() for d in f.decls if d.init)
    return f.text[:f.decl_start] + out + inits + f.text[f.decl_end:]


def normalise(text, name):
    f = Func(text, name)
    if not f.decls:
        return text
    return render_decls(f, list(range(len(f.decls))))


# ---------------------------------------------------------------- statement swaps

CALL = re.compile(r"\b(?!sizeof\b|if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*\s*\(")
IDENT = re.compile(r"\b[A-Za-z_]\w*\b")
ASSIGN = re.compile(r"(?<![=!<>+\-*/%&|^])(?:<<|>>|[-+*/%&|^])?=(?!=)|\+\+|--")
MEM = re.compile(r"\[|->|\.|(?<![\w)\]])\s*\*")


def strip_comments(s):
    s = re.sub(r"/\*.*?\*/", " ", s, flags=re.S)
    return re.sub(r"//[^\n]*", " ", s)


def info(stmt):
    """(swappable, idents, writes, memwrite, memaccess, call) for one statement."""
    t = strip_comments(stmt).strip()
    first = IDENT.match(t)
    w = first.group(0) if first else ""
    if t.startswith("{") or w in KEYWORDS or not t.endswith(";"):
        return None
    if parse_decl_line("    " + t + "\n") is not None:
        ids = set(IDENT.findall(t))
        return True, ids, ids, False, False, False
    ids = set(IDENT.findall(t))
    call = bool(CALL.search(t))
    writes, memwrite = set(), False
    for m in ASSIGN.finditer(t):
        lhs = t[:m.start()] if m.group(0) not in ("++", "--") else t[:m.start()] + t[m.end():]
        if m.group(0) in ("++", "--") and not lhs.strip():
            lhs = t[m.end():]
        lhs = lhs.split(",")[-1].split("=")[-1]
        if MEM.search(lhs) or lhs.strip().startswith("*"):
            memwrite = True
        li = IDENT.findall(lhs)
        if li:
            writes.add(li[0])
    memaccess = bool(MEM.search(t))
    return True, ids, writes, memwrite, memaccess, call


def independent(a, b):
    ia, ib = info(a), info(b)
    if not ia or not ib:
        return False
    _, ids_a, w_a, mw_a, ma_a, c_a = ia
    _, ids_b, w_b, mw_b, ma_b, c_b = ib
    if c_a or c_b:
        return False
    if (mw_a and (ma_b or mw_b)) or (mw_b and (ma_a or mw_a)):
        return False
    if w_a & ids_b or w_b & ids_a:
        return False
    return True


def blocks_of(f: Func):
    """(start, end) text spans of statement lists: the body after the declarations and nested blocks."""
    text = f.text
    spans = [(f.body_start, f.close)]
    i = f.body_start
    stack = []
    while i < f.close:
        c = text[i]
        if text.startswith("/*", i) or text.startswith("//", i):
            i = skip_trivia(text, i)
            continue
        if c in "\"'":
            i = skip_literal(text, i)
            continue
        if c == "{":
            before = text[:i].rstrip()
            if not before.endswith("="):
                spans.append((i + 1, match_close(text, i) - 1))
        i += 1
    return spans


def statements(text, start, end):
    out = []
    i = skip_trivia(text, start)
    while i < end:
        e = stmt_end(text, i, end)
        if e < 0 or e > end:
            return None
        out.append((i, e))
        i = skip_trivia(text, e)
    return out


def stmt_swaps(text, name):
    f = Func(text, name)
    outs = []
    for s, e in blocks_of(f):
        sts = statements(text, s, e)
        if not sts:
            continue
        for (a0, a1), (b0, b1) in zip(sts, sts[1:]):
            A, B = text[a0:a1], text[b0:b1]
            gap = text[a1:b0]
            if independent(A, B):
                outs.append(text[:a0] + B + gap + A + text[b1:])
    return outs


# ---------------------------------------------------------------- compile and score

class Scorer:
    def __init__(self, path: Path, name: str, thumb: bool, jobs: int):
        self.path, self.name = path, name
        self.unit = 2 if thumb else 4
        ref = DELINKS / path.resolve().relative_to(ROOT).with_suffix(".o")
        if not ref.exists():
            sys.exit(f"{ref} not found: add a delinks.txt entry and run make first")
        self.orig = function_words(Elf(ref.read_bytes()), name, self.unit)
        if self.orig is None:
            sys.exit(f"{name} not found in {ref}")
        self.tmp = Path(tempfile.mkdtemp(prefix="permute-", dir=ROOT / "build"))
        self.cache = {}
        self.serial = itertools.count()
        self.count = 0
        self.jobs = jobs
        self.srcdir = path.resolve().parent
        self.pool = ThreadPoolExecutor(max_workers=jobs)

    def close(self):
        self.pool.shutdown()
        shutil.rmtree(self.tmp, ignore_errors=True)

    def _one(self, text, opt):
        key = (hashlib.sha1(text.encode()).hexdigest(), opt)
        if key in self.cache:
            return self.cache[key]
        src = self.tmp / f"v{next(self.serial)}.c"
        obj = src.with_suffix(".o")
        src.write_text(text)
        r = subprocess.run([str(CC), "-c", opt, *BASE_FLAGS, "-i", str(self.srcdir), "-o", str(obj), str(src)],
                           cwd=ROOT, capture_output=True, text=True)
        sc = None
        if r.returncode == 0 and obj.exists():
            try:
                sc = score_words(self.orig, function_words(Elf(obj.read_bytes()), self.name, self.unit))
            except (ValueError, IndexError, struct.error):
                sc = None
        for p in (src, obj):
            p.unlink(missing_ok=True)
        self.cache[key] = sc
        self.count += 1
        return sc

    def many(self, texts, opt=None):
        opt = opt or BUILD_OPT
        unique = list(dict.fromkeys(texts))
        got = dict(zip(unique, self.pool.map(lambda t: self._one(t, opt), unique)))
        return [got[t] for t in texts]


class Best:
    def __init__(self, keep=5):
        self.keep = keep
        self.items = {}

    def add(self, text, sc):
        if sc is None:
            return
        self.items[text] = sc
        top = sorted(self.items.items(), key=lambda kv: (kv[1][0], len(kv[0])))[: self.keep]
        self.items = dict(top)

    def ranked(self):
        return sorted(self.items.items(), key=lambda kv: (kv[1][0], len(kv[0])))


def key(sc):
    return (10 ** 9, 0) if sc is None else (sc[0], 0)


# ---------------------------------------------------------------- axes

def decl_axis(S, best, cur, curs, name, deadline, rng):
    f = Func(cur, name)
    n = len(f.decls)
    if n < 2:
        return cur, curs
    perms = list(itertools.permutations(range(n))) if n <= 5 else None
    if perms:
        texts = [render_decls(f, list(p)) for p in perms]
        scs = S.many(texts)
        for t, sc in zip(texts, scs):
            best.add(t, sc)
            if sc is not None and key(sc) < key(curs):
                cur, curs = t, sc
        return cur, curs
    # sample, then hill climb on pair swaps, restarting from random orders
    order = list(range(n))
    while time.time() < deadline and curs[0] > 0:
        samples = [rng.sample(range(n), n) for _ in range(S.jobs * 4)]
        texts = [render_decls(f, o) for o in samples]
        scs = S.many(texts)
        for t, sc in zip(texts, scs):
            best.add(t, sc)
        pairs = [(key(sc), o) for sc, o in zip(scs, samples) if sc]
        if not pairs:
            continue
        pairs.sort(key=lambda x: x[0])
        for _, o in pairs[:2]:
            o = list(o)
            osc = None
            while time.time() < deadline:
                nbrs = []
                for i, j in itertools.combinations(range(n), 2):
                    p = o[:]
                    p[i], p[j] = p[j], p[i]
                    nbrs.append(p)
                texts = [render_decls(f, p) for p in nbrs]
                scs = S.many(texts)
                for t, sc in zip(texts, scs):
                    best.add(t, sc)
                    if sc is not None and key(sc) < key(curs):
                        cur, curs = t, sc
                cand = min(((key(sc), p) for sc, p in zip(scs, nbrs)), key=lambda x: x[0])
                if osc is not None and cand[0] >= osc:
                    break
                osc, o = cand[0], cand[1]
        f = Func(cur, name)
        n = len(f.decls)
    return cur, curs


def climb(S, best, cur, curs, neighbours, deadline):
    while time.time() < deadline and curs[0] > 0:
        nbrs = neighbours(cur)
        if not nbrs:
            break
        scs = S.many(nbrs)
        moved = False
        for t, sc in zip(nbrs, scs):
            best.add(t, sc)
        cands = [(key(sc), t, sc) for t, sc in zip(nbrs, scs) if sc is not None]
        if not cands:
            break
        k, t, sc = min(cands, key=lambda x: x[0])
        if k < key(curs):
            cur, curs, moved = t, sc, True
        if not moved:
            break
    return cur, curs


def type_neighbours(name):
    def gen(text):
        f = Func(text, name)
        out = []
        for i, d in enumerate(f.decls):
            if d.stars or d.arr or d.type not in TYPES:
                continue
            for t in TYPES:
                if t != d.type:
                    out.append(render_decls(f, list(range(len(f.decls))), {d.name: t}))
        return out
    return gen


def types_axis(S, best, cur, curs, name, deadline, rng):
    cur, curs = climb(S, best, cur, curs, type_neighbours(name), deadline)
    # pairs of locals changed together, while time remains
    f = Func(cur, name)
    idx = [d.name for d in f.decls if not d.stars and not d.arr and d.type in TYPES]
    for a, b in itertools.combinations(idx, 2):
        if time.time() > deadline or curs[0] == 0:
            break
        f = Func(cur, name)
        texts = [render_decls(f, list(range(len(f.decls))), {a: ta, b: tb})
                 for ta in TYPES for tb in TYPES]
        scs = S.many(texts)
        for t, sc in zip(texts, scs):
            best.add(t, sc)
            if sc is not None and key(sc) < key(curs):
                cur, curs = t, sc
    return cur, curs


def stmt_axis(S, best, cur, curs, name, deadline, rng):
    return climb(S, best, cur, curs, lambda t: stmt_swaps(t, name), deadline)


def flags_axis(S, best, cur, curs, name):
    print(f"  flags (diagnostic only; the build uses {BUILD_OPT}):")
    for opt in [BUILD_OPT] + DIAG_OPTS:
        sc = S.many([cur], opt)[0]
        print(f"    {opt:8s} {fmt(sc)}")


def mutate(text, name, rng):
    """One random change on any enabled axis (decl swap, statement swap, type)."""
    f = Func(text, name)
    kind = rng.choice(["decl", "stmt", "types"])
    if kind == "decl" and len(f.decls) >= 2:
        o = list(range(len(f.decls)))
        i, j = rng.sample(o, 2)
        o[i], o[j] = o[j], o[i]
        return render_decls(f, o)
    if kind == "types":
        nb = type_neighbours(name)(text)
        if nb:
            return rng.choice(nb)
    sw = stmt_swaps(text, name)
    return rng.choice(sw) if sw else text


def mix_phase(S, best, cur, curs, name, deadline, rng):
    """Random walk over all axes together; plateaus are allowed."""
    stale = 0
    while time.time() < deadline and curs[0] > 0 and stale < 40:
        seen = S.count
        pool = [t for t, _ in best.ranked()] or [cur]
        base = rng.choice(pool + [cur])
        batch = []
        for _ in range(S.jobs * 3):
            t = base
            for _ in range(rng.choice([1, 2, 3])):
                t = mutate(t, name, rng)
            batch.append(t)
        for t, sc in zip(batch, S.many(batch)):
            best.add(t, sc)
            if sc is not None and key(sc) <= key(curs):
                cur, curs = t, sc
        stale = stale + 1 if S.count == seen else 0
    return cur, curs


def fmt(sc):
    if sc is None:
        return "does not compile / not found"
    return f"{sc[0]} differ of {sc[1]}  ({100.0 * (sc[1] - sc[0]) / sc[1]:.2f}%)"


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("file")
    ap.add_argument("name")
    ap.add_argument("--budget-seconds", type=float, default=120)
    ap.add_argument("--axes", default="decl,stmt,types,flags")
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--jobs", type=int, default=min(8, os.cpu_count() or 2))
    ap.add_argument("--thumb", action="store_true")
    ap.add_argument("--apply", action="store_true", help="replace the source when the best variant matches in full")
    a = ap.parse_args()

    path = Path(a.file).resolve()
    if path.relative_to(ROOT).parts[:2] == ("src", "arm7"):
        use_arm7_flags()
    axes = [x for x in a.axes.split(",") if x]
    rng = random.Random(a.seed)
    t0 = time.time()
    S = Scorer(path, a.name, a.thumb, a.jobs)
    try:
        orig_text = path.read_text()
        base_sc = S.many([orig_text])[0]
        print(f"{a.name}: start {fmt(base_sc)}")
        if base_sc is None:
            sys.exit("the starting file does not compile or lacks the function")
        best = Best()
        best.add(orig_text, base_sc)
        cur = normalise(orig_text, a.name)
        curs = S.many([cur])[0]
        if curs is None:
            cur, curs = orig_text, base_sc
        print(f"  normalised (initialisers split): {fmt(curs)}")
        best.add(cur, curs)
        real = [x for x in axes if x != "flags"]
        rounds = 2
        slice_ = a.budget_seconds / max(1, len(real) * rounds)
        helped = {}
        for r in range(rounds):
            before = key(curs)
            for ax in real:
                if curs[0] == 0:
                    break
                deadline = time.time() + slice_
                prev = key(curs)
                fn = {"decl": decl_axis, "stmt": stmt_axis, "types": types_axis}[ax]
                cur, curs = fn(S, best, cur, curs, a.name, deadline, rng)
                print(f"  round {r + 1} {ax:5s}: {fmt(curs)}")
                if key(curs) < prev:
                    helped.setdefault(ax, 0)
                    helped[ax] += prev[0] - curs[0]
            if key(curs) >= before or curs[0] == 0:
                break
        if real and curs[0] > 0 and time.time() < t0 + a.budget_seconds:
            prev = key(curs)
            cur, curs = mix_phase(S, best, cur, curs, a.name, t0 + a.budget_seconds, rng)
            print(f"  mixed walk: {fmt(curs)}")
            if key(curs) < prev:
                helped["mixed"] = prev[0] - curs[0]
        if "flags" in axes:
            flags_axis(S, best, cur, curs, a.name)
        ranked = best.ranked()
        print("  best variants:")
        for i, (t, sc) in enumerate(ranked):
            print(f"    {i + 1}. {fmt(sc)}")
        out = ROOT / "build/permute"
        out.mkdir(exist_ok=True)
        dest = out / f"{a.name}.best.c"
        dest.write_text(ranked[0][0])
        print(f"  best written to {dest.relative_to(ROOT)}")
        if ranked[0][1][0] == 0:
            print("  MATCH: no differing instructions")
            if a.apply:
                path.write_text(ranked[0][0])
                print(f"  {a.file} replaced; check with fn.py diff, then tidy it")
        cpu = resource.getrusage(resource.RUSAGE_CHILDREN)
        me = resource.getrusage(resource.RUSAGE_SELF)
        cpu_s = cpu.ru_utime + cpu.ru_stime + me.ru_utime + me.ru_stime
        print(f"  axes that helped: {', '.join(f'{k} (-{v})' for k, v in helped.items()) or 'none'}")
        print(f"  {S.count} compiles, {time.time() - t0:.0f}s wall, {cpu_s:.0f}s CPU")
    finally:
        S.close()


if __name__ == "__main__":
    main()
