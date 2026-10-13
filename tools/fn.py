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
    fn.py diff NAME [--color WHEN]             compile its file and compare with the original
    fn.py check FILE.c NAME                    fast yes/no: compile FILE.c alone and compare NAME
                                               with build/fnindex.json (tools/fnindex.py); counts
                                               differing rows as diff does (issue 0070)
    fn.py lines NAME [FILE.c]                  our compiled code for NAME, each run of instructions
                                               under the C line it comes from (mwcc -g line table)
    fn.py finish NAME                          mark its file `complete` (linked from our C)
    fn.py rename OLD NEW                       rename a symbol in symbols.txt, src/ and include/;
                                               moves src/.../OLD.c to NEW.c as well, and updates
                                               config/AWRE/names-provenance.csv, asm_functions.txt
                                               and the moved file's compilers.txt line

Everything reads config/AWRE/arm9/**/{symbols,relocs,delinks}.txt, and
config/AWRE/arm7/ for the `arm7` module (one module for ARM7's three parts;
names func_arm7_XXXXXXXX). `pick` leaves ARM7 out unless given --arm7. `diff`
runs ninja for the object and objdiff.json, then objdiff-cli, prints both
listings side by side (lines marked | differ; lines marked = name another
symbol at the same address, or an absolute symbol from the module's list in
configure.py (ARM9_ABSOLUTE_SYMBOLS, ARM7_ABSOLUTE_SYMBOLS) with the original
word's value, or branch to the same address, or hold the same bytes decoded
differently (issue 0077), and count as matching), and exits 0 only at 100%. Its last line,
`NAME: N differ, P% match`, gives the rows marked | and objdiff's percentage.
Rank attempts by N, then P (issue 0067): objdiff's score charges one wrong
opcode as much as 12 wrong registers, and charges for = rows too.
`fn.py check` prints the same N and P when the last build left a delinked
original for the function's file (it runs objdiff-cli on that object and
its own; issue 0070); otherwise it counts by position, so one missing or
extra instruction makes it count every one after it.

`pick --families` is based on khdays-decomp tools/pick_family.py
(CC0 1.0, commit ba2de370; see docs/third-party/khdays-decomp.md).
"""

import argparse
import bisect
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass, field
from functools import cache
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "config/AWRE/arm9"
CONFIG7 = ROOT / "config/AWRE/arm7"
INTERESTING = ROOT / "config/AWRE/interesting.txt"
PROVENANCE = ROOT / "config/AWRE/names-provenance.csv"
COMPILERS = ROOT / "config/AWRE/compilers.txt"
ASM_FUNCTIONS = ROOT / "config/AWRE/asm_functions.txt"
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
    starts: list[int] = field(default_factory=list)  # sorted addrs of function, data and bss symbols
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
    """The ARM9 modules. Other tools (loop_state.py) count on
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
            # A label (`_drsb`) is an entry point inside a function, not a new object.
            if not m.group(2).startswith("label"):
                mod.starts.append(int(m.group(3), 16))
    for line in (d / "relocs.txt").read_text().splitlines():
        m = RELOC.match(line)
        if m:
            mod.relocs.append((int(m.group(1), 16), m.group(2), int(m.group(3), 16), m.group(5)))
            if m.group(4):
                mod.addends[int(m.group(1), 16)] = int(m.group(4), 16)
    mod.text, mod.files = parse_delinks(d / "delinks.txt", mod.texts)
    mod.funcs.sort(key=lambda f: f.addr)
    mod.starts = sorted(set(mod.starts))
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


def next_object(f: Func) -> int:
    """Where the next object after f starts: the next function, data or bss
    symbol, or the end of f's .text section if that comes first."""
    mod = all_modules()[f.module]
    if f.module == "arm7":
        # The end of the .text section that holds f: ARM7 has one per part.
        end = next(e for s, e in mod.texts if s <= f.addr < e)
    else:
        end = mod.text[1]
    i = bisect.bisect_right(mod.starts, f.addr)
    return min(mod.starts[i], end) if i < len(mod.starts) else end


