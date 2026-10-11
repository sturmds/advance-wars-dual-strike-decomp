#!/usr/bin/env python3
"""Copy matched C to functions that compile to the same code.

    propagate.py [--dry-run] [--identical-only] [--limit N] [--module M]
                 [--only NAME,...|@FILE] [--report FILE]

Two kinds of candidates, both from build/fnindex.json:

- Identical twins: an unmatched function whose masked hash equals that of a
  function in a `complete` file. The bytes are the same apart from what the
  relocations point at, so the template's C is copied with the relocation
  symbols swapped by position.
- Near twins: same mode, same size, same relocation offsets and the same
  mnemonics before the literal pool, but other immediates or pool
  constants. The differing values are mapped by position too, and written
  into the copy in several ways (render strategies, below).

For each candidate the tool takes only the template's function definition
from its file (comments dropped) and the declarations that definition uses.
A symbol the template reaches through a header (a typed global, a
prototype) gets a declaration of the twin's symbol in the new file, copied
from the header's declaration of the template's symbol; when the twin's
symbol already has a declaration in a header, including that header is
tried first. Twins keep their address names.

Every variant goes through the fast check (fnindex.check). A candidate that
passes is written to src/game/<module>/<name>.c (SDK code in main goes to
src/sdk/<name>.c, ARM7 code to src/arm7/sdk/<name>.c), added with `fn.py add`, compared with `fn.py diff`, and
marked with `fn.py finish` at 100%. Failures are removed again. Matches
found during a run serve as templates for the rest of the run.

--dry-run         check only; write nothing under src/ or config/
--identical-only  skip near twins
--limit N         stop after N matched candidates
--module M        only candidates in module M (main, itcm, ov000, ...). ARM7
                  candidates come only with --module arm7; templates come
                  from every module, and an ARM7 candidate is compiled with
                  the ARM7 flags.
--only NAMES      only these candidates (comma list, or @file with one per line)
--report FILE     JSON results (default build/propagate.json)

Near-twin render strategies, tried in order until one passes: plain values;
also values divided by 4 and 2 (byte offsets written as array indices);
the same while leaving values below 16 alone (a small literal often has
several roles); the same with shift amounts unguarded. Bit-field widths
(`: 8`) and, except in the last strategy, shift amounts (`<< 16`) are never
substituted, and branch targets are left out of the value map.
"""
# Based on khdays-decomp tools/dedupprop.py, tools/genfamily.py,
# tools/nearfam.py and tools/port_template.py
# (CC0 1.0, commit ba2de370; see docs/third-party/khdays-decomp.md).

import argparse

import collections
import json
import re
import resource
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn  # noqa: E402
import fnindex  # noqa: E402

ROOT = fn.ROOT
SCRATCH = ROOT / "build/propagate"
REPORT = ROOT / "build/propagate.json"
TEMPLATES_PER_CANDIDATE = 4

# ---------------------------------------------------------------- C text

KEYWORDS = set("""auto break case char const continue default do double else enum extern float for goto
if inline int long register return short signed sizeof static struct switch typedef union unsigned
void volatile while asm __asm""".split())


def strip_comments(text: str) -> str:
    """Drop comments, leaving string and character literals as they are."""
    return re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*',
                  lambda m: m.group(0) if m.group(0)[0] in "\"'" else " ", text, flags=re.S)


