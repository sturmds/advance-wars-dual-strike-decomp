#!/usr/bin/env python3
"""Sort the started, unlinked functions by the kind of difference left.

    python3 tools/triage_partials.py --out FILE.md [--jobs 3] [--no-sweep] [NAME ...]

For each function in a source file that is not marked `complete`
(PROGRESS.md, "Started, not linked", ARM7 included), or only the NAMEs given:

  class      from `fn.py diff`: register-only (same instructions, other
             register numbers), order (the same instructions in another
             order, literal pool included, registers may differ too),
             missing/extra (a few instructions more or fewer), structural
             (anything bigger)
  sweep      the best score of `tools/pragmas.py`, and its setting
  neighbour  the nearest file in the same module that uses a #pragma, with
             its distance in bytes; `adjacent` when it is the next file
  proto      what tools/protocheck.py reports for the file, by kind (count,
             call, width, ret); fix a count or call row first

Writes a Markdown table to FILE.md (and FILE.json next to it). The sweep
costs CPU only: --jobs functions run at once, one compile each.
"""
import argparse
import json
import re
import subprocess
import sys
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn  # noqa: E402
import fnindex  # noqa: E402
import pragmas  # noqa: E402
import progress  # noqa: E402
import protocheck  # noqa: E402

ROOT = fn.ROOT
PRAGMA = re.compile(r"^\s*#pragma\s+(?!once\b|thumb\b|push\b|pop\b)(.+?)\s*$", re.M)
REG = re.compile(r"\b(r\d+|ip|lr|sb|sl|fp)\b")


# ---------------------------------------------------------------- diff classes

def diff_rows(name: str):
    """[(original, ours, mark)] from `fn.py diff`, and the percentage."""
    out = subprocess.run([sys.executable, str(ROOT / "tools/fn.py"), "diff", name],
                         cwd=ROOT, capture_output=True, text=True).stdout
    lines = out.splitlines()
    head = next((i for i, ln in enumerate(lines) if ln.startswith("original")), None)
    m = re.search(r"([\d.]+)% match", out)
    if head is None or not m:
        return None, None
    width = lines[head].index("ours") - 3
    rows = []
    for ln in lines[head + 1:]:
        if not ln.strip():
            break
        rows.append((ln[:width].strip(), ln[width + 3:].strip(), ln[width + 1:width + 2]))
    return rows, float(m.group(1))


def norm(ins: str, regs: bool) -> str:
    """An instruction without its address, pc offsets and branch targets;
    registers become R when regs is False."""
    s = re.sub(r"^\s*[0-9a-f]+:\s*", "", ins)
    s = re.sub(r"\[pc, #-?0x[0-9a-f]+\] \(->0x[0-9a-f]+\)", "[pc]", s)
    s = re.sub(r"^(b\w*|bl\w*)\s+0x[0-9a-f]+$", r"\1 T", s)
    if not regs:
        s = REG.sub("R", s)
        s = re.sub(r"\{[^}]*\}", "{L}", s)
    return s