def next_start(f: Func) -> int:
    """The end of f's delinks.txt range: f's symbol size, cut at the next
    object (issue 0051). When the next object starts within the padding to
    the next 4-byte boundary (a Thumb function that ends on a halfword,
    MTX_RotY43_), the range takes the padding in: the compiled object holds
    it too. Bytes from the end to next_object(f) otherwise stay out, and
    trailing_gap() reports them."""
    stop = next_object(f)
    return stop if f.end <= stop <= (f.end + 3) & ~3 else min(f.end, stop)


def trailing_gap(f: Func) -> tuple[int, int] | None:
    """The bytes after f's range that no symbol names, as (start, end), or
    None. They may be data, or f's own literal pool or code when dsd sized
    the symbol too small (issue 0060); only a look at them can tell."""
    end, stop = next_start(f), next_object(f)
    return (end, stop) if end < stop else None


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
    gap = trailing_gap(f)
    if gap:
        print(f"note: 0x{gap[0]:08x}-0x{gap[1]:08x} after {f.name} has no symbol and stays out of the range."
              f" If it is {f.name}'s literal pool or code (issue 0060), extend the symbol size and the range.",
              file=sys.stderr)
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
    # make release checks the format.
    subprocess.run([sys.executable, ROOT / "tools/format.py", ROOT / fi[0]], check=True)
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
                # A file built with another compiler keeps it after the move.
                text = COMPILERS.read_text()
                new_text = re.sub(rf"^{re.escape(path)} ", f"{new_path} ", text, flags=re.M)
                if new_text != text:
                    COMPILERS.write_text(new_text)
                    print(f"updated {COMPILERS.relative_to(ROOT)}")
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
    # The list of asm functions names each one in its second column; keep
    # the reason column where it was.
    def asm_row(m: re.Match) -> str:
        width = len(m.group(2)) + len(m.group(3))
        return m.group(1) + new.ljust(width - 1) + " "
    text = ASM_FUNCTIONS.read_text()
    new_text = re.sub(rf"^(\S+\s+)({re.escape(old)})(\s+)", asm_row, text, flags=re.M)
    if new_text != text:
        ASM_FUNCTIONS.write_text(new_text)
        print(f"updated {ASM_FUNCTIONS.relative_to(ROOT)}")
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
    paths = next(((ROOT / project / u["target_path"], ROOT / project / u["base_path"])
                  for u in json.loads((ROOT / project / "objdiff.json").read_text())["units"]
                  if u["name"] == unit and "target_path" in u and "base_path" in u), None)
    scored = score_rows(json.loads(r.stdout), f.name, f.module, paths)
    if scored is None:
        print(f"error: {path} does not define {f.name}. Check the function name in the C file.")
        return 1
    left, right, same, percent = scored
    width = max((len(t) for t, _ in left), default=10) + 2
    paint = painter(args.color)
    print(paint("1", f"{'original':<{width}}   ours"))
    differ = count_differ(left, right, same)
    for i in range(max(len(left), len(right))):
        a, ka = left[i] if i < len(left) else ("", "missing")
        b, kb = right[i] if i < len(right) else ("", "missing")
        mark = "=" if i in same else "|" if (ka or kb) else " "
        # Colours as in `git diff`: the original in red, ours in green.
        # Pad before colouring so the columns stay aligned.
        if mark == "|":
            print(f"{paint('31', f'{a:<{width}}')} {mark} {paint('32', b)}")
        elif mark == "=":
            print(f"{a:<{width}} {paint('36', mark)} {b}")
        else:
            print(f"{a:<{width}} {mark} {b}")
    if same and not differ:
        percent = 100.0
    if same:
        print(f"\n= : {len(same)} row(s) where a relocation names another symbol at the same address"
              " or a symbol of the original word's value, a branch reaches the same address,"
              " or the bytes are the same but decoded differently; counted as matching.")
    print(f"\n{f.name}: {differ} differ, {percent if percent is not None else '?'}% match")
    return 0 if percent == 100.0 else 1


def objdiff(target: Path, base: Path, name: str) -> dict | None:
    """objdiff-cli's JSON for function `name`, with `target` the original's
    object and `base` ours. None when objdiff-cli fails."""
    r = subprocess.run([OBJDIFF, "diff", "-1", str(target), "-2", str(base), "-o", "-", "--format", "json", name],
                       cwd=ROOT, capture_output=True, text=True)
    return None if r.returncode else json.loads(r.stdout)


