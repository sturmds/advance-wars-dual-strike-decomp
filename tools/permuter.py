#!/usr/bin/env python3
"""Random source mutation for near-miss functions, with decomp-permuter.

    nice -n 19 python3 tools/permuter.py NAME [NAME ...] [--seconds 600] [--jobs 4]
    nice -n 19 python3 tools/permuter.py --parked FILE.md --out REPORT.md [--seconds 1800]

For each function, it sets up build/permuter/NAME/ and runs decomp-permuter
(tools.lock; docs/third-party/decomp-permuter.md) on it for --seconds:

  pre.c        the function's file after `mwccarm -E` (#line lines removed)
  base.c       pre.c with each line decomp-permuter's C parser rejects (an
               `asm` declaration) hidden in a pragma, see parser_safe
  target.o     the original, from build/delinks/ (build/arm7/delinks/ for ARM7)
  compile.sh   the build's compiler and flags (tools/configure.py)
  settings.toml  the function name, compiler_type mwcc, and an objdump
               command that disassembles only that function

decomp-permuter applies random rewrites to the function (temporaries, statement
order, casts, loop forms, if/else, commutative operands, and more) and writes
each variant that beats its best score to output-SCORE-N/. Its score comes
from arm-none-eabi-objdump; this tool rescores each of the best five with
`fn.py check`'s relocation-aware compare, which is what counts. The best goes
to build/permuter/NAME/best.c with a diff of the function against base.c.
Bring the change back to the real source by hand and confirm with `fn.py diff`.

--parked reads the function names from the first column of a Markdown table
(partials-parked.md) and runs them one after another; --out writes a summary
table. Run under `nice -n 19`, with --jobs low enough to leave room for the
loop's agents.

decomp-permuter needs the `toml` module; a shim over the standard library's
tomllib in build/permuter/_shim/ stands in for it.
"""
import argparse
import base64
import difflib
import inspect
import json
import os
import re
import shutil
import signal
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import configure  # noqa: E402
import fn  # noqa: E402
import fnindex  # noqa: E402

ROOT = fn.ROOT
PERMUTER = ROOT / ".cache/tools/decomp-permuter/permuter.py"
WORK = ROOT / "build/permuter"
SHIM = '''"""Stand-in for the toml package: decomp-permuter only reads TOML."""
import tomllib


def load(f):
    if hasattr(f, "read"):
        data = f.read()
        return tomllib.loads(data.decode() if isinstance(data, bytes) else data)
    with open(f, "rb") as fh:
        return tomllib.load(fh)


def loads(s):
    return tomllib.loads(s)
'''


# objdump for decomp-permuter's scorer: relocations to a known symbol become
# its address plus the addend, so `data_ov000_022dbd7c` in the original and
# `Ov000_gMapHeaders+0x54` in ours compare equal, as in `fn.py check`. The
# script is written next to the inputs; canon_line is copied into it.
def canon_line(line: str, addrs: dict) -> str:
    import re
    m = re.search(r"(R_ARM_\w+\s+)([A-Za-z_.$][\w.$]*)(?:([+-])0x([0-9a-f]+))?$", line)
    if not m or addrs.get(m.group(2)) is None:
        return line
    add = int(m.group(4) or "0", 16)
    return line[:m.start()] + m.group(1) + f"0x{addrs[m.group(2)] + (-add if m.group(3) == '-' else add):08x}"


OBJDUMP = """import json, subprocess, sys
from pathlib import Path

{canon}

addrs = json.loads((Path(__file__).parent / "symbols.json").read_text())
out = subprocess.run(["arm-none-eabi-objdump", "-drz", "--disassemble=" + sys.argv[1], sys.argv[2]],
                     capture_output=True, text=True, check=True).stdout
sys.stdout.write("\\n".join(canon_line(line, addrs) for line in out.splitlines()) + "\\n")
"""


# mwcc syntax that decomp-permuter's C parser (pycparser) rejects: `asm void
# _ll_udiv_common(void);` in include/sdk/runtime.h (issue 0068).
MWCC_ONLY = re.compile(r"^[ \t]*asm\b[^;{}]*;[ \t]*$", re.M)


def parser_safe(text: str) -> str:
    """text with each MWCC_ONLY line replaced by `#pragma _permuter b64literal`.
    The parser keeps the pragma, and decomp-permuter writes the original line
    back when it prints the base or a candidate, so mwcc still compiles the
    real text. One line stays one line, so line numbers in errors still hold."""
    def hide(m):
        return "#pragma _permuter b64literal " + base64.b64encode(m.group(0).strip().encode()).decode()
    return MWCC_ONLY.sub(hide, text)


