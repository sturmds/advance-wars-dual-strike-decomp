#!/usr/bin/env python3
"""One-function helper for the decompile loop (docs/workflow.md).

    fn.py pick [-n N] [--min-size S] [--max-size S] [--all]
                                               ranked candidates not yet in a file
    fn.py pick --families [-n N] [--min-members M]
                                               unmatched functions grouped by masked hash, largest
                                               families first (heads for propagate.py)
    fn.py show NAME|ADDR                       location, callers, callees, data, other references
                                               (pointer tables, callbacks), disassembly
    fn.py add NAME src/PATH.c                  add a delinks.txt entry covering the function
    fn.py diff NAME                            compile its file and compare with the original
    fn.py check FILE.c NAME                    fast yes/no: compile FILE.c alone and compare NAME
                                               with build/fnindex.json (tools/fnindex.py)
    fn.py finish NAME                          mark its file `complete` (linked from our C)
    fn.py rename OLD NEW                       rename a symbol in symbols.txt, src/ and include/;
                                               moves src/.../OLD.c to NEW.c as well, and updates
                                               config/AWRE/names-provenance.csv

Everything reads config/AWRE/arm9/**/{symbols,relocs,delinks}.txt, and
config/AWRE/arm7/ for the `arm7` module (one module for ARM7's three parts;
names func_arm7_XXXXXXXX). `pick` leaves ARM7 out unless given --arm7. `diff`
runs ninja for the object and objdiff.json, then objdiff-cli, prints both
listings side by side (lines marked | differ; lines marked = name another
symbol at the same address, or an absolute symbol from configure.py's
ARM9_ABSOLUTE_SYMBOLS with the original word's value, and count as
matching), and exits 0 only at 100%.

`pick --families` is based on khdays-decomp tools/pick_family.py
(CC0 1.0, commit ba2de370; see docs/third-party/khdays-decomp.md).
"""

import argparse
import bisect
import json
import re
import subprocess
import sys
from dataclasses import dataclass, field
from functools import cache
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "config/AWRE/arm9"
CONFIG7 = ROOT / "config/AWRE/arm7"
INTERESTING = ROOT / "config/AWRE/interesting.txt"
PROVENANCE = ROOT / "config/AWRE/names-provenance.csv"
OBJDIFF = ROOT / ".cache/tools/objdiff-cli/objdiff-cli"
OBJDUMP = "arm-none-eabi-objdump"

# Rough split of main (docs/awre-facts.md): below this is game code, above it
# System library and C library code. Overlays and ITCM are handled per module.
MAIN_GAME_END = 0x02109000
MAIN_GAME_START = 0x02000b24  # below: secure area and BIOS stubs
# ITCM holds game code (OAM building, BG scroll, main loop, range maps, camera)
# below this address and SDK code above it.
ITCM_GAME_START = 0x01ff8000
ITCM_GAME_END = 0x01ffd878

FUNC = re.compile(r"^(\S+) kind:function\((arm|thumb),size=(0x[0-9a-f]+)\S*\) addr:(0x[0-9a-f]+)")
ANY_SYM = re.compile(r"^(\S+) kind:(\S+) addr:(0x[0-9a-f]+)")
RELOC = re.compile(r"^from:(0x[0-9a-f]+) kind:(\S+) to:(0x[0-9a-f]+)(?: add:(-?0x[0-9a-f]+))? module:(\S+)")
TEXT = re.compile(r"\s+\.text\s+start:(0x[0-9a-f]+) end:(0x[0-9a-f]+)")
CALL_KINDS = {"arm_call", "thumb_call", "arm_call_thumb", "thumb_call_arm", "arm_branch"}


def ninja(*targets: str) -> list[str]:
    """A ninja command line that waits for the checkout's build lock, as
    `make` does: two ninja runs in one build/ break each other's delink step."""
    (ROOT / "build").mkdir(exist_ok=True)
    return ["flock", "build/.ninja.lock", "ninja", *targets]


@dataclass
class Func:
    module: str
    name: str
    mode: str
    addr: int
    size: int

    @property
    def end(self) -> int:
        return self.addr + self.size