def score_rows(data: dict, name: str, module: str | None, objects: tuple[Path, Path] | None = None):
    """The rows `fn.py diff` prints, from objdiff-cli's JSON: (original
    rows, our rows, indices of the rows marked =, objdiff's percentage).
    A row is (text, objdiff's diff kind). None when our object does not
    define `name`. `objects` (the original's object, ours) lets rows match
    by their bytes (same_bytes); without it they can only match by
    relocation."""
    def side(key):
        for s in (data.get(key) or {}).get("symbols", []):
            if s.get("name") == name:
                return s
        return {}

    orig, ours = side("left"), side("right")
    if not ours:
        return None
    percent = orig.get("match_percent", ours.get("match_percent"))

    def rows(s):
        out = []
        for ins in s.get("instructions", []):
            i = ins.get("instruction")
            text = f"{int(i.get('address', 0)):4x}: {i['formatted']}" if i else ""
            out.append((text, ins.get("diff_kind")))
        return out

    # objdiff compares relocations by symbol name. Rows where both sides
    # point at the same address under different names (issue 0017) match,
    # and so do words relocated to an absolute symbol of the original value.
    # Issue 0077 adds branches that reach the same address (branch_target),
    # a plain word equal to our relocated one (resolved_word), and rows
    # whose bytes are the same in both objects (same_bytes).
    sections = [function_bytes(p, name) for p in objects] if objects else None
    same = {i for i, (a, b) in enumerate(zip(orig.get("instructions", []), ours.get("instructions", [])))
            if a.get("diff_kind") and (same_target(data, a, b) or absolute_word(data, a, b, module)
                                       or pool_word(data, a, b) or branch_target(data, a, b, name, module)
                                       or resolved_word(data, a, b)
                                       or (sections and same_bytes(a, b, sections)))}
    return rows(orig), rows(ours), same, percent


def count_differ(left, right, same) -> int:
    """How many rows `fn.py diff` marks |."""
    return sum(1 for i in range(max(len(left), len(right)))
               if i not in same and ((left[i][1] if i < len(left) else "missing")
                                     or (right[i][1] if i < len(right) else "missing")))


@dataclass
class DiffScore:
    """What `fn.py diff` printed: rows marked | (`=` rows not counted),
    objdiff's percentage, and the listing as (original, ours, mark) rows."""
    differ: int
    percent: float | None
    rows: list[tuple[str, str, str]]

    def rank(self) -> tuple[int, float]:
        return rank(self.differ, self.percent)


def rank(differ: int, percent: float | None) -> tuple[int, float]:
    """Sort key, closest first: fewest differing rows, then the highest
    percentage (issue 0067)."""
    return differ, -(percent or 0.0)


DIFF_LAST = re.compile(r"^\S+: (\d+) differ, ([\d.]+|\?)% match$", re.M)


def parse_diff(out: str) -> DiffScore | None:
    """Read `fn.py diff --color never` output. None when it has no listing
    (a compile error, a function the file does not define)."""
    lines = out.splitlines()
    head = next((i for i, ln in enumerate(lines) if ln.startswith("original")), None)
    m = DIFF_LAST.search(out)
    if head is None or not m:
        return None
    width = lines[head].index("ours") - 3
    rows = []
    for ln in lines[head + 1:]:
        if not ln.strip():
            break
        rows.append((ln[:width].strip(), ln[width + 3:].strip(), ln[width + 1:width + 2]))
    return DiffScore(int(m.group(1)), None if m.group(2) == "?" else float(m.group(2)), rows)


def painter(when: str):
    """A function that wraps text in an ANSI colour code. `auto` colours
    only on a terminal and when NO_COLOR is unset, so tools that read the
    output through a pipe (triage_partials.py, propagate.py) get plain text."""
    on = when == "always" or (when == "auto" and sys.stdout.isatty() and not os.environ.get("NO_COLOR"))
    return (lambda code, text: f"\033[{code}m{text}\033[m") if on else (lambda code, text: text)