def permuter_modules() -> None:
    """Make decomp-permuter's src package importable."""
    for p in (str(WORK / "_shim"), str(PERMUTER.parent)):
        if p not in sys.path:
            sys.path.insert(0, p)


def parse_error(text: str):
    """decomp-permuter's parse error for text as base.c, or None if it parses."""
    permuter_modules()
    from src import ast_util  # noqa: E402
    from src.error import CandidateConstructionFailure  # noqa: E402
    try:
        ast_util.parse_c(text)
    except CandidateConstructionFailure as e:
        return e.message
    return None


def symbol_map(d: Path) -> dict:
    """Addresses of the symbols that base.c or target.o can name."""
    known = fn.symbol_addresses()
    words = set(re.findall(r"[A-Za-z_]\w*", (d / "base.c").read_text()))
    dump = subprocess.run(["arm-none-eabi-objdump", "-r", str(d / "target.o")],
                          capture_output=True, text=True).stdout
    words |= set(re.findall(r"R_ARM_\w+\s+([A-Za-z_.$][\w.$]*)", dump))
    return {w: known[w] for w in words if known.get(w) is not None}


def setup(name: str) -> tuple[Path, str, str]:
    """Write build/permuter/NAME/; returns (dir, source path, module)."""
    f = fn.find(name)
    where = fn.file_of(f)
    if where is None:
        sys.exit(f"error: {name} has no file in delinks.txt")
    path = where[0]
    delinks = "build/arm7/delinks" if f.module == "arm7" else "build/delinks"
    target = ROOT / delinks / Path(path).with_suffix(".o")
    if not target.exists():
        sys.exit(f"error: {target.relative_to(ROOT)} is missing; run make first")
    cc, flags = configure.compiler_for(path, f.module)

    d = WORK / name
    if d.exists():
        shutil.rmtree(d)
    d.mkdir(parents=True)
    pre = subprocess.run([str(ROOT / cc), *flags.split(), "-E", str(ROOT / path)],
                         cwd=ROOT, capture_output=True, text=True)
    if pre.returncode:
        sys.exit(f"error: mwccarm -E failed on {path}:\n{pre.stdout}{pre.stderr}")
    text = "\n".join(line for line in pre.stdout.splitlines() if not line.startswith("#line")) + "\n"
    (d / "pre.c").write_text(text)
    (d / "base.c").write_text(parser_safe(text))
    shutil.copyfile(target, d / "target.o")
    (d / "compile.sh").write_text(
        "#!/bin/bash\n"
        "# Called as: compile.sh INPUT.c -o OUTPUT.o\n"
        f'cd "{ROOT}" || exit 1\n'
        f'exec "{ROOT / cc}" {flags} -c "$1" -o "$3"\n')
    (d / "compile.sh").chmod(0o755)
    (d / "settings.toml").write_text(
        f'func_name = "{name}"\n'
        'compiler_type = "mwcc"\n'
        f'objdump_command = "{sys.executable} {d / "objdump.py"} {name}"\n')
    (d / "objdump.py").write_text(OBJDUMP.format(canon=inspect.getsource(canon_line)))
    (d / "symbols.json").write_text(json.dumps(symbol_map(d)))
    shim = WORK / "_shim"
    shim.mkdir(exist_ok=True)
    (shim / "toml.py").write_text(SHIM)
    err = parse_error((d / "base.c").read_text())
    if err:
        sys.exit(f"error: decomp-permuter cannot parse {(d / 'base.c').relative_to(ROOT)} "
                 f"(add the construct to MWCC_ONLY in tools/permuter.py):\n{err}")
    return d, path, f.module


def run(d: Path, seconds: int, jobs: int, log, verbose: bool = False) -> int:
    """Run decomp-permuter on d for at most `seconds`; returns its exit code
    (None when stopped by the time limit)."""
    env = dict(os.environ, PYTHONPATH=str(WORK / "_shim"))
    cmd = [sys.executable, str(PERMUTER), str(d), "-j", str(jobs), "--stack-diffs", "--best-only",
           "--stop-on-zero"] + (["--show-errors"] if verbose else [])
    p = subprocess.Popen(cmd, cwd=PERMUTER.parent, env=env, stdout=log, stderr=subprocess.STDOUT,
                         start_new_session=True)
    try:
        return p.wait(timeout=seconds)
    except subprocess.TimeoutExpired:
        os.killpg(p.pid, signal.SIGINT)
        try:
            p.wait(timeout=30)
        except subprocess.TimeoutExpired:
            os.killpg(p.pid, signal.SIGKILL)
            p.wait()
        return None


