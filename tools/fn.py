#!/usr/bin/env python3
"""One-function helper for the decompile loop (docs/workflow.md).

    fn.py show NAME|ADDR   where the function lives, and its original disassembly
    fn.py diff NAME        compile its file, then compare with the original

`show` reads config/AWRE/arm9/**/symbols.txt and delinks.txt. `diff` runs
ninja for the object and objdiff.json, then objdiff-cli in one-shot mode,
and prints the match percentage and both disassemblies side by side.
"""

import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "config/AWRE/arm9"
OBJDIFF = ROOT / ".cache/tools/objdiff-cli/objdiff-cli"
OBJDUMP = "arm-none-eabi-objdump"

SYMBOL = re.compile(r"^(\S+) kind:function\((arm|thumb),size=(0x[0-9a-f]+)\S*\) addr:(0x[0-9a-f]+)")


def module_dirs():
    yield "main", CONFIG
    for name in ("itcm", "dtcm"):
        yield name, CONFIG / name
    for d in sorted((CONFIG / "overlays").iterdir()):
        yield d.name, d


def find_symbol(key: str):
    want_addr = int(key, 16) if re.fullmatch(r"(0x)?[0-9a-fA-F]{8}", key) else None
    hits = []
    for module, d in module_dirs():
        for line in (d / "symbols.txt").read_text().splitlines():
            m = SYMBOL.match(line)
            if not m:
                continue
            name, mode, size, addr = m.group(1), m.group(2), int(m.group(3), 16), int(m.group(4), 16)
            if name == key or (want_addr is not None and addr == want_addr):
                hits.append(dict(module=module, dir=d, name=name, mode=mode, size=size, addr=addr))
    if not hits:
        sys.exit(f"error: no function {key} in config/AWRE/arm9/**/symbols.txt")
    if len(hits) > 1:
        # Overlays share addresses; a bare address can match several.
        print(f"note: {key} matches {len(hits)} functions; using the first. "
              "Pass a name to pick one.", file=sys.stderr)
    return hits[0]


def file_of(sym) -> str | None:
    """The delinks.txt file whose .text range holds the function, if any."""
    path = None
    for line in (sym["dir"] / "delinks.txt").read_text().splitlines():
        if line and not line[0].isspace() and line.endswith(":"):
            path = line[:-1]
        m = re.match(r"\s+\.text\s+start:(0x[0-9a-f]+) end:(0x[0-9a-f]+)", line)
        if path and m and int(m.group(1), 16) <= sym["addr"] < int(m.group(2), 16):
            return path
    return None


def objdump(obj: Path, name: str, mode: str) -> list[str]:
    args = [OBJDUMP, "-dr", "-marmv5te", "--no-show-raw-insn", f"--disassemble={name}", str(obj)]
    if mode == "thumb":
        args.insert(1, "-Mforce-thumb")
    out = subprocess.run(args, capture_output=True, text=True, check=True).stdout
    lines = []
    for line in out.splitlines():
        m = re.match(r"\s+([0-9a-f]+):\s+(.*)", line)
        if m:
            lines.append(f"{int(m.group(1), 16):4x}: {m.group(2).strip()}")
    return lines


def cmd_show(key: str) -> int:
    sym = find_symbol(key)
    path = file_of(sym)
    print(f"{sym['name']}  {sym['module']} 0x{sym['addr']:08x}  {sym['mode']}  size 0x{sym['size']:x}")
    print(f"file: {path or '(none: still in a gap object; add a delinks.txt entry)'}")
    obj = ROOT / "build/delinks" / (Path(path).with_suffix(".o") if path else "")
    if not path:
        gaps = sorted((ROOT / "build/delinks").glob(f"_dsd_gap@{sym['module']}_*.o"))
        obj = gaps[0] if gaps else None
    if obj is None or not obj.exists():
        sys.exit("error: no delinked object yet. Run `make` first.")
    print("\n".join(objdump(obj, sym["name"], sym["mode"])))
    return 0


def cmd_diff(key: str) -> int:
    sym = find_symbol(key)
    path = file_of(sym)
    if not path:
        sys.exit(f"error: {sym['name']} is not inside any delinks.txt file. Add one first.")
    if not (ROOT / path).exists():
        sys.exit(f"error: {path} does not exist yet.")
    subprocess.run([sys.executable, ROOT / "tools/configure.py"], check=True)
    unit = str(Path(path).with_suffix(""))
    ours = ROOT / "build" / Path(path).with_suffix(".o")
    orig = ROOT / "build/delinks" / Path(path).with_suffix(".o")
    r = subprocess.run(["ninja", str(ours.relative_to(ROOT)), "objdiff.json", "build/delink.stamp"],
                       cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        sys.stdout.write(r.stdout + r.stderr)
        return 1

    r = subprocess.run([OBJDIFF, "diff", "-p", ".", "-u", unit, "-o", "-", "--format", "json", sym["name"]],
                       cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        sys.stdout.write(r.stdout + r.stderr)
        return 1
    data = json.loads(r.stdout)

    def side(key):
        for s in (data.get(key) or {}).get("symbols", []):
            if s.get("name") == sym["name"]:
                return s
        return {}

    orig, ours = side("left"), side("right")
    percent = orig.get("match_percent", ours.get("match_percent"))

    def rows(s):
        out = []
        for ins in s.get("instructions", []):
            i = ins.get("instruction")
            text = f"{int(i.get('address', 0)):4x}: {i['formatted']}" if i else ""
            out.append((text, ins.get("diff_kind")))
        return out

    left, right = rows(orig), rows(ours)
    width = max((len(t) for t, _ in left), default=10) + 2
    print(f"{'original':<{width}}   ours")
    for i in range(max(len(left), len(right))):
        a, ka = left[i] if i < len(left) else ("", None)
        b, kb = right[i] if i < len(right) else ("", None)
        mark = "|" if (ka or kb) else " "
        print(f"{a:<{width}} {mark} {b}")
    print(f"\n{sym['name']}: {percent if percent is not None else '?'}% match")
    return 0 if percent == 100.0 else 1


def main() -> int:
    if len(sys.argv) != 3 or sys.argv[1] not in ("show", "diff"):
        sys.exit(__doc__)
    return {"show": cmd_show, "diff": cmd_diff}[sys.argv[1]](sys.argv[2])


if __name__ == "__main__":
    sys.exit(main())