def cmd_lines(args) -> int:
    f = find(args.name)
    if args.file:
        src = Path(args.file).resolve()
    else:
        fi = file_of(f)
        if not fi:
            sys.exit(f"error: {f.name} is not inside any delinks.txt file. Run fn.py add first, or give FILE.c.")
        src = ROOT / fi[0]
    import configure
    cc, cflags = configure.compiler_for(src, f.module)
    with tempfile.TemporaryDirectory() as d:
        obj = Path(d) / "lines.o"
        # -g adds the line table and leaves .text as it is.
        r = subprocess.run([str(ROOT / cc), "-c", *cflags.split(), "-g", "-o", str(obj), str(src)],
                           cwd=ROOT, capture_output=True, text=True)
        if r.returncode:
            sys.stdout.write(r.stdout + r.stderr)
            return 1
        # -z: a pool word is 0 until linked, and objdump would fold those into "...".
        cmd = [OBJDUMP, "-dlrz", "-marmv5te", "--no-show-raw-insn", f"--disassemble={f.name}", str(obj)]
        if f.mode == "thumb":
            cmd.insert(1, "-Mforce-thumb")
        out = subprocess.run(cmd, capture_output=True, text=True, check=True).stdout
    sources: dict[Path, list[str]] = {}
    blocks = []  # [header, [asm lines]]
    for line in out.splitlines():
        if m := re.match(r"(\S.*?):(\d+)(?: \(discriminator \d+\))?$", line):
            path = source_path(m.group(1))
            n = int(m.group(2))
            text = ""
            if path:
                if path not in sources:
                    sources[path] = path.read_text(errors="replace").splitlines()
                if n <= len(sources[path]):
                    text = sources[path][n - 1].strip()
            where = str(n) if path == src else f"{rel(path) if path else m.group(1)}:{n}"
            blocks.append([f"{where}: {text}", []])
        elif m := re.match(r"\s+([0-9a-f]+):\s+(R_ARM_\w+)\s+(\S+)", line):
            # A relocation: show its target instead of the placeholder.
            addr, kind, target = int(m.group(1), 16), m.group(2), m.group(3)
            asm, i = next(((a, i) for _, a in reversed(blocks) for i, x in enumerate(a)
                           if int(x.split(":", 1)[0], 16) == addr), (None, None))
            if asm is not None:
                if kind in ("R_ARM_PC24", "R_ARM_CALL", "R_ARM_JUMP24", "R_ARM_THM_CALL", "R_ARM_THM_PC22"):
                    asm[i] = re.sub(r"\s\S+ <.*>$", " " + re.sub(r"-0x[48]$", "", target), asm[i])
                elif asm[i].split(":", 1)[1].strip().startswith(".word"):
                    asm[i] = re.sub(r"\.word\s+\S+", ".word " + target, asm[i])
                else:
                    asm[i] += f"  ; {target}"
        elif m := re.match(r"\s+([0-9a-f]+):\s+(.*)", line):
            if not blocks:
                blocks.append(["?: (no line)", []])
            blocks[-1][1].append(f"{int(m.group(1), 16):4x}: {m.group(2).strip()}")
    # Name what a pc-relative load reads: the pool word's relocation target
    # or its value, as `ldr r1, =gCampaignMapState`.
    pool = {}
    for _, asm in blocks:
        for a in asm:
            if m := re.match(r"\s*([0-9a-f]+): \.word\s+(\S+)", a):
                pool[int(m.group(1), 16)] = m.group(2)
    for _, asm in blocks:
        for i, a in enumerate(asm):
            if (m := re.search(r"\[pc, #-?\d+\]\s+@ ([0-9a-f]+) <[^>]*>$", a)) and int(m.group(1), 16) in pool:
                asm[i] = a[:m.start()] + "=" + pool[int(m.group(1), 16)]
    print("\n\n".join(f"{head}\n---\n" + "\n".join(asm) for head, asm in blocks if asm))
    return 0


def source_path(name: str) -> Path | None:
    r"""The source file a line-table entry names. mwcc writes Windows-style
    paths and joins the directory twice (`\a\b\/\a\b\/x.c`), so try each
    tail of the path, longest first, until one exists."""
    parts = [p for p in name.replace("\\", "/").split("/") if p]
    for i in range(len(parts)):
        for base in (Path("/"), ROOT):
            p = base.joinpath(*parts[i:])
            if p.is_file():
                return p.resolve()
    return None


def rel(path: Path) -> str:
    return str(path.relative_to(ROOT)) if path.is_relative_to(ROOT) else str(path)


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