def classify(rows) -> dict:
    left = [r[0] for r in rows if r[0]]
    right = [r[1] for r in rows if r[1]]
    differ = sum(r[2] == "|" for r in rows)
    out = {"orig": sum(not x.split(": ", 1)[-1].startswith(".word") for x in left),
           "ours": sum(not x.split(": ", 1)[-1].startswith(".word") for x in right), "differ": differ}
    a = [norm(x, False) for x in left]
    b = [norm(x, False) for x in right]
    if differ == 0:
        out["class"] = "match"
    elif a == b:
        out["class"] = "register-only"
    elif Counter(a) == Counter(b):
        out["class"] = "order"
    else:
        extra = sum((Counter(a) - Counter(b)).values()) + sum((Counter(b) - Counter(a)).values())
        out["extra"] = extra
        small = max(4, len(a) // 25)
        out["class"] = "missing/extra" if extra <= small else "structural"
    return out


# ---------------------------------------------------------------- neighbours

def pragma_files():
    """{module: [(start, end, path, [pragma lines])]} for files with a pragma."""
    out = {}
    for mod in fn.all_modules().values():
        for path, start, end, complete in mod.files:
            p = ROOT / path
            if not path.startswith("src/") or not p.exists() or not start:
                continue
            found = PRAGMA.findall(p.read_text())
            if found:
                out.setdefault(mod.name, []).append((start, end, path, found))
    return out


def nearest(module: str, addr: int, end: int, path: str, table):
    best = None
    for s, e, p, found in table.get(module, []):
        if p == path:
            continue
        dist = s - end if s >= end else addr - e if e <= addr else 0
        if best is None or dist < best[0]:
            best = (dist, p, found)
    return best


# ---------------------------------------------------------------- main

def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("names", nargs="*")
    ap.add_argument("--out", required=True, type=Path)
    ap.add_argument("--jobs", type=int, default=3)
    ap.add_argument("--no-sweep", action="store_true")
    a = ap.parse_args()

    funcs = []
    for path, names, _ in progress.unlinked():
        for n in names:
            if not a.names or n in a.names:
                funcs.append((path, n))
    by_name = {f.name: f for mod in fn.all_modules().values() for f in mod.funcs}
    table = pragma_files()
    protos = protocheck.load()
    rows = {}
    for path, n in funcs:
        f = by_name[n]
        d, pct = diff_rows(n)
        row = {"file": path, "name": n, "module": f.module, "size": f.size, "pct": pct}
        row.update(classify(d) if d else {"class": "no diff"})
        kinds = Counter(kind for _, kind, _, _ in protocheck.report(protos, [path], everything=False))
        row["proto"] = dict(kinds)
        nb = nearest(f.module, f.addr, f.end, path, table)
        if nb:
            row["neighbour"] = {"dist": nb[0], "file": nb[1], "pragmas": nb[2]}
        rows[n] = row
        print(f"{n}: {row['class']} {pct}%", file=sys.stderr)

    if not a.no_sweep:
        index = fnindex.load()

        def sweep(item):
            path, n = item
            S = pragmas.Sweep(ROOT / path, n, pragmas.IndexTarget(n, by_name[n].module, index), jobs=1)
            try:
                base = S.sweep()
                ranked = S.ranked()
                best = ranked[0] if ranked else None
                res = {"base": base["differ"], "units": base["units"], "compiles": S.compiles}
                if best:
                    res.update({"best": best[1]["differ"], "ok": best[1]["ok"],
                                "setting": pragmas.setting_text(best[0][0]), "place": best[0][1]})
                return n, res
            finally:
                S.close()

        with ThreadPoolExecutor(max_workers=a.jobs) as pool:
            for n, res in pool.map(sweep, funcs):
                rows[n]["sweep"] = res
                print(f"{n}: sweep {res.get('base')} -> {res.get('best')} {res.get('setting', '')}", file=sys.stderr)

    a.out.with_suffix(".json").write_text(json.dumps(list(rows.values()), indent=1))
    a.out.write_text(markdown(list(rows.values())))
    return 0


def markdown(rows) -> str:
    order = ["register-only", "order", "missing/extra", "structural", "match", "no diff"]
    counts = Counter(r["class"] for r in rows)
    out = ["# Partial functions: triage", "",
           "Written by `tools/triage_partials.py`. Classes from `fn.py diff`; `sweep` is the best",
           "`tools/pragmas.py` score (instructions that differ after alignment) against the build flags.",
           "", "| Class | Functions |", "|---|---:|"]
    out += [f"| {c} | {counts[c]} |" for c in order if counts[c]]
    out += ["", "| Function | Module | Bytes | Match | Class | Differ rows | Instr. orig/ours | Sweep: flags -> best | Best setting | Nearest pragma file | Proto |",
            "|---|---|---:|---:|---|---:|---|---|---|---|---|"]
    for r in sorted(rows, key=lambda r: (order.index(r["class"]) if r["class"] in order else 9, -(r["pct"] or 0))):
        sw = r.get("sweep", {})
        if sw.get("best") is not None:
            gain = f"{sw['base']} -> {sw['best']}{' MATCH' if sw.get('ok') else ''}"
            setting = "" if sw["best"] >= (sw["base"] or 0) else f"{sw['setting']} ({sw['place']})"
        else:
            gain, setting = "", ""
        nb = r.get("neighbour")
        nbs = ""
        if nb:
            where = "adjacent" if nb["dist"] == 0 else f"{nb['dist']:#x} away"
            nbs = f"{Path(nb['file']).name}: {'; '.join(nb['pragmas'])} ({where})"
        out.append(f"| {r['name']} | {r['module']} | {r['size']} | {r['pct'] if r['pct'] is not None else '?'}% | "
                   f"{r['class']} | {r.get('differ', '')} | {r.get('orig', '')}/{r.get('ours', '')} | {gain} | "
                   f"{setting} | {nbs} | {proto_text(r.get('proto', {}))} |")
    return "\n".join(out) + "\n"


def proto_text(kinds: dict) -> str:
    return " ".join(f"{k} {kinds[k]}" for k in ("count", "call", "width", "ret") if kinds.get(k))


if __name__ == "__main__":
    sys.exit(main())
