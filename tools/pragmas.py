#!/usr/bin/env python3
"""Compile one function under mwcc #pragmas that change code generation.

    python3 tools/pragmas.py FILE.c NAME [--jobs N] [--module M] [--json OUT] [--apply]
    python3 tools/pragmas.py FILE.c NAME --ref OBJ.o      compare with an object, no ROM
    python3 tools/pragmas.py --accepted                   check each pragma against mwccarm

Some original source files switched optimisations per file or per function:
the name entry screen only matches at `#pragma optimization_level 0`, and
NameEntry_MoveFromKey only with `#pragma opt_common_subs off`. permute.py
changes the C, never these switches. This tool keeps the C as it is and adds
pragma lines in two places:

  file    at the top of the file, before the #includes
  func    `#pragma push` and the lines just before the function, `#pragma pop`
          after it (other functions and header inlines keep the build's flags)

It tries every setting alone (optimization_level 0-3, optimize_for_size, and
each opt_* switch and peephole both on and off), then pairs of the settings
that changed the function, each level with each of those, and triples of the
best ones. A setting that leaves the function's bytes as they were is
dropped after the first pass.

The score is the number of instructions that differ after aligning both
sides (an inserted instruction costs one), as in permute.py. A match is
checked with fnindex.compare, the same test as `fn.py check`. The table
prints the best settings; --json writes every result; --apply writes the
best matching variant over FILE.c. Confirm with `fn.py diff` and
`make release`. An ARM7 function, or a file under src/arm7/, compiles with
the ARM7 flags (configure.CFLAGS7). Needs `build/fnindex.json` (ROM) unless --ref is given.
"""
import argparse
import difflib
import hashlib
import itertools
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import configure  # noqa: E402
import fnindex  # noqa: E402
import permute  # noqa: E402

ROOT = fnindex.ROOT
CC = ROOT / configure.CC

# Pragmas mwccarm 2.0-72 accepts that change code generation (checked with
# --accepted: an unknown pragma only gives an "illegal #pragma" warning).
# `scheduling`, `register_coloring` and `opt_cse` are not accepted.
LEVELS = [("optimization_level", str(n)) for n in range(4)] + [("optimize_for_size", "on")]
SWITCHES = ["opt_common_subs", "opt_loop_invariants", "opt_propagation", "opt_strength_reduction",
            "opt_strength_reduction_strict", "opt_lifetimes", "opt_dead_code", "opt_dead_assignments",
            "peephole", "opt_unroll_loops", "opt_vectorize_loops", "opt_pointer_analysis"]
SINGLES = [(p,) for p in LEVELS] + [((s, v),) for s in SWITCHES for v in ("off", "on")]
PLACES = ("file", "func")
TRIPLE_POOL = 8


def setting_text(setting) -> str:
    return "; ".join(f"{p} {v}" for p, v in setting) or "(build flags)"


def find_function(text: str, name: str) -> tuple[int, int]:
    """(start of the definition's first line, index just past its closing brace)."""
    for m in re.finditer(r"^[A-Za-z_][^;{}()#]*?\b" + re.escape(name) + r"\s*\(", text, re.M):
        j = permute.skip_trivia(text, permute.match_close(text, m.end() - 1))
        if j < len(text) and text[j] == "{":
            return m.start(), permute.match_close(text, j)
    raise SystemExit(f"no definition of {name} in the file")


def variant(text: str, name: str, setting, place: str) -> str:
    """The source with the pragma lines of `setting` added at `place`."""
    if not setting:
        return text
    lines = "".join(f"#pragma {p} {v}\n" for p, v in setting)
    if place == "file":
        return lines + text
    start, end = find_function(text, name)
    return text[:start] + "#pragma push\n" + lines + text[start:end] + "\n#pragma pop\n" + text[end:]


# ---------------------------------------------------------------- targets

def tokens(code: bytes, relocs: dict[int, str], unit: int) -> list:
    """One token per instruction; a relocated one becomes ('R', class)."""
    out = []
    for i in range(0, len(code), unit):
        hit = next((relocs[o] for o in range(i - 3, i + unit) if o in relocs), None)
        out.append(("R", hit) if hit else code[i:i + unit])
    return out


def our_function(obj: bytes, name: str):
    """(code, {offset: class}, [(offset, type, symbol, addend)]) or None."""
    secs, syms, relas = fnindex.read_elf(obj)
    sym = next((s for s in syms if s.name == name and s.shndx not in (0, 0xfff1) and s.kind == 2), None)
    if sym is None:
        return None
    code = secs[sym.shndx][1][sym.value:sym.value + sym.size]
    rel, full = {}, []
    for off, rtype, si, addend in relas.get(sym.shndx, []):
        if sym.value <= off < sym.value + sym.size:
            rel[off - sym.value] = fnindex.ELF_CLASS.get(rtype, str(rtype))
            full.append((off - sym.value, rtype, syms[si].name, addend))
    return code, rel, full