def pool_word(data, left, right) -> bool:
    """True when both rows are the same 32-bit word relocated to the same
    address, and objdiff shows one as `.word` and the other as an
    instruction. The original's object has no mapping symbols, so objdiff
    finds its literal pool words from the loads that read them, and it can
    miss the last word of a function (Isd_VFormat)."""
    if left.get("diff_kind") != "DIFF_REPLACE" or right.get("diff_kind") != "DIFF_REPLACE":
        return False
    targets, words = [], []
    for key, row in (("left", left), ("right", right)):
        ins = row["instruction"]
        rel = ins.get("relocation")
        if not rel or rel.get("type_name") != "R_ARM_ABS32" or ins.get("size") != 4:
            return False
        addr = symbol_addresses().get(data[key]["symbols"][rel["target_symbol"]]["name"])
        if addr is None:
            return False
        targets.append(addr + int(rel.get("addend", 0)))
        words.append(ins["formatted"].startswith(".word "))
    return left["instruction"]["address"] == right["instruction"]["address"] \
        and targets[0] == targets[1] and words[0] != words[1]


def absolute_word(data, left, right, module: str | None = None) -> bool:
    """True when the original row is a plain `.word N` and ours is a word
    relocated to a symbol from the module's absolute list
    (configure.absolute_symbols) whose value plus addend is N. dsd leaves no relocation there, because
    the original object has none to give."""
    from configure import absolute_symbols
    symbols = absolute_symbols(module)
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
    if name not in symbols:
        return False
    value = (symbols[name] + int(rel.get("addend", 0))) & 0xFFFFFFFF
    return int(largs[0]["unsigned"]) == value


# Added to a branch relocation's symbol plus addend, gives the address the
# branch reaches: the pc offset, 8 in ARM code and 4 in Thumb.
BRANCH_PC = {"R_ARM_PC24": 8, "R_ARM_CALL": 8, "R_ARM_JUMP24": 8, "R_ARM_XPC25": 8,
             "R_ARM_THM_CALL": 4, "R_ARM_THM_PC22": 4, "R_ARM_THM_XPC22": 4, "R_ARM_THM_JUMP24": 4}