class Chunk:
    """A top-level piece of a C file: a preprocessor line, a declaration
    (ending in `;`) or a function definition."""

    def __init__(self, text: str, kind: str):
        self.text = text.strip()
        self.kind = kind  # "pp", "decl", "func"

    @property
    def name(self) -> str | None:
        """The function a definition or prototype declares."""
        head = self.text.split("{", 1)[0] if self.kind == "func" else self.text
        # A pointer to function: `(*f)(` as the first parenthesis. A later
        # one is a parameter, as in `void g(int x, void (*cb)(void))`.
        m = re.match(r"[^(]*\(\s*\*\s*(\w+)\s*\)\s*\(", head)
        if m and self.kind == "decl":
            return m.group(1)
        # `u8 (*f(int mode))[2]`: skip the `u8 (*` of a declarator.
        m = re.search(r"(\w+)\s*\((?!\s*\*)", head)
        return m.group(1) if m and m.group(1) not in KEYWORDS else None

    def declared(self) -> set[str]:
        """Names a declaration introduces (functions and objects; not types)."""
        t = self.text
        if self.kind != "decl" or re.match(r"(typedef\b|struct\b|union\b|enum\b)[^;(]*\{", t) or t.startswith("typedef"):
            return set()
        t = re.sub(r"\[[^\]]*\]", "[]", t)  # array sizes may hold sizeof(...)
        t = re.sub(r"=[^,;]*", "", t)  # initialisers
        if "(" in t:
            n = Chunk(t, "decl").name
            return {n} if n else set()
        return {m.group(1) for m in re.finditer(r"(\w+)\s*(?:\[\]\s*)*[,;]", t)}

    @property
    def is_type(self) -> bool:
        return self.kind == "decl" and bool(re.match(r"(typedef|struct|union|enum)\b", self.text))

    @property
    def defines_data(self) -> bool:
        """A declaration that allocates storage (not extern, not a prototype)."""
        if self.kind != "decl" or self.is_type or self.text.startswith("extern"):
            return False
        return "(" not in self.text.split("=", 1)[0] or "=" in self.text


def chunks(text: str) -> list[Chunk]:
    out = []
    i, n = 0, len(text)
    start = 0
    depth = 0
    at_line_start = True
    while i < n:
        c = text[i]
        if at_line_start and depth == 0 and text[start:i].strip() == "" and c == "#":
            j = i
            while True:
                j = text.find("\n", j)
                if j == -1:
                    j = n
                    break
                if text[j - 1] != "\\":
                    break
                j += 1
            out.append(Chunk(text[i:j], "pp"))
            i = start = j
            continue
        if c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            i = j + 1
            continue
        at_line_start = c == "\n" or (at_line_start and c in " \t")
        if c in "{(":
            depth += 1
        elif c in "})":
            depth -= 1
            if c == "}" and depth == 0:
                head = text[start:i]
                body_start = head.find("{")
                if body_start != -1 and re.search(r"\)\s*(\[[^\]]*\]\s*)*$", head[:body_start]) and "=" not in head[:body_start]:
                    out.append(Chunk(text[start:i + 1], "func"))
                    start = i + 1
        elif c == ";" and depth == 0:
            out.append(Chunk(text[start:i + 1], "decl"))
            start = i + 1
        i += 1
    if text[start:].strip():
        out.append(Chunk(text[start:], "decl"))
    return out


def words(text: str) -> set[str]:
    return set(re.findall(r"[A-Za-z_]\w*", text))


def rename(text: str, mapping: dict[str, str]) -> str:
    if not mapping:
        return text
    pat = re.compile(r"(?<![\w.])(?<!->)(" + "|".join(re.escape(k) for k in sorted(mapping, key=len, reverse=True)) + r")(?!\w)")
    # Struct fields named like a symbol (after `.` or `->`) stay.
    return pat.sub(lambda m: mapping[m.group(1)], text)


# ---------------------------------------------------------------- headers

class Headers:
    """Declarations of functions and objects in include/, by name."""

    def __init__(self):
        self.decl: dict[str, tuple[str, str]] = {}  # name -> (include path, declaration)
        for h in sorted((ROOT / "include").rglob("*.h")):
            rel = h.relative_to(ROOT / "include").as_posix()
            for ch in chunks(strip_comments(h.read_text())):
                for name in ch.declared():
                    self.decl.setdefault(name, (rel, ch.text))


# ---------------------------------------------------------------- symbols

