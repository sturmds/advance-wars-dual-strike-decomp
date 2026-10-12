#!/usr/bin/env python3
"""Check that `fn.py check` agrees with `fn.py diff`.

    python3 tools/tests/test_fncheck.py [-n 20]

Takes N functions from files marked complete (the issue 0017 same-address
case, Thumb, every module, then a spread of sizes) and every function in a
started file, runs both tools on each, and prints a table. Fails if check
says match where diff is below 100%, or if a complete function does not
match in check. check may be stricter than diff, never looser. Where check
ran objdiff-cli (a file with a delinked original from the last build), it
fails if check's count of differing rows is not the count diff prints
(issue 0070). Before that fix check counted by position: in the started
files, UnitInfoPanel_DrawPage showed 12 in check and 14 in diff, and
func_ov002_02357e5c 21 and 14.

Then, for the complete functions with relocations, it changes one byte,
one relocation target and one relocation kind in a copy of the index, and
fails if check still says match.

Runs fn.py diff, so it calls ninja; it does not write to config/.
"""

import argparse
import re
import subprocess
import sys
import time
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(TOOLS))
import fn  # noqa: E402
import fnindex  # noqa: E402

ROOT = fnindex.ROOT


def diff_score(name: str) -> tuple[float | None, int | None]:
    """objdiff's percentage and the rows marked |, from `fn.py diff`."""
    r = subprocess.run([sys.executable, str(TOOLS / "fn.py"), "diff", name, "--color", "never"], cwd=ROOT,
                       capture_output=True, text=True)
    m = re.search(r"([\d.]+)% match", r.stdout)
    s = fn.parse_diff(r.stdout)
    return (float(m.group(1)) if m else None), (s.differ if s else None)


def pick(index: fnindex.Index, n: int) -> list[tuple[dict, fnindex.Result]]:
    done = sorted((f for fs in index.by_name.values() for f in fs if f["status"] == "complete"),
                  key=lambda f: (f["size"], f["name"]))
    checked = {f["name"]: fnindex.check(index, ROOT / f["file"], f["name"], f["module"]) for f in done}
    out = []

    def take(f):
        if f not in out and len(out) < n:
            out.append(f)
    for f in [f for f in done if checked[f["name"]].renamed][:3]:
        take(f)
    take(next(f for f in done if f["mode"] == "thumb"))
    for module in sorted({f["module"] for f in done}):
        take(next(f for f in reversed(done) if f["module"] == module))
    rest = [f for f in done if f not in out]
    step = max(1, len(rest) // max(1, n - len(out)))
    for f in rest[::step]:
        take(f)
    return [(f, checked[f["name"]]) for f in out]


def mutations(index: fnindex.Index, f: dict) -> list[str]:
    """Wrong originals that check must reject. Returns what it accepted."""
    obj, _ = fnindex.compile_object(ROOT / f["file"])
    covered = {off + i for off, *_ in f["relocs"] for i in range(4)}
    raw = bytearray.fromhex(f["raw"])
    flip = next(i for i in range(len(raw)) if i not in covered)
    raw[flip] ^= 1
    off, kind, to, dest, label = f["relocs"][0]
    other = "load" if kind != "load" else "arm_call"
    wrong = {
        f"byte {flip:#x} changed": {"raw": raw.hex()},
        f"relocation at {off:#x} to {to + 4:#x}": {"relocs": [[off, kind, to + 4, dest, label]] + f["relocs"][1:]},
        f"relocation at {off:#x} kind {other}": {"relocs": [[off, other, to, dest, label]] + f["relocs"][1:]},
    }
    accepted = []
    for what, change in wrong.items():
        fake = fnindex.Index({f["name"]: [{**f, **change}]}, index.symbols)
        if fnindex.compare(fake, obj, f["name"]).ok:
            accepted.append(f"{f['name']}: check accepts a wrong original ({what})")
    return accepted


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-n", type=int, default=20, help="complete functions to test")
    args = ap.parse_args()
    index = fnindex.load()
    started = [f for fs in index.by_name.values() for f in fs if f["status"] == "started"]
    cases = pick(index, args.n) + [(f, None) for f in started]
    rows, failures = [], []
    for f, _ in cases:
        t = time.time()
        res = fnindex.check(index, ROOT / f["file"], f["name"], f["module"])
        ms = (time.time() - t) * 1000
        pct, differ = diff_score(f["name"])
        rows.append((f, res, ms, pct, differ))
        if res.ok and pct != 100.0:
            failures.append(f"{f['name']}: check says match, diff says {pct}%")
        if res.percent is not None and res.differ != differ:
            failures.append(f"{f['name']}: check counts {res.differ} differing rows, diff {differ}")
        if f["status"] == "complete" and not res.ok:
            failures.append(f"{f['name']}: complete, but check says {res}")
    print(f"{'function':<34} {'module':<6} {'size':>6} {'status':<8} {'check':>9} {'renamed':>7} {'ms':>4} {'diff %':>6} {'diff':>4}")
    for f, res, ms, pct, differ in rows:
        outcome = "match" if res.ok else f"{res.differ}/{res.units}"
        print(f"{f['name']:<34} {f['module']:<6} {f['size']:>#6x} {f['status']:<8} {outcome:>9} "
              f"{res.renamed:>7} {ms:>4.0f} {pct if pct is not None else '?':>6} "
              f"{differ if differ is not None else '?':>4}")
    mutated = [f for f, res, *_ in rows if f["status"] == "complete" and f["relocs"]]
    for f in mutated:
        failures += mutations(index, f)
    for line in failures:
        print("FAIL", line)
    print(f"\n{len(rows)} functions, {len(mutated) * 3} wrong originals, {len(failures)} failures")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