@dataclass
class Module:
    name: str
    dir: Path
    funcs: list[Func] = field(default_factory=list)
    syms: dict[int, str] = field(default_factory=dict)  # addr -> name, any kind
    relocs: list[tuple[int, str, int, str]] = field(default_factory=list)
    addends: dict[int, int] = field(default_factory=dict)  # from -> relocs.txt `add:`
    text: tuple[int, int] = (0, 0)
    files: list[tuple[str, int, int, bool]] = field(default_factory=list)  # path, start, end, complete
    texts: list[tuple[int, int]] = field(default_factory=list)  # every .text section (ARM7 has three)


def module_dirs():
    yield "main", CONFIG
    for name in ("itcm", "dtcm"):
        yield name, CONFIG / name
    for d in sorted((CONFIG / "overlays").iterdir()):
        yield d.name, d


def parse_delinks(path: Path, texts: list | None = None):
    text, files = (0, 0), []
    cur = None
    for line in path.read_text().splitlines():
        if line and not line[0].isspace() and line.endswith(":"):
            cur = [line[:-1], 0, 0, False]
            files.append(cur)
            continue
        m = TEXT.match(line)
        if m and cur is None:
            text = (int(m.group(1), 16), int(m.group(2), 16))
            if texts is not None:
                texts.append(text)
        elif m:
            cur[1], cur[2] = int(m.group(1), 16), int(m.group(2), 16)
        elif cur is not None and line.strip() == "complete":
            cur[3] = True
    return text, [tuple(f) for f in files]


@cache
def modules() -> dict[str, Module]:
    """The ARM9 modules. Other tools (loop_state.py, progress.py) count on
    this staying ARM9 only; all_modules() adds ARM7."""
    return {name: load_module(name, d) for name, d in module_dirs()}


@cache
def all_modules() -> dict[str, Module]:
    """ARM9 modules, then `arm7` once config/AWRE/arm7/ exists."""
    out = dict(modules())
    if (CONFIG7 / "symbols.txt").exists():
        out["arm7"] = load_module("arm7", CONFIG7)
    return out


def load_module(name: str, d: Path) -> Module:
    mod = Module(name, d)
    for line in (d / "symbols.txt").read_text().splitlines():
        m = FUNC.match(line)
        if m:
            mod.funcs.append(Func(name, m.group(1), m.group(2), int(m.group(4), 16), int(m.group(3), 16)))
        m = ANY_SYM.match(line)
        if m:
            mod.syms.setdefault(int(m.group(3), 16), m.group(1))
    for line in (d / "relocs.txt").read_text().splitlines():
        m = RELOC.match(line)
        if m:
            mod.relocs.append((int(m.group(1), 16), m.group(2), int(m.group(3), 16), m.group(5)))
            if m.group(4):
                mod.addends[int(m.group(1), 16)] = int(m.group(4), 16)
    mod.text, mod.files = parse_delinks(d / "delinks.txt", mod.texts)
    mod.funcs.sort(key=lambda f: f.addr)
    mod.relocs.sort()
    return mod


def target_module(dest: str) -> list[str]:
    """relocs.txt `module:` value -> module names it may refer to."""
    if dest in ("main", "itcm", "dtcm", "arm7"):
        return [dest]
    m = re.match(r"overlays?\(([\d,\s]+)\)", dest)
    return [f"ov{int(x):03d}" for x in m.group(1).split(",")] if m else []


def name_at(dest: str, addr: int) -> str:
    for mod in target_module(dest):
        n = all_modules()[mod].syms.get(addr)
        if n:
            return n
    return f"0x{addr:08x}"


def find(key: str) -> Func:
    want = int(key, 16) if re.fullmatch(r"(0x)?[0-9a-fA-F]{8}", key) else None
    hits = [f for mod in all_modules().values() for f in mod.funcs
            if f.name == key or (want is not None and f.addr == want)]
    if not hits:
        sys.exit(f"error: no function {key} in config/AWRE/arm9/**/symbols.txt or config/AWRE/arm7/")
    if len(hits) > 1:
        # Overlays share addresses; a bare address can match several.
        names = ", ".join(f"{f.name} ({f.module})" for f in hits)
        print(f"note: {key} matches {names}; using the first. Pass a name to pick one.", file=sys.stderr)
    return hits[0]


def file_of(f: Func):
    for path, start, end, complete in all_modules()[f.module].files:
        if start <= f.addr < end:
            return path, start, end, complete
    return None


def relocs_in(f: Func):
    rel = all_modules()[f.module].relocs
    i = bisect.bisect_left(rel, (f.addr,))
    j = bisect.bisect_left(rel, (f.end,))
    return rel[i:j]