# ARM condition codes by their number in bits 28-31, in objdiff's spelling.
CONDITIONS = ["eq", "ne", "hs", "lo", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le"]


def symbol_of(data, key: str, name: str) -> dict:
    """Function `name` on one side ("left" or "right") of objdiff-cli's JSON."""
    return next((s for s in (data.get(key) or {}).get("symbols", []) if s.get("name") == name), {})


@cache
def _module_parts() -> dict:
    import fnindex
    return fnindex.module_parts()


def rom_word(module: str | None, addr: int) -> int | None:
    """The original module's 32-bit word at `addr`, None when unknown."""
    import fnindex
    try:
        data, base = fnindex.part_of(_module_parts()[module], addr)
    except KeyError:
        return None
    off = addr - base
    return int.from_bytes(data[off:off + 4], "little") if off + 4 <= len(data) else None


def branch_target(data, left, right, name: str, module: str | None = None) -> bool:
    """True when both rows are the same branch to the same address (issue
    0077). A side reaches its address through a relocation (symbol, addend
    and pc offset) or, with no relocation, through objdiff's destination in
    its object, placed at function `name`'s address. dsd delinks a branch
    into a neighbouring function without a relocation when both sit in one
    object, and Isd_DisableFiq's `b 0x10` is that.

    dsd also writes `b #0` or `bl #0` over every branch it relocates, which
    loses the condition (docs/tool-issues.md): _dadd's `bmi` shows as `b` on
    the original's side. That row matches only when the ROM's word at that
    address has our condition."""
    kinds = ("DIFF_ARG_MISMATCH", "DIFF_OP_MISMATCH")
    if left.get("diff_kind") not in kinds or right.get("diff_kind") not in kinds:
        return False
    func = symbol_addresses().get(name)
    if func is None:
        return False
    targets, ops, starts = [], [], []
    for key, row in (("left", left), ("right", right)):
        ins = row["instruction"]
        args = [p["arg"] for p in ins.get("parts", []) if "arg" in p]
        ops.append(next((p["opcode"]["mnemonic"] for p in ins.get("parts", []) if "opcode" in p), ""))
        starts.append(int(symbol_of(data, key, name).get("address", 0)))
        rel = ins.get("relocation")
        if len(args) != 1:
            return False
        if rel and args[0].get("reloc"):
            pc = BRANCH_PC.get(rel.get("type_name"))
            addr = symbol_addresses().get(data[key]["symbols"][rel["target_symbol"]]["name"])
            if pc is None or addr is None:
                return False
            targets.append((addr + int(rel.get("addend", 0)) + pc) & 0xFFFFFFFF)
        elif "branch_dest" in args[0] and not rel:
            targets.append((func + int(args[0]["branch_dest"]) - starts[-1]) & 0xFFFFFFFF)
        else:
            return False
    if targets[0] != targets[1] or not ops[0].startswith("b"):
        return False
    if ops[0] == ops[1]:
        return True
    if ops[0] not in ("b", "bl") or not left["instruction"].get("relocation"):
        return False
    cond = ops[1][len(ops[0]):]
    if ops[1][:len(ops[0])] != ops[0] or cond not in CONDITIONS:
        return False
    word = rom_word(module, func + int(left["instruction"].get("address", 0)) - starts[0])
    return word is not None and word >> 28 == CONDITIONS.index(cond)


def resolved_word(data, left, right) -> bool:
    """True when the original row is a plain `.word N` and ours is a word
    relocated to a symbol whose address plus addend is N (issue 0077). dsd
    gives no relocation to a word that points into the middle of a
    function, like Isd_RestoreFiq's own address plus 8."""
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
    addr = symbol_addresses().get(data["right"]["symbols"][rel["target_symbol"]]["name"])
    return addr is not None and int(largs[0]["unsigned"]) == (addr + int(rel.get("addend", 0))) & 0xFFFFFFFF


def function_bytes(path: Path, name: str) -> tuple[bytes, int, set[int]] | None:
    """(bytes of the section holding function `name`, its offset there, the
    offsets of that section's relocations) from an object file."""
    import fnindex
    try:
        secs, syms, relas = fnindex.read_elf(Path(path).read_bytes())
    except (OSError, struct.error, IndexError, ValueError):
        return None
    sym = next((s for s in syms if s.name == name and s.kind == 2), None)  # STT_FUNC
    if sym is None:
        return None
    return secs[sym.shndx][1], sym.value, {off for off, *_ in relas.get(sym.shndx, [])}


def same_bytes(left, right, sections) -> bool:
    """True when every instruction in the row has no relocation and the same
    bytes as the other object at the same place in the function (issue
    0077). objdiff decodes words by the mapping symbols each object has:
    ours marks _fdiv's table and _ffix's hand-encoded branches as data, the
    original's object does not, so the same bytes show as `.word` on one
    side and an instruction on the other. A row only one side has, like
    the 8 bytes after _dfix that only our symbol covers, is compared with
    the other object's bytes at that place."""
    if not sections or not all(sections):
        return False
    rows = [left.get("instruction"), right.get("instruction")]
    if not any(rows):
        return False
    for k, ins in enumerate(rows):
        if ins is None:
            continue
        mine, other = sections[k], sections[1 - k]
        off, size = int(ins.get("address", 0)), int(ins.get("size", 0))
        there = off - mine[1] + other[1]
        if ins.get("relocation") or size <= 0 or off + size > len(mine[0]) or there < 0 \
                or there + size > len(other[0]):
            return False
        for (_, _, relocs), at in ((mine, off), (other, there)):
            if any(at - 3 <= r < at + size for r in relocs):
                return False
        if mine[0][off:off + size] != other[0][there:there + size]:
            return False
    return True


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
    p.add_argument("--color", choices=["auto", "always", "never"], default="auto",
                   help="colour differing lines as git diff does (default: on a terminal only)")
    p.set_defaults(fn=cmd_diff)
    p = sub.add_parser("check")
    p.add_argument("file")
    p.add_argument("name")
    p.add_argument("--module", help="for a name defined in several modules")
    p.set_defaults(fn=cmd_check)
    p = sub.add_parser("lines")
    p.add_argument("name")
    p.add_argument("file", nargs="?", help="compile this file instead of the function's own")
    p.set_defaults(fn=cmd_lines)
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