def elf_relocs(obj: bytes, name: str) -> dict[int, tuple[str, int, str, int]] | None:
    """Offset in the function -> (symbol, addend as seen by the CPU, class,
    symbol kind) for the function `name` in a compiled object."""
    secs, syms, relas = fnindex.read_elf(obj)
    sym = next((s for s in syms if s.name == name and s.shndx not in (0, 0xfff1) and s.kind == 2), None)
    if sym is None:
        return None
    out = {}
    for off, rtype, si, addend in relas.get(sym.shndx, []):
        if sym.value <= off < sym.value + sym.size:
            cls = fnindex.ELF_CLASS.get(rtype, f"elf{rtype}")
            s = syms[si]
            out[off - sym.value] = (s.name, addend + fnindex.PC_BIAS.get(cls, 0), cls, s.kind)
    return out


def symbol_at(dest: str, addr: int) -> str | None:
    for mod in fn.target_module(dest):
        n = fn.all_modules()[mod].syms.get(addr)
        if n:
            return n
    return None


def symbol_map(tpl_relocs, tpl: dict, cand: dict) -> tuple[dict | None, str]:
    """C symbol in the template -> symbol for the candidate, by position."""
    want_t = {r[0]: r for r in tpl["relocs"]}
    want_c = {r[0]: r for r in cand["relocs"]}
    if sorted(want_t) != sorted(want_c):
        return None, "relocation offsets differ"
    m = {tpl["name"]: cand["name"]}
    for off, (sname, addend, cls, kind) in tpl_relocs.items():
        if off not in want_c:
            return None, "template object has an extra relocation"
        if kind == 3 or sname not in fnindex_symbols:
            return None, "template uses file-local data (a string or static)"
        _, _, to, dest, label = want_c[off]
        target = symbol_at(dest, to - addend)
        if target is None:
            return None, f"no symbol at {to - addend:#x} for {sname}{addend:+#x}"
        if m.get(sname, target) != target:
            return None, f"{sname} maps to two symbols"
        m[sname] = target
    # Two template symbols may map to one twin symbol (the twin calls one
    # function where the template calls two); generate() then drops the
    # repeated declaration, and the check decides.
    return {k: v for k, v in m.items() if k != v}, ""


fnindex_symbols: dict = {}

# ---------------------------------------------------------------- near twins

IMM = re.compile(r"#(-?(?:0x[0-9a-f]+|\d+))")
BRANCH = re.compile(r"(b|bl|blx|bx)(eq|ne|cs|cc|hs|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al)?(\.n|\.w)?$")


_operands: dict[str, list] = {}


def operands(f: dict) -> list[tuple[str, str]]:
    """(mnemonic, operands) of the code before the literal pool."""
    if f["name"] not in _operands:
        raw = bytes.fromhex(f["raw"])
        d = fnindex.disassemble(raw, f["addr"], f["addr"], f["addr"] + f["pool"], f["mode"] == "thumb")
        # objdump writes a zero offset as `[r1]`; spell it out so that it
        # lines up with `[r1, #4]` in a twin.
        _operands[f["name"]] = [(mn, re.sub(r"\[(\w+)\]", r"[\1, #0]", ops.split("@")[0]))
                                for _, (mn, ops) in sorted(d.items())]
    return _operands[f["name"]]


def value_map(tpl: dict, cand: dict) -> tuple[dict | None, str]:
    """Template value -> candidate value, from instruction immediates and
    literal-pool words without a relocation."""
    val: dict[int, int] = {}

    def put(x, y):
        if x == y:
            return True
        if val.get(x, y) != y:
            return False
        val[x] = y
        return True
    for (ma, oa), (mb, ob) in zip(operands(tpl), operands(cand)):
        if ma != mb:
            return None, "mnemonics differ"
        if BRANCH.match(ma) or "[pc" in oa:
            continue  # branch targets and pool addresses come from the layout
        ia, ib = IMM.findall(oa), IMM.findall(ob)
        if len(ia) != len(ib):
            return None, "operand counts differ"
        for x, y in zip(ia, ib):
            if not put(int(x, 0), int(y, 0)):
                return None, f"value {x} maps to two values"
    ra, rb = bytes.fromhex(tpl["raw"]), bytes.fromhex(cand["raw"])
    rel = {r[0] for r in tpl["relocs"]}
    for o in range((tpl["pool"] + 3) & ~3, len(ra) - 3, 4):
        if o in rel:
            continue
        x, y = int.from_bytes(ra[o:o + 4], "little"), int.from_bytes(rb[o:o + 4], "little")
        if x >= 1 << 31 and y >= 1 << 31:
            x, y = x - (1 << 32), y - (1 << 32)
        if not put(x, y):
            return None, f"pool word {x:#x} maps to two values"
    return val, ""