def calls_from(f: Func):
    return [(k, to, dest) for frm, k, to, dest in relocs_in(f) if k in CALL_KINDS]


def loads_from(f: Func):
    return [(to, dest) for frm, k, to, dest in relocs_in(f) if k == "load"]


def callers_of(f: Func):
    out = []
    for mod in all_modules().values():
        for frm, k, to, dest in mod.relocs:
            if to == f.addr and k in CALL_KINDS and f.module in target_module(dest):
                caller = next((g for g in mod.funcs if g.addr <= frm < g.end), None)
                out.append(caller.name if caller else f"0x{frm:08x}")
    return sorted(set(out))


def describe(dest: str, addr: int) -> str:
    """Module and kind of the symbol at addr, e.g. `dtcm bss`."""
    for mod in target_module(dest):
        for line in (all_modules()[mod].dir / "symbols.txt").read_text().splitlines():
            m = ANY_SYM.match(line)
            if m and int(m.group(3), 16) == addr:
                return f"{mod} {m.group(2)}"
    return dest


def users_of(dest: str, addr: int, skip: Func, limit: int = 6) -> list[str]:
    """Other functions that load the address of the symbol at addr."""
    out = []
    for mod in all_modules().values():
        for frm, k, to, d in mod.relocs:
            if to == addr and k == "load" and set(target_module(d)) & set(target_module(dest)):
                g = next((g for g in mod.funcs if g.addr <= frm < g.end), None)
                if g and g.name != skip.name and g.name not in out:
                    out.append(g.name)
    return out[:limit] + (["..."] if len(out) > limit else [])


@cache
def sym_addrs(module: str) -> list[int]:
    return sorted(all_modules()[module].syms)


def refs_to(f: Func, limit: int = 8) -> list[str]:
    """Places that hold f's address other than calls: a function that loads
    it (a callback), or data such as a table of function pointers, named as
    the symbol at or below the place plus the offset."""
    out = set()
    for mod in all_modules().values():
        for frm, k, to, dest in mod.relocs:
            if to != f.addr or k in CALL_KINDS or f.module not in target_module(dest):
                continue
            g = next((g for g in mod.funcs if g.addr <= frm < g.end), None)
            if g:
                out.add(f"address taken in {g.name}")
                continue
            addrs = sym_addrs(mod.name)
            i = bisect.bisect_right(addrs, frm) - 1
            if i < 0:
                out.add(f"0x{frm:08x} ({mod.name})")
            else:
                out.add(f"{mod.syms[addrs[i]]}+{frm - addrs[i]:#x} ({mod.name})")
    refs = sorted(out)
    return refs[:limit] + (["..."] if len(refs) > limit else [])


def matched_names() -> set[str]:
    out = set()
    for mod in all_modules().values():
        for path, start, end, complete in mod.files:
            if complete:
                out.update(f.name for f in mod.funcs if start <= f.addr < end)
    return out


def is_game(f: Func) -> bool:
    return is_game_at(f.module, f.addr)


def is_game_at(module: str, addr: int) -> bool:
    if module == "main":
        return MAIN_GAME_START <= addr < MAIN_GAME_END
    if module == "itcm":
        return ITCM_GAME_START <= addr < ITCM_GAME_END
    return module.startswith("ov")


def interesting() -> list[str]:
    if not INTERESTING.exists():
        return []
    return [ln.split("#")[0].strip() for ln in INTERESTING.read_text().splitlines() if ln.split("#")[0].strip()]


def next_start(f: Func) -> int:
    mod = all_modules()[f.module]
    later = [g.addr for g in mod.funcs if g.addr > f.addr]
    if f.module == "arm7":
        # The end of the .text section that holds f: ARM7 has one per part.
        end = next(e for s, e in mod.texts if s <= f.addr < e)
        return min([a for a in later if a < end] + [end])
    return min(later + [mod.text[1]])


# ---------------------------------------------------------------- commands

def has_backward_branch(entry) -> bool:
    """True when the function branches to a lower address (a loop)."""
    if entry["mode"] != "arm":
        return False
    raw = bytes.fromhex(entry["raw"])
    end = entry["pool"] or len(raw)
    for i in range(0, end - 3, 4):
        word = int.from_bytes(raw[i:i + 4], "little")
        # B/Bcc (not BL): bits 27-24 = 1010; the offset is a signed 24 bits.
        if (word >> 24) & 0xf == 0xa and (word >> 28) != 0xf and word & 0x800000:
            return True
    return False


