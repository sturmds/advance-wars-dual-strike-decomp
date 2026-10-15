#!/usr/bin/env python3
"""Check that a source change left every function's code alone.

    python3 tools/objcmp.py snap [FILE]      record the code of every function
    python3 tools/objcmp.py compare [FILE]   compare the current build with it

Run `snap` on a build of the tree before a change that should not change
code (merging files, moving declarations into headers, renaming macros),
then `make` and `compare`. FILE defaults to build/objcmp.json.

The record holds each function's disassembly, with relocations, from the
objects of every C file in a delinks.txt, keyed by name. Offsets inside the
object and the labels of branch targets are left out, so a function that
moved to another file or another place in its file compares equal.
`compare` lists the functions whose code changed and those that are gone,
and exits 1 when there are any. `make check` still decides whether the ROM
matches: this finds which caller a changed declaration moved, and it catches
a change that the hashes would only show as a failure somewhere in a module.
"""
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from configure import obj_of  # noqa: E402

OBJDUMP = "arm-none-eabi-objdump"
DEFAULT = ROOT / "build/objcmp.json"


def objects() -> list:
    srcs = set()
    for d in (ROOT / "config/AWRE").rglob("delinks.txt"):
        srcs |= set(re.findall(r"^(src/\S+\.c):$", d.read_text(), re.M))
    return [o for o in (obj_of(s) for s in sorted(srcs)) if (ROOT / o).exists()]


def clean(line: str) -> str:
    line = re.sub(r"^\s*[0-9a-f]+:\s*", "", line).replace("\t", " ")
    line = re.sub(r"\b(b[a-z]*) [0-9a-f]+ <[^>]*>", r"\1", line)
    return re.sub(r"\b[0-9a-f]+ <", "<", line)


def snapshot(owners: dict = None) -> dict:
    """Each function's code by name. owners, when given, gets each name's
    objects."""
    funcs = {}
    objs = objects()
    for i in range(0, len(objs), 400):
        out = subprocess.run([OBJDUMP, "-dr", "--no-show-raw-insn", *objs[i:i + 400]],
                             cwd=ROOT, capture_output=True, text=True, check=True).stdout
        cur, obj = None, None
        for line in out.splitlines():
            f = re.match(r"^(\S+\.o):\s+file format", line)
            m = re.match(r"^[0-9a-f]+ <(.+)>:$", line)
            if f or line.startswith("Disassembly"):
                obj = f.group(1) if f else obj
                cur = None
            elif m:
                cur = []
                funcs.setdefault(m.group(1), []).append(cur)
                if owners is not None:
                    owners.setdefault(m.group(1), []).append(obj)
            elif cur is not None and line.strip():
                cur.append(clean(line))
    # A name can be defined in two modules (overlays); compare them as a set.
    return {k: sorted("\n".join(c) for c in v) for k, v in funcs.items()}


def changed(before: dict, owners: dict = None) -> list:
    """Names whose code differs from before, or that are gone."""
    now = snapshot(owners)
    return sorted(k for k in before if before[k] != now.get(k))


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("cmd", choices=["snap", "compare"])
    ap.add_argument("file", nargs="?", type=Path, default=DEFAULT)
    args = ap.parse_args()
    now = snapshot()
    if args.cmd == "snap":
        args.file.parent.mkdir(parents=True, exist_ok=True)
        args.file.write_text(json.dumps(now))
        print(f"objcmp: {len(now)} functions recorded in {args.file}")
        return 0
    before = json.loads(args.file.read_text())
    changed = sorted(k for k in before if k in now and before[k] != now[k])
    gone = sorted(k for k in before if k not in now)
    for k in changed:
        print(f"changed: {k}")
    for k in gone:
        print(f"gone: {k}")
    print(f"objcmp: {len(before)} functions, {len(changed)} changed, {len(gone)} gone, "
          f"{len(set(now) - set(before))} new")
    return 1 if changed or gone else 0


if __name__ == "__main__":
    sys.exit(main())
