#!/usr/bin/env python3
"""Import names from a reviewed rename plan (docs/third-party/romhacking-890.md).

    import_names.py PLAN.csv            dry run: check every row, print the renames
    import_names.py PLAN.csv --apply    run `fn.py rename` per row, record each name
                                        in config/AWRE/names-provenance.csv

PLAN.csv has the columns of rename-plan.csv (module, address, old, new, source)
plus two optional review columns: `accept` (rows with `n` are skipped) and
`final_name` (overrides `new`). `source` is a file:line inside the document's
archive; it is stored as `rh890:<source>`.

Checks before anything changes:
  - `old` is still the symbol at `address` in `module` (rows already renamed
    are skipped with a note);
  - the new name is a C identifier, not used by any module, and appears once
    in the plan.
"""

import argparse
import csv
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn  # noqa: E402

PREFIX = "rh890:"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("plan", type=Path)
    ap.add_argument("--apply", action="store_true")
    args = ap.parse_args()

    mods = fn.modules()
    taken = {n for m in mods.values() for n in m.syms.values()}
    rows, errors, seen = [], [], set()
    for r in csv.DictReader(open(args.plan)):
        if r.get("accept", "").strip().lower() == "n":
            continue
        new = (r.get("final_name") or "").strip() or r["new"]
        addr = int(r["address"], 16)
        mod = mods.get(r["module"])
        if mod is None:
            errors.append(f"{r['old']}: unknown module {r['module']}")
            continue
        current = mod.syms.get(addr)
        if current == new:
            print(f"skip {r['old']}: already {new}")
            continue
        if current != r["old"]:
            errors.append(f"{r['old']}: {r['module']} 0x{addr:08x} holds {current}")
        if not re.fullmatch(r"[A-Za-z_]\w*", new):
            errors.append(f"{r['old']}: {new} is not a C identifier")
        elif new in taken or new in seen:
            errors.append(f"{r['old']}: {new} is already used")
        seen.add(new)
        rows.append((r["old"], new, r["module"], addr, PREFIX + r["source"]))

    for e in errors:
        print(f"error: {e}", file=sys.stderr)
    if errors:
        return 1
    for old, new, module, addr, source in rows:
        print(f"{module} 0x{addr:08x} {old} -> {new}  ({source})")
    print(f"{len(rows)} renames")
    if not args.apply:
        print("dry run; pass --apply to rename")
        return 0

    for old, new, module, addr, source in rows:
        subprocess.run([sys.executable, str(fn.ROOT / "tools/fn.py"), "rename", old, new], check=True,
                       stdout=subprocess.DEVNULL)
        with open(fn.PROVENANCE, "a", newline="") as f:
            csv.writer(f, lineterminator="\n").writerow([new, module, f"0x{addr:08x}", source])
    print(f"renamed {len(rows)}; recorded in {fn.PROVENANCE.relative_to(fn.ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