def cmd_pick_families(args) -> int:
    """Rank unmatched functions by how many share one masked hash."""
    import fnindex
    index = fnindex.load()
    entries = [e for es in index.by_name.values() for e in es]
    started = {e["mhash"] for e in entries if e["status"] == "started"}
    groups: dict[str, list] = {}
    for e in entries:
        game = is_game_at(e["module"], e["addr"])
        if (e["module"] == "arm7") != args.arm7:
            continue
        if e["status"] == "none" and (args.all or args.arm7 or game):
            groups.setdefault(e["mhash"], []).append(e)
    fams = []
    skipped = 0
    for h, members in groups.items():
        if len(members) < args.min_members:
            continue
        if h in started:
            skipped += 1
            continue
        members.sort(key=lambda e: e["addr"])
        fams.append((len(members), len(members) * members[0]["size"], members[0]))
    fams.sort(key=lambda f: (-f[0], -f[1], f[2]["addr"]))
    print(f"{'head':<24} {'module':<6} {'members':>7} {'size':>6} {'total':>7} loop")
    for n, total, head in fams[:args.n]:
        loop = "yes" if has_backward_branch(head) else "no"
        print(f"{head['name']:<24} {head['module']:<6} {n:>7} {head['size']:>#6x} {total:>#7x} {loop}")
    print(f"\n{len(fams)} families with {args.min_members}+ unmatched members "
          f"({sum(f[0] for f in fams)} functions); {skipped} skipped because a started "
          "function has the same shape. loop = backward branch in the head.")
    return 0


def cmd_pick(args) -> int:
    if args.families:
        return cmd_pick_families(args)
    done = matched_names()
    wanted = interesting()
    rows = []
    for mod in all_modules().values():
        if (mod.name == "arm7") != args.arm7:
            continue
        for f in mod.funcs:
            if f.mode != "arm" or not args.min_size <= f.size <= args.max_size or file_of(f):
                continue
            if not args.all and not args.arm7 and not is_game(f):
                continue
            calls = calls_from(f)
            callee_names = {name_at(d, t) for _, t, d in calls}
            unmatched = len(callee_names - done)
            star = f.name in wanted or f"0x{f.addr:08x}" in wanted
            # Wanted first, then fewest unmatched callees, then smallest.
            rows.append(((not star, unmatched, f.size), f, len(callee_names), len(loads_from(f)), star))
    rows.sort(key=lambda r: r[0])
    print(f"{'name':<22} {'module':<6} {'addr':<10} {'size':>6} calls data")
    for _, f, ncalls, nloads, star in rows[:args.n]:
        print(f"{f.name:<22} {f.module:<6} 0x{f.addr:08x} {f.size:>#6x} {ncalls:>5} {nloads:>4}{'  *' if star else ''}")
    what = "ARM7" if args.arm7 else "all ARM9 code" if args.all else "game code"
    print(f"\n{len(rows)} candidates ({what}, ARM, "
          f"size {args.min_size:#x}-{args.max_size:#x}). "
          "* = listed in config/AWRE/interesting.txt")
    return 0


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


def cmd_show(args) -> int:
    f = find(args.key)
    fi = file_of(f)
    print(f"{f.name}  {f.module} 0x{f.addr:08x}  {f.mode}  size {f.size:#x}  ({'game' if is_game(f) else 'SDK/library'})")
    print(f"file:    {fi[0] + (' (complete)' if fi[3] else '') if fi else '(none yet: fn.py add)'}")
    done = matched_names()
    callees = sorted({name_at(d, t) for _, t, d in calls_from(f)})
    print("calls:   " + (", ".join(c + (" (matched)" if c in done else "") for c in callees) or "nothing (leaf)"))
    loads = sorted({(name_at(d, t), t, d) for t, d in loads_from(f)})
    print("data:    " + ("" if loads else "none"))
    for name, addr, dest in loads:
        print(f"  {name:<24} {describe(dest, addr)}  used by: {', '.join(users_of(dest, addr, f)) or 'only this'}")
    print("callers: " + (", ".join(callers_of(f)) or "none found"))
    print("refs:    " + ("; ".join(refs_to(f)) or "none"))
    delinks = ROOT / ("build/arm7/delinks" if f.module == "arm7" else "build/delinks")
    if fi:
        obj = delinks / Path(fi[0]).with_suffix(".o")
    else:
        # dsd names ARM7's gap objects after the stand-in ROM's autoloads.
        prefix = f"autoload_{arm7_index(f.addr)}" if f.module == "arm7" else f.module
        gaps = sorted(delinks.glob(f"_dsd_gap@{prefix}_*.o"))
        obj = next((g for g in gaps if re.search(rf" [Tt] {re.escape(f.name)}$", subprocess.run(
            ["arm-none-eabi-nm", "--defined-only", str(g)], capture_output=True, text=True).stdout, re.M)), None)
    if obj is None or not obj.exists():
        sys.exit("error: no delinked object yet. Run `make` first.")
    print("\n".join(objdump(obj, f.name, f.mode)))
    return 0