NUM = r"(?<![\w.])(-\s*)?(0x[0-9a-fA-F]+|\d+)([uUlL]*)(?![\w.])"
GUARD = str.maketrans("0123456789abcdef", "ghijklmnopqrstuv")  # a guard token holds no digits


def render_values(text: str, val: dict, scaled: bool, guard_shifts: bool, min_val: int) -> str:
    """Write the candidate's values into the template text. Values are
    swapped in one pass, so a result is never mapped a second time."""
    guards: list[str] = []

    def hide(m):
        guards.append(m.group(0))
        return f"\0{len(guards) - 1:x}\0".translate(GUARD)
    text = re.sub(r"(?<=[\w)\]])\s*:\s*\d+(?=\s*[;,])", hide, text)  # bit-field widths
    if guard_shifts:
        text = re.sub(r"(<<|>>)=?\s*(0x[0-9a-fA-F]+|\d+)", hide, text)
    subs = {a: b for a, b in val.items() if abs(a) >= min_val}
    if scaled:
        for a, b in list(subs.items()):
            for d in (4, 2):
                if a % d == 0 and b % d == 0 and abs(a) >= 2 * d and a // d not in subs:
                    subs[a // d] = b // d

    def swap(m):
        sign, digits, suffix = m.group(1) or "", m.group(2), m.group(3)
        try:
            v = int(digits, 0)
        except ValueError:  # a leading zero
            return m.group(0)

        def fmt(x):
            return hex(x) if digits.lower().startswith("0x") else str(x)
        if sign and -v in subs:  # a negative template value, written `-N`
            b = subs[-v]
            return ("-" + fmt(-b) if b < 0 else fmt(b)) + suffix
        if v in subs:
            b = subs[v]
            return sign + (fmt(b) if b >= 0 else "-" + fmt(-b)) + suffix
        return m.group(0)
    text = re.sub(NUM, swap, text)
    for i, g in enumerate(guards):
        text = text.replace(f"\0{i:x}\0".translate(GUARD), g, 1)
    return text


STRATEGIES = [  # (name, scaled, guard shifts, min value)
    ("plain", False, True, 0),
    ("scaled", True, True, 0),
    ("scaled, skip small", True, True, 16),
    ("scaled, shifts unguarded", True, False, 0),
]

# ---------------------------------------------------------------- generation


class Template:
    def __init__(self, f: dict, path: Path, root: str | None = None):
        self.f = f
        self.path = path
        self.root = root or f["name"]
        self._parsed = None
        self._relocs = None

    def parsed(self):
        if self._parsed is None:
            self._parsed = chunks(strip_comments(self.path.read_text()))
        return self._parsed

    def relocs(self):
        """The template function's relocations in a grown compile of its file."""
        if self._relocs is None:
            obj, _ = fnindex.compile_object(self.path, SCRATCH)
            self._relocs = elf_relocs(obj, self.f["name"]) if obj else None
        return self._relocs


def extract(tpl: Template) -> tuple[list[Chunk], Chunk, list[Chunk]] | str:
    """(declarations used, the definition, other definitions it calls)."""
    cs = tpl.parsed()
    body = next((c for c in cs if c.kind == "func" and c.name == tpl.f["name"]), None)
    if body is None:
        return "definition not found in template file"
    used = words(body.text) | {w for c in cs if c.kind == "pp" and c.text.startswith("#define") for w in words(c.text)}
    keep, calls = [], []
    # Types first, then declarations whose names the body or a kept chunk uses.
    for c in cs:
        if c is body:
            continue
        if c.kind == "pp":
            keep.append(c)
        elif c.kind == "func":
            if c.name in used:
                if re.match(r"\s*static\b", c.text):
                    return f"uses static function {c.name}"
                calls.append(c)
        elif c.is_type:
            keep.append(c)
        else:
            names = c.declared()
            if names & used:
                if c.defines_data:
                    return f"uses data defined in the template file ({', '.join(sorted(names & used))})"
                keep.append(c)
    return keep, body, calls


def generate(tpl: Template, cand: dict, smap: dict, headers: Headers, prefer_include: bool) -> str | None:
    got = extract(tpl)
    if isinstance(got, str):
        return None
    keep, body, calls = got
    lines = []
    declared_here = set()
    for c in keep:
        declared_here |= c.declared()
    protos = [Chunk(c.text.split("{", 1)[0].rstrip() + ";", "decl") for c in calls]
    for c in protos:
        declared_here |= c.declared()
    includes = [c.text for c in keep if c.kind == "pp" and c.text.startswith("#include")]
    extra_decls = []
    for old, new in smap.items():
        if old == tpl.f["name"] or old in declared_here:
            continue  # renamed with the file's own declaration
        if old not in headers.decl:
            continue
        hdr_new = headers.decl.get(new)
        if hdr_new and prefer_include:
            inc = f'#include "{hdr_new[0]}"'
            if inc not in includes:
                includes.append(inc)
            continue
        extra_decls.append(rename(headers.decl[old][1], {old: new}))
    for c in keep:
        if c.kind == "pp" and c.text.startswith("#include"):
            continue
        lines.append(rename(c.text, smap))
    lines += [rename(c.text, smap) for c in protos]
    lines += extra_decls
    # A collapsed mapping declares one name twice, maybe with two types:
    # keep the first.
    seen: set[str] = set()
    unique = []
    for line in lines:
        names = Chunk(line, "decl").declared() if not line.startswith("#") else set()
        if names and names <= seen:
            continue
        seen |= names
        unique.append(line)
    lines = unique
    out = "\n".join(includes) + "\n\n"
    if lines:
        out += "\n".join(lines) + "\n\n"
    out += rename(body.text, smap) + "\n"
    return out


def variants(tpl: Template, cand: dict, headers: Headers, near: bool):
    """Yield (strategy, C text) to try, or a failure reason as a string."""
    rel = tpl.relocs()
    if rel is None:
        yield "template does not compile"
        return
    smap, why = symbol_map(rel, tpl.f, cand)
    if smap is None:
        yield why
        return
    smap[tpl.f["name"]] = cand["name"]
    vmap = {}
    if near:
        vmap, why = value_map(tpl.f, cand)
        if vmap is None:
            yield why
            return
    seen = set()
    for prefer_include in (True, False):
        text = generate(tpl, cand, smap, headers, prefer_include)
        if text is None:
            yield extract(tpl)
            return
        for name, scaled, guard, minv in (STRATEGIES if near and vmap else STRATEGIES[:1]):
            t = render_values(text, vmap, scaled, guard, minv) if vmap else text
            if t not in seen:
                seen.add(t)
                yield (name, t)


def src_path(f: dict) -> str:
    func = fn.Func(f["module"], f["name"], f["mode"], f["addr"], f["size"])
    if f["module"] == "arm7":
        return f"src/arm7/sdk/{f['name']}.c"
    if f["module"] == "main" and not fn.is_game(func):
        return f"src/sdk/{f['name']}.c"
    return f"src/game/{f['module']}/{f['name']}.c"


# ---------------------------------------------------------------- driver

def data_after(f: dict) -> bool:
    """True if a symbol lies between the function's end and the next
    function. `fn.py add` covers that gap, and our file defines nothing
    there (func_02000afc is followed by BuildInfo, which dsd needs)."""
    func = fn.Func(f["module"], f["name"], f["mode"], f["addr"], f["size"])
    stop = fn.next_start(func)
    return any(func.end <= a < stop for a in fn.all_modules()[f["module"]].syms)


def fn_cli(*args) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, str(ROOT / "tools/fn.py"), *args], cwd=ROOT,
                          capture_output=True, text=True)