def function_text(source: str, name: str) -> str:
    """The definition of `name` in source, up to its closing brace."""
    m = re.search(r"^[^\n;{}]*\b" + re.escape(name) + r"\s*\([^;{]*\)\s*\{", source, re.M)
    if not m:
        return ""
    depth, i = 0, m.end() - 1
    while i < len(source):
        if source[i] == "{":
            depth += 1
        elif source[i] == "}":
            depth -= 1
            if depth == 0:
                return source[m.start():i + 1]
        i += 1
    return source[m.start():]


TOKEN = re.compile(r"0[xX][0-9a-fA-F]+[uUlL]*|\d+[uUlL]*|[A-Za-z_]\w*|->|\+\+|--|<<=?|>>=?|[<>!=]=|&&|\|\||\S")


def statements(code: str) -> list:
    """The code as one statement per line, spacing made uniform, so a diff
    shows what decomp-permuter changed and not how it reformatted."""
    out, cur = [], []
    for t in TOKEN.findall(code):
        cur.append(t)
        if t in (";", "{", "}"):
            out.append(" ".join(cur))
            cur = []
    if cur:
        out.append(" ".join(cur))
    return out


def printed_base(d: Path, name: str) -> str:
    """base.c as decomp-permuter prints an unchanged candidate, so it is
    formatted like the outputs (it adds parentheses and its own spacing)."""
    permuter_modules()
    from src import ast_util  # noqa: E402
    ast = ast_util.parse_c((d / "base.c").read_text())
    fn_def, _ = ast_util.extract_fn(ast, name)
    ast_util.normalize_ast(fn_def, ast)
    return ast_util.to_c(ast)


def write_diff(d: Path, name: str) -> None:
    base = statements(function_text(printed_base(d, name), name))
    best = statements(function_text((d / "best.c").read_text(), name))
    diff = difflib.unified_diff(base, best, "base.c", "best.c", lineterm="", n=2)
    (d / "best.diff").write_text("\n".join(diff) + "\n")


def rescore(d: Path, name: str, module: str, index) -> list:
    """[(differ, permuter score, output dir, Result)] for the best five outputs and base.c."""
    outs = []
    for o in d.glob("output-*"):
        m = re.fullmatch(r"output-(\d+)-\d+", o.name)
        if m and (o / "source.c").exists():
            outs.append((int(m.group(1)), o))
    outs.sort()
    rows = []
    for score, o in [(None, d)] + outs[:5]:
        src = o / ("pre.c" if o == d else "source.c")
        res = fnindex.check(index, src, name, module, tmpdir=d)
        rows.append((res.differ if res.message != "compile error" else 10 ** 6, score, o, res))
    return rows


def base_score(d: Path):
    """decomp-permuter's score of the unchanged function, from its log."""
    m = re.search(r"base score = (\d+)", (d / "permuter.log").read_text(errors="replace"))
    return int(m.group(1)) if m else None


def one(name: str, seconds: int, jobs: int, index, verbose: bool = False) -> dict:
    d, path, module = setup(name)
    start = time.time()
    with open(d / "permuter.log", "w") as log:
        code = run(d, seconds, jobs, log, verbose)
    if base_score(d) is None:
        tail = "\n".join((d / "permuter.log").read_text(errors="replace").splitlines()[-20:])
        sys.exit(f"error: decomp-permuter stopped before scoring {name} (exit {code}):\n{tail}")
    rows = rescore(d, name, module, index)
    base = rows[0]
    # A match wins; otherwise decomp-permuter's score, then fn.py check's
    # count of differing rows.
    best = min(rows, key=lambda r: (not r[3].ok, r[1] if r[1] is not None else base_score(d), r[0]))
    out = {"name": name, "file": path, "seconds": round(time.time() - start), "exit": code,
           "base": base[0], "best": best[0], "units": best[3].units, "ok": best[3].ok,
           "score_base": base_score(d), "score_best": best[1] if best[1] is not None else base_score(d),
           "outputs": len(list(d.glob("output-*"))), "best_dir": best[2]}
    if best[2] != d:
        shutil.copyfile(best[2] / "source.c", d / "best.c")
        write_diff(d, name)
    return out