def arm7_index(addr: int) -> int:
    """dsd's autoload index for the ARM7 part that holds addr (tools/arm7.py)."""
    import arm7
    regs = arm7.regions(arm7.ARM7_BIN.read_bytes())
    return arm7.region_of(regs, addr).index


def cmd_add(args) -> int:
    f = find(args.name)
    path = args.path
    if not re.fullmatch(r"src/[\w/]+\.c", path):
        sys.exit("error: path must look like src/dir/name.c")
    if file_of(f):
        sys.exit(f"error: {f.name} is already in {file_of(f)[0]}")
    mod = all_modules()[f.module]
    if any(p == path for m in all_modules().values() for p, *_ in m.files):
        sys.exit(f"error: {path} already exists in a delinks.txt; pick another name")
    start, end = f.addr, next_start(f)
    for p, s, e, _ in mod.files:
        if s < end and start < e:
            sys.exit(f"error: 0x{start:08x}-0x{end:08x} overlaps {p}")

    delinks = mod.dir / "delinks.txt"
    head, _, rest = delinks.read_text().partition("\n\n")
    blocks = [b for b in rest.strip("\n").split("\n\n") if b.strip()]
    blocks.append(f"{path}:\n    .text       start:0x{start:08x} end:0x{end:08x}")

    def block_start(b):
        m = TEXT.search(b)
        return int(m.group(1), 16) if m else 0
    blocks.sort(key=block_start)
    delinks.write_text(head + "\n\n" + "\n\n".join(blocks) + "\n")
    print(f"added {path}: .text 0x{start:08x}-0x{end:08x} to {delinks.relative_to(ROOT)}")
    return 0


def cmd_finish(args) -> int:
    f = find(args.name)
    fi = file_of(f)
    if not fi:
        sys.exit(f"error: {f.name} is not in any file; fn.py add first")
    if fi[3]:
        print(f"{fi[0]} is already marked")
        return 0
    delinks = all_modules()[f.module].dir / "delinks.txt"
    text = delinks.read_text().replace(f"\n{fi[0]}:\n", f"\n{fi[0]}:\n    complete\n", 1)
    delinks.write_text(text)
    print(f"marked {fi[0]} complete")
    return 0


def snake_case(name: str) -> str:
    """PlayerAddFunds -> player_add_funds, the file naming used in src/."""
    name = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name)
    return re.sub(r"([A-Z]+)([A-Z][a-z])", r"\1_\2", name).lower()


def cmd_rename(args) -> int:
    old, new = args.old, args.new
    if not re.fullmatch(r"[A-Za-z_]\w*", new):
        sys.exit(f"error: {new} is not a C identifier")
    word = re.compile(rf"(?<!\w){re.escape(old)}(?!\w)")
    for mod in all_modules().values():
        if new in mod.syms.values():
            sys.exit(f"error: {new} already exists in {mod.name}")
    hits = 0
    for mod in all_modules().values():
        p = mod.dir / "symbols.txt"
        text = p.read_text()
        new_text = re.sub(rf"^{re.escape(old)} ", f"{new} ", text, flags=re.M)
        if new_text != text:
            p.write_text(new_text)
            hits += 1
    if not hits:
        sys.exit(f"error: no symbol {old}")
    if hits > 1:
        print(f"warning: {old} was defined in {hits} modules; renamed in all", file=sys.stderr)
    for mod in all_modules().values():
        for path, *_ in mod.files:
            if Path(path).stem == old and (ROOT / path).exists():
                new_path = str(Path(path).with_name(snake_case(new) + ".c"))
                (ROOT / path).rename(ROOT / new_path)
                d = mod.dir / "delinks.txt"
                d.write_text(d.read_text().replace(f"\n{path}:\n", f"\n{new_path}:\n"))
                print(f"moved {path} -> {new_path}")
    for p in list((ROOT / "src").rglob("*.[ch]")) + list((ROOT / "include").rglob("*.h")):
        text = p.read_text()
        if word.search(text):
            p.write_text(word.sub(new, text))
            print(f"updated {p.relative_to(ROOT)}")
    # A renamed import keeps its source line, under the new name.
    text = PROVENANCE.read_text()
    new_text = re.sub(rf"^{re.escape(old)},", f"{new},", text, flags=re.M)
    if new_text != text:
        PROVENANCE.write_text(new_text)
        print(f"updated {PROVENANCE.relative_to(ROOT)}")
    print(f"renamed {old} -> {new}")
    return 0