def remove_entry(path: str) -> None:
    for d in [*fn.CONFIG.rglob("delinks.txt"), fn.CONFIG7 / "delinks.txt"]:
        if not d.exists():
            continue
        text = d.read_text()
        head, _, rest = text.partition("\n\n")
        blocks = rest.strip("\n").split("\n\n")
        keep = [b for b in blocks if not b.startswith(f"{path}:\n")]
        if len(keep) != len(blocks):
            d.write_text(head + "\n\n" + "\n\n".join(keep) + "\n")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--identical-only", action="store_true")
    ap.add_argument("--limit", type=int)
    ap.add_argument("--module")
    ap.add_argument("--only")
    ap.add_argument("--report", type=Path, default=REPORT)
    args = ap.parse_args()
    t0 = time.process_time()
    SCRATCH.mkdir(parents=True, exist_ok=True)

    only = None
    if args.only:
        src = Path(args.only[1:]).read_text().split() if args.only.startswith("@") else args.only.split(",")
        only = {s.strip() for s in src if s.strip()}

    fnindex.write()  # status must reflect delinks.txt
    idx = fnindex.load()
    fnindex_symbols.update(idx.symbols)
    funcs = [v[0] for v in idx.by_name.values()]
    headers = Headers()

    by_hash = collections.defaultdict(list)
    by_shape = collections.defaultdict(list)

    def shape(f):
        return (f["mode"], f["size"], f["mnemonics"], tuple((r[0], fnindex.reloc_class(r[1])) for r in f["relocs"]))

    def add_template(f, path, root=None):
        t = Template(f, path, root)
        by_hash[f["mhash"]].append(t)
        if f["mnemonics"]:
            by_shape[shape(f)].append(t)
    for f in funcs:
        if f["status"] == "complete":
            add_template(f, ROOT / f["file"])

    todo = [f for f in funcs if f["status"] == "none"
            and (f["module"] == args.module if args.module else f["module"] != "arm7")
            and (only is None or f["name"] in only)
            and not data_after(f)]
    todo.sort(key=lambda f: (f["module"], f["addr"]))

    failed: dict[str, dict] = {}  # candidate -> best failed attempt
    matched: list[dict] = []

    def attempt(cand, near: bool):
        pool = by_shape.get(shape(cand), []) if near else by_hash.get(cand["mhash"], [])
        pool = [t for t in pool if t.f["name"] != cand["name"]][:TEMPLATES_PER_CANDIDATE]
        if not pool:
            return None
        best = None
        for tpl in pool:
            for v in variants(tpl, cand, headers, near):
                if isinstance(v, str):
                    best = best or {"name": cand["name"], "template": tpl.f["name"], "status": "no-map", "why": v}
                    continue
                strategy, text = v
                tmp = SCRATCH / f"{cand['name']}.c"
                tmp.write_text(text)
                res = fnindex.check(idx, tmp, cand["name"], cand["module"], SCRATCH)
                if res.ok:
                    # `template` names the function in mainline's C that the
                    # text came from, also when copied through an earlier match.
                    return {"name": cand["name"], "template": tpl.root, "via": tpl.f["name"], "status": "match",
                            "kind": "near" if near else "identical", "strategy": strategy, "text": text,
                            "module": cand["module"]}
                if res.units == 0:
                    r = {"name": cand["name"], "template": tpl.f["name"], "status": "compile-error",
                         "why": res.compiler_output.strip().splitlines()[0] if res.compiler_output.strip() else ""}
                else:
                    pct = 100.0 * (res.units - res.differ) / res.units
                    r = {"name": cand["name"], "template": tpl.f["name"], "status": "differ", "percent": round(pct, 2),
                         "why": str(res)}
                if best is None or best["status"] != "differ" or (r["status"] == "differ" and r["percent"] > best["percent"]):
                    best = r
        return best

    phases = [False] if args.identical_only else [False, True]
    done: set[str] = set()
    for near in phases:
        # A match can be the template for another candidate. After the first
        # round, retry only candidates whose family gained a template.
        grown = None
        while grown is None or grown:
            new_keys = set()
            for cand in todo:
                if cand["name"] in done:
                    continue
                if args.limit and len(matched) >= args.limit:
                    break
                key = shape(cand) if near else cand["mhash"]
                if grown is not None and key not in grown:
                    continue
                r = attempt(cand, near)
                if r is None:
                    continue
                r["kind"] = "near" if near else "identical"
                if r["status"] == "match":
                    matched.append(r)
                    done.add(cand["name"])
                    failed.pop(cand["name"], None)
                    tmp = SCRATCH / "templates" / f"{cand['name']}.c"
                    tmp.parent.mkdir(exist_ok=True)
                    tmp.write_text(r["text"])
                    add_template(cand, tmp, r["template"])
                    new_keys.add(shape(cand))
                    new_keys.add(cand["mhash"])
                    print(f"match {r['kind']:<9} {cand['name']:<28} <- {r['template']} ({r['strategy']})", flush=True)
                else:
                    failed[cand["name"]] = r
            grown = new_keys
    results = list(failed.values())

    print(f"{len(matched)} candidates pass the fast check "
          f"({sum(m['kind'] == 'identical' for m in matched)} identical, {sum(m['kind'] == 'near' for m in matched)} near)",
          flush=True)

    # Write, add, diff, finish.
    if not args.dry_run and matched:
        for m in matched:
            f = idx.func(m["name"])
            m["path"] = src_path(f)
            p = ROOT / m["path"]
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text(m["text"])
            fn.modules.cache_clear()
            fn.all_modules.cache_clear()
            try:
                fn.cmd_add(argparse.Namespace(name=m["name"], path=m["path"]))
            except SystemExit as e:
                m["status"] = f"add failed: {e}"
                p.unlink()
        fn.modules.cache_clear()
        fn.all_modules.cache_clear()
        for m in matched:
            if m["status"] != "match":
                continue
            r = fn_cli("diff", m["name"])
            if r.returncode == 0:
                fn_cli("finish", m["name"])
            else:
                pct = re.search(r"([\d.]+)% match", r.stdout)
                m["status"] = f"diff {pct.group(1)}%" if pct else "diff build error"
                print(f"fn.py diff disagrees: {m['name']} {m['status']}", flush=True)
                (ROOT / m["path"]).unlink(missing_ok=True)
                remove_entry(m["path"])
        fnindex.write()

    for m in matched:
        m.pop("text", None)
    near_miss = [r for r in results if r["status"] == "differ" and r["percent"] >= 99]
    summary = {
        "matched": sum(m["status"] == "match" for m in matched),
        "identical": sum(m["status"] == "match" and m["kind"] == "identical" for m in matched),
        "near": sum(m["status"] == "match" and m["kind"] == "near" for m in matched),
        "candidates": len(matched) + len(results),
        "compile_errors": sum(r["status"] == "compile-error" for r in results),
        "differ": sum(r["status"] == "differ" for r in results),
        "near_misses_99": len(near_miss),
        "no_map": sum(r["status"] == "no-map" for r in results),
        "diff_disagreed": sum(m["status"] != "match" for m in matched),
        "cpu_seconds": round(time.process_time() + sum(resource.getrusage(resource.RUSAGE_CHILDREN)[:2]) - t0, 1),
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps({"summary": summary, "matched": matched, "failed": results}, indent=1))
    print(json.dumps(summary))
    return 0


if __name__ == "__main__":
    sys.exit(main())