class IndexTarget:
    """The original from build/fnindex.json."""

    def __init__(self, name, module=None, index=None):
        self.index = index or fnindex.load()
        self.name, self.module = name, module
        want = self.index.func(name, module)
        self.func_module = want["module"]
        self.unit = 2 if want["mode"] == "thumb" else 4
        relocs = {off: fnindex.reloc_class(kind) for off, kind, *_ in want["relocs"]}
        self.tokens = tokens(bytes.fromhex(want["raw"]), relocs, self.unit)

    def exact(self, obj):
        return fnindex.compare(self.index, obj, self.name, self.module).ok


class ObjectTarget:
    """The original from a compiled object (for tests; no ROM needed)."""

    def __init__(self, path, name, thumb=False):
        self.name, self.unit = name, 2 if thumb else 4
        got = our_function(Path(path).read_bytes(), name)
        if got is None:
            sys.exit(f"{path} does not define {name}")
        self.code, rel, self.relocs = got
        self.tokens = tokens(self.code, rel, self.unit)

    def exact(self, obj):
        got = our_function(obj, self.name)
        return got is not None and got[0] == self.code and got[2] == self.relocs


# ---------------------------------------------------------------- sweep

class Sweep:
    def __init__(self, path: Path, name: str, target, jobs: int):
        self.path, self.name, self.target = path, name, target
        self.text = path.read_text()
        # ARM7 code builds with other flags (configure.CFLAGS7).
        cc, cflags = configure.compiler_for(path, getattr(target, "func_module", None))
        self.cc, self.cflags = ROOT / cc, cflags
        (ROOT / "build").mkdir(exist_ok=True)
        self.tmp = Path(tempfile.mkdtemp(prefix="pragmas-", dir=ROOT / "build"))
        self.pool = ThreadPoolExecutor(max_workers=jobs)
        self.serial = itertools.count()
        self.results = {}   # (setting, place) -> dict
        self.compiles = 0

    def close(self):
        self.pool.shutdown()
        shutil.rmtree(self.tmp, ignore_errors=True)

    def _one(self, setting, place):
        src = self.tmp / f"v{next(self.serial)}.c"
        obj = src.with_suffix(".o")
        src.write_text(variant(self.text, self.name, setting, place))
        cmd = [str(self.cc), "-c", *self.cflags.split(), "-i", str(self.path.parent), "-o", str(obj), str(src)]
        r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
        res = {"setting": [list(p) for p in setting], "place": place, "differ": None, "units": None,
               "ok": False, "hash": None}
        if r.returncode == 0 and obj.exists():
            data = obj.read_bytes()
            got = our_function(data, self.name)
            if got:
                ours = tokens(got[0], got[1], self.target.unit)
                sm = difflib.SequenceMatcher(None, self.target.tokens, ours, autojunk=False)
                total = max(len(self.target.tokens), len(ours))
                res["differ"] = total - sum(b.size for b in sm.get_matching_blocks())
                res["units"] = total
                res["ok"] = self.target.exact(data)
                res["hash"] = hashlib.sha1(got[0] + repr(got[2]).encode()).hexdigest()
        for p in (src, obj):
            p.unlink(missing_ok=True)
        self.compiles += 1
        return res

    def run(self, jobs):
        """jobs: [(setting, place)]; results for new ones are stored."""
        new = [j for j in dict.fromkeys(jobs) if j not in self.results]
        for j, res in zip(new, self.pool.map(lambda j: self._one(*j), new)):
            self.results[j] = res
        return [self.results[j] for j in jobs]

    def sweep(self):
        base = self.run([((), "file")])[0]
        self.run([(s, p) for s in SINGLES for p in PLACES])
        if self.matched():
            return base
        # Settings that changed the function, per place.
        live = {p: [s for s in SINGLES if self.results[(s, p)]["hash"] not in (None, base["hash"])]
                for p in PLACES}
        # `func` adds nothing when it gives the same bytes as `file` for every setting.
        places = [p for p in PLACES
                  if p == "file" or any(self.results[(s, p)]["hash"] != self.results[(s, "file")]["hash"]
                                        for s in SINGLES)]
        jobs = []
        for p in places:
            sw = [s for s in live[p] if s[0][0] in SWITCHES]
            for a, b in itertools.combinations(sw, 2):
                if a[0][0] != b[0][0]:
                    jobs.append((a + b, p))
            for lv in LEVELS:
                for s in sw:
                    jobs.append(((lv,) + s, p))
        self.run(jobs)
        if self.matched():
            return base
        # Triples from the switches in the best results so far.
        jobs = []
        for p in places:
            ranked = sorted((r for (s, pl), r in self.results.items() if pl == p and r["differ"] is not None and s),
                            key=lambda r: r["differ"])
            pool = []
            for r in ranked:
                for pv in r["setting"]:
                    pv = tuple(pv)
                    if pv[0] in SWITCHES and pv not in pool:
                        pool.append(pv)
            pool = pool[:TRIPLE_POOL]
            for combo in itertools.combinations(pool, 3):
                if len({pv[0] for pv in combo}) == 3:
                    jobs.append((combo, p))
                    for lv in LEVELS:
                        jobs.append(((lv,) + combo, p))
        self.run(jobs)
        return base

    def matched(self):
        return any(r["ok"] for r in self.results.values())

    def ranked(self):
        rows = [(k, r) for k, r in self.results.items() if r["differ"] is not None]
        return sorted(rows, key=lambda kr: (not kr[1]["ok"], kr[1]["differ"], len(kr[0][0]), kr[0][1] != "file"))