def cmd_diff(args) -> int:
    f = find(args.name)
    fi = file_of(f)
    if not fi:
        sys.exit(f"error: {f.name} is not inside any delinks.txt file. Run fn.py add first.")
    path = fi[0]
    if not (ROOT / path).exists():
        sys.exit(f"error: {path} does not exist yet.")
    subprocess.run([sys.executable, ROOT / "tools/configure.py"], check=True)
    unit = str(Path(path).with_suffix(""))
    import configure
    ours = ROOT / configure.obj_of(path)
    project = "build/arm7" if f.module == "arm7" else "."
    stamp = "build/arm7/delink.stamp" if f.module == "arm7" else "build/delink.stamp"
    r = subprocess.run(ninja(str(ours.relative_to(ROOT)), f"{project}/objdiff.json".removeprefix("./"), stamp),
                       cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        sys.stdout.write(r.stdout + r.stderr)
        return 1

    r = subprocess.run([OBJDIFF, "diff", "-p", project, "-u", unit, "-o", "-", "--format", "json", f.name],
                       cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        sys.stdout.write(r.stdout + r.stderr)
        return 1
    data = json.loads(r.stdout)

    def side(key):
        for s in (data.get(key) or {}).get("symbols", []):
            if s.get("name") == f.name:
                return s
        return {}

    orig, ours_sym = side("left"), side("right")
    if not ours_sym:
        print(f"error: {path} does not define {f.name}. Check the function name in the C file.")
        return 1
    percent = orig.get("match_percent", ours_sym.get("match_percent"))

    def rows(s):
        out = []
        for ins in s.get("instructions", []):
            i = ins.get("instruction")
            text = f"{int(i.get('address', 0)):4x}: {i['formatted']}" if i else ""
            out.append((text, ins.get("diff_kind")))
        return out

    left, right = rows(orig), rows(ours_sym)
    # objdiff compares relocations by symbol name. Rows where both sides
    # point at the same address under different names (issue 0017) match,
    # and so do words relocated to an absolute symbol of the original value.
    same = {i for i, (a, b) in enumerate(zip(orig.get("instructions", []), ours_sym.get("instructions", [])))
            if a.get("diff_kind") and (same_target(data, a, b) or absolute_word(data, a, b))}
    width = max((len(t) for t, _ in left), default=10) + 2
    print(f"{'original':<{width}}   ours")
    differ = 0
    for i in range(max(len(left), len(right))):
        a, ka = left[i] if i < len(left) else ("", "missing")
        b, kb = right[i] if i < len(right) else ("", "missing")
        mark = "=" if i in same else "|" if (ka or kb) else " "
        differ += mark == "|"
        print(f"{a:<{width}} {mark} {b}")
    if same and not differ:
        percent = 100.0
    if same:
        print(f"\n= : {len(same)} relocation(s) name another symbol at the same address,"
              " or an absolute symbol of the same value; counted as matching.")
    print(f"\n{f.name}: {percent if percent is not None else '?'}% match")
    return 0 if percent == 100.0 else 1


def cmd_check(args) -> int:
    import fnindex
    index = fnindex.load()
    try:
        res = fnindex.check(index, Path(args.file), args.name, args.module)
    except KeyError as e:
        sys.exit(f"error: {e.args[0]}")
    if res.compiler_output:
        sys.stdout.write(res.compiler_output)
    print(res)
    return 0 if res.ok else 1


@cache
def symbol_addresses() -> dict[str, int | None]:
    """Name -> address over all modules; None when a name has two addresses."""
    out = {}
    for mod in all_modules().values():
        for line in (mod.dir / "symbols.txt").read_text().splitlines():
            m = ANY_SYM.match(line)
            if m:
                addr = int(m.group(3), 16)
                out[m.group(1)] = addr if out.get(m.group(1), addr) == addr else None
    return out


def same_target(data, left, right) -> bool:
    """True when two objdiff rows differ only in a relocation's symbol name,
    and both relocations resolve to the same address plus addend."""
    if left.get("diff_kind") != "DIFF_ARG_MISMATCH" or right.get("diff_kind") != "DIFF_ARG_MISMATCH":
        return False
    targets = []
    for key, row in (("left", left), ("right", right)):
        ins = row["instruction"]
        args = [p["arg"] for p in ins.get("parts", []) if "arg" in p]
        # Every argument that differs must be the relocation.
        for arg, d in zip(args, row.get("arg_diff", [])):
            if "diff_index" in d and not arg.get("reloc"):
                return False
        rel = ins.get("relocation")
        if not rel:
            return False
        name = data[key]["symbols"][rel["target_symbol"]]["name"]
        addr = symbol_addresses().get(name)
        if addr is None:
            return False
        targets.append((rel["type"], addr + int(rel.get("addend", 0))))
    left_ins, right_ins = left["instruction"], right["instruction"]
    if [p for p in left_ins["parts"] if "arg" not in p] != [p for p in right_ins["parts"] if "arg" not in p]:
        return False
    return targets[0] == targets[1]


def absolute_word(data, left, right) -> bool:
    """True when the original row is a plain `.word N` and ours is a word
    relocated to a symbol from ARM9_ABSOLUTE_SYMBOLS (tools/configure.py)
    whose value plus addend is N. dsd leaves no relocation there, because
    the original object has none to give."""
    from configure import ARM9_ABSOLUTE_SYMBOLS
    if left.get("diff_kind") != "DIFF_ARG_MISMATCH" or right.get("diff_kind") != "DIFF_ARG_MISMATCH":
        return False
    lins, rins = left["instruction"], right["instruction"]
    largs = [p["arg"] for p in lins.get("parts", []) if "arg" in p]
    rel = rins.get("relocation")
    if lins.get("relocation") or not rel or rel.get("type_name") != "R_ARM_ABS32":
        return False
    if len(largs) != 1 or "unsigned" not in largs[0]:
        return False
    if [p for p in lins["parts"] if "arg" not in p] != [p for p in rins["parts"] if "arg" not in p]:
        return False
    name = data["right"]["symbols"][rel["target_symbol"]]["name"]
    if name not in ARM9_ABSOLUTE_SYMBOLS:
        return False
    value = (ARM9_ABSOLUTE_SYMBOLS[name] + int(rel.get("addend", 0))) & 0xFFFFFFFF
    return int(largs[0]["unsigned"]) == value


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("pick")
    p.add_argument("-n", type=int, default=20)
    p.add_argument("--max-size", type=lambda s: int(s, 0), default=0x100)
    p.add_argument("--min-size", type=lambda s: int(s, 0), default=0x10)
    p.add_argument("--all", action="store_true", help="include SDK and library code")
    p.add_argument("--families", action="store_true", help="rank families of same-shape functions")
    p.add_argument("--arm7", action="store_true", help="ARM7 functions only (all of it is SDK/library code)")
    p.add_argument("--min-members", type=int, default=2, help="with --families")
    p.set_defaults(fn=cmd_pick)
    p = sub.add_parser("show")
    p.add_argument("key")
    p.set_defaults(fn=cmd_show)
    p = sub.add_parser("add")
    p.add_argument("name")
    p.add_argument("path")
    p.set_defaults(fn=cmd_add)
    p = sub.add_parser("diff")
    p.add_argument("name")
    p.set_defaults(fn=cmd_diff)
    p = sub.add_parser("check")
    p.add_argument("file")
    p.add_argument("name")
    p.add_argument("--module", help="for a name defined in several modules")
    p.set_defaults(fn=cmd_check)
    # Not "complete": the worktree sandbox refuses commands containing that word.
    p = sub.add_parser("finish")
    p.add_argument("name")
    p.set_defaults(fn=cmd_finish)
    p = sub.add_parser("rename")
    p.add_argument("old")
    p.add_argument("new")
    p.set_defaults(fn=cmd_rename)
    args = ap.parse_args()
    return args.fn(args)


if __name__ == "__main__":
    sys.exit(main())