def explain(name: str) -> None:
    """decomp-permuter's view of base.c against target.o: its score and the
    normalised lines that differ."""
    d, _, _ = setup(name)
    permuter_modules()
    from src.objdump import objdump, get_arch  # noqa: E402
    from src.scorer import Scorer  # noqa: E402
    cmd = f"{sys.executable} {d / 'objdump.py'} {name}"
    obj = d / "base.o"
    subprocess.run(["bash", str(d / "compile.sh"), str(d / "pre.c"), "-o", str(obj)], check=True)
    score, _ = Scorer(str(d / "target.o"), stack_differences=True, algorithm="difflib", debug_mode=False,
                      ign_branch_targets=True, objdump_command=cmd).score(str(obj))
    arch = get_arch(str(d / "target.o"))
    lines = [[ln.row for ln in objdump(str(o), arch, objdump_command=cmd.split(), stack_differences=True,
                                       ign_branch_targets=True)] for o in (d / "target.o", obj)]
    print(f"{name}: decomp-permuter score {score}")
    for row in difflib.unified_diff(*lines, "target.o", "base.o", lineterm="", n=0):
        print(row)


def summary_line(r: dict) -> str:
    gain = f"{r['base']} -> {r['best']}" + (" MATCH" if r["ok"] else "")
    where = r["best_dir"].relative_to(ROOT) if r["best_dir"] != WORK / r["name"] else "-"
    return (f"{r['name']}: {gain} of {r['units']} differ, permuter score {r['score_base']} -> {r['score_best']}, "
            f"{r['outputs']} outputs, {r['seconds']} s; best {where}")


def parked_names(path: Path) -> list:
    names = []
    for line in path.read_text().splitlines():
        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        if line.startswith("|") and cells and re.fullmatch(r"[A-Za-z_]\w*", cells[0]) and cells[0] != "function":
            names.append(cells[0])
    return names


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("names", nargs="*")
    ap.add_argument("--parked", type=Path, help="take the names from this Markdown table")
    ap.add_argument("--out", type=Path, help="write a Markdown summary here")
    ap.add_argument("--seconds", type=int, default=600, help="time per function")
    ap.add_argument("--jobs", type=int, default=4)
    ap.add_argument("--show-errors", action="store_true", help="log the compiler errors of variants")
    ap.add_argument("--diff", action="store_true",
                    help="only rewrite build/permuter/NAME/best.diff from best.c")
    ap.add_argument("--explain", action="store_true",
                    help="set up only, and print decomp-permuter's score and line diff of base.c")
    a = ap.parse_args()
    names = list(a.names) + (parked_names(a.parked) if a.parked else [])
    if not names:
        ap.error("give function names or --parked FILE")
    if not PERMUTER.exists():
        sys.exit("error: decomp-permuter is missing; run make setup")
    if a.diff:
        for n in names:
            write_diff(WORK / n, n)
            print((WORK / n / "best.diff").read_text())
        return 0
    if a.explain:
        for n in names:
            explain(n)
        return 0
    index = fnindex.load()
    results = []
    for n in names:
        r = one(n, a.seconds, a.jobs, index, a.show_errors)
        results.append(r)
        print(summary_line(r), flush=True)
        if a.out:
            a.out.write_text(markdown(results, a.seconds, a.jobs))
    return 0 if all(r["ok"] for r in results) else 1


def markdown(results: list, seconds: int, jobs: int) -> str:
    out = ["# decomp-permuter run", "",
           f"Written by `tools/permuter.py`: {seconds} s per function, {jobs} jobs. Before and best:",
           "rows that differ by `fn.py check` (counted as `fn.py diff` counts them), and decomp-permuter's",
           "score (aligned; 0 is a match). Check each best variant by hand: some change behaviour.", "",
           "| Function | Before | Best | Instructions | Score before | Score best | Outputs | Best variant |",
           "|---|---:|---:|---:|---:|---:|---:|---|"]
    for r in results:
        where = r["best_dir"].relative_to(ROOT) if r["best_dir"] != WORK / r["name"] else ""
        best = f"{r['best']}{' MATCH' if r['ok'] else ''}"
        out.append(f"| {r['name']} | {r['base']} | {best} | {r['units']} | {r['score_base']} | {r['score_best']} | "
                   f"{r['outputs']} | {where} |")
    return "\n".join(out) + "\n"


if __name__ == "__main__":
    sys.exit(main())