def fmt(r):
    if r["differ"] is None:
        return "does not compile / not found"
    pct = 100.0 * (r["units"] - r["differ"]) / r["units"]
    return f"{r['differ']} differ of {r['units']}  ({pct:.2f}%){'  MATCH' if r['ok'] else ''}"


def accepted() -> int:
    """Compile a probe with each pragma; report the ones mwccarm warns about."""
    bad = 0
    with tempfile.TemporaryDirectory() as d:
        for p, v in [lv for lv in LEVELS] + [(s, "on") for s in SWITCHES] + [("pragma_probe_unknown", "on")]:
            src = Path(d) / "probe.c"
            src.write_text(f"#pragma {p} {v}\nint f(int a);\nint f(int a) {{ return a * 3; }}\n")
            r = subprocess.run([str(CC), "-c", *configure.CFLAGS.split(), "-w", "all", "-o", str(src.with_suffix(".o")),
                                str(src)], cwd=ROOT, capture_output=True, text=True)
            ok = r.returncode == 0 and "illegal #pragma" not in r.stdout + r.stderr
            print(f"  {p} {v}: {'accepted' if ok else 'not accepted'}")
            bad += not ok and p != "pragma_probe_unknown"
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("file", nargs="?")
    ap.add_argument("name", nargs="?")
    ap.add_argument("--module")
    ap.add_argument("--ref", help="compare with this object instead of the original (no ROM)")
    ap.add_argument("--thumb", action="store_true", help="with --ref: the function is Thumb")
    ap.add_argument("--jobs", type=int, default=min(4, os.cpu_count() or 2))
    ap.add_argument("--top", type=int, default=8)
    ap.add_argument("--json", help="write every result to this file")
    ap.add_argument("--apply", action="store_true", help="write the best matching variant over FILE.c")
    ap.add_argument("--accepted", action="store_true", help="check that mwccarm accepts each pragma")
    a = ap.parse_args()
    if a.accepted:
        return accepted()
    if not a.file or not a.name:
        ap.error("FILE.c and NAME are needed")
    path = Path(a.file).resolve()
    target = ObjectTarget(a.ref, a.name, a.thumb) if a.ref else IndexTarget(a.name, a.module)
    S = Sweep(path, a.name, target, a.jobs)
    try:
        base = S.sweep()
        ranked = S.ranked()
        print(f"{a.name}: build flags {fmt(base)}")
        seen = set()
        for (setting, place), r in ranked:
            if r["hash"] in seen or len(seen) >= a.top:
                continue
            seen.add(r["hash"])
            print(f"  {fmt(r):40s} {place:4s}  {setting_text(setting)}")
        print(f"  {S.compiles} compiles")
        if a.json:
            best = ranked[0][1] if ranked else None
            Path(a.json).write_text(json.dumps({"name": a.name, "file": a.file, "base": base, "best": best,
                                                "results": [r for _, r in ranked]}, indent=1))
        if ranked and ranked[0][1]["ok"]:
            (setting, place), _ = ranked[0]
            print(f"  MATCH with {setting_text(setting)} ({place})")
            if a.apply:
                path.write_text(variant(S.text, a.name, setting, place))
                print(f"  {a.file} rewritten; tidy the pragma lines, then fn.py diff")
            return 0
        return 1
    finally:
        S.close()


if __name__ == "__main__":
    sys.exit(main())
