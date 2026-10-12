#!/usr/bin/env python3
"""Function index and fast match check.

    fnindex.py [--out build/fnindex.json]     build the index (a few seconds)

The index lists every function in config/AWRE/arm9/**/symbols.txt and
config/AWRE/arm7/symbols.txt (module `arm7`) with its
module, mode, address, size, original bytes (from extract/), relocations
(from relocs.txt), a masked hash, the mnemonics before the literal pool, and
whether a C file covers it (`status`: complete, started or none).
`mnemonics` is one space-separated string; `raw` is hex; relocations are
[offset in the function, kind, target address, target module, name]; the
address and name include relocs.txt's `add:` (`Ov000_gUnitStats+0x1e`).

The masked hash covers the bytes with every relocated word zeroed, the size,
and the list of relocation offsets and kinds. Two functions with the same
hash differ only in what they call or load.

`check()` compiles one C file with the build's compiler command into a
temporary object and compares one function against the index: bytes outside
relocations, relocation offsets and kinds, and relocation targets as
addresses. A relocation to `Table+0x1c` equals one to the symbol dsd placed
at that address (issue 0017). `fn.py check` is the command-line front end.
It never runs ninja or dsd and never writes to config/. An `arm7` function
is compiled with the ARM7 flags (configure.CFLAGS7).

Based on khdays-decomp (CC0): the index follows tools/index_funcs.py, the
check follows tools/match.py and tools/verify_idx.py. Ideas only, no code
copied. See docs/third-party/khdays-decomp.md.
"""

import argparse
import hashlib
import json
import re
import struct
import subprocess
import sys
import tempfile
import time
from dataclasses import dataclass
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import configure  # noqa: E402
import fn  # noqa: E402

ROOT = fn.ROOT
INDEX = ROOT / "build/fnindex.json"
EXTRACT = ROOT / "extract"
VERSION = 3

# Relocation kinds, grouped by what part of the word they change.
ARM_CALLS = {"arm_call", "arm_call_thumb", "arm_branch"}
THUMB_CALLS = {"thumb_call", "thumb_call_arm"}


def reloc_class(kind: str) -> str:
    if kind in ARM_CALLS:
        return "arm_call"
    if kind in THUMB_CALLS:
        return "thumb_call"
    return "load"


# ---------------------------------------------------------------- index

def module_bins() -> dict[str, tuple[bytes, int]]:
    """Module name -> (bytes, base address) for every module with code."""
    def base(yaml: Path) -> int:
        return int(re.search(r"^base_address: (\d+)", yaml.read_text(), re.M).group(1))
    out = {
        "main": ((EXTRACT / "arm9/arm9.bin").read_bytes(), base(EXTRACT / "arm9/arm9.yaml")),
        "itcm": ((EXTRACT / "arm9/itcm.bin").read_bytes(), base(EXTRACT / "arm9/itcm.yaml")),
    }
    cur = None
    for line in (EXTRACT / "arm9_overlays/overlays.yaml").read_text().splitlines():
        m = re.match(r"\s*- id: (\d+)", line)
        if m:
            cur = {"id": int(m.group(1))}
            continue
        m = re.match(r"\s+(base_address|file_name): (\S+)", line)
        if m and cur is not None:
            cur[m.group(1)] = m.group(2)
            if len(cur) == 3:
                data = (EXTRACT / "arm9_overlays" / cur["file_name"]).read_bytes()
                out[f"ov{cur['id']:03d}"] = (data, int(cur["base_address"]))
                cur = None
    return out


def module_parts() -> dict[str, list[tuple[bytes, int]]]:
    """Module name -> [(bytes, base address)]: one part per ARM9 module, and
    ARM7's three parts (static, WRAM and main RAM autoloads)."""
    out = {name: [part] for name, part in module_bins().items()}
    arm7_bin = EXTRACT / "arm7/arm7.bin"
    if arm7_bin.exists() and "arm7" in fn.all_modules():
        import arm7
        data = arm7_bin.read_bytes()
        out["arm7"] = [(data[r.offset:r.offset + r.size], r.base) for r in arm7.regions(data)]
    return out


def part_of(parts: list[tuple[bytes, int]], addr: int) -> tuple[bytes, int]:
    for data, base in parts:
        if base <= addr < base + len(data):
            return data, base
    raise KeyError(f"0x{addr:08x} is in no part of its module")


def masked(raw: bytes, relocs) -> bytes:
    """raw with every relocated word zeroed."""
    b = bytearray(raw)
    for off, *_ in relocs:
        b[off:off + 4] = bytes(4)
    return bytes(b)


def masked_hash(mode: str, raw: bytes, relocs) -> str:
    h = hashlib.sha1()
    h.update(f"{mode} {len(raw)} ".encode())
    h.update(masked(raw, relocs))
    h.update(repr([(off, reloc_class(kind)) for off, kind, *_ in relocs]).encode())
    return h.hexdigest()[:16]


DIS = re.compile(r"^\s*([0-9a-f]+):\s+[0-9a-f]+(?: [0-9a-f]+)?\s+(\S+)\s*(.*)$")
PC_LOAD = re.compile(r"\[pc(?:, #(-?\d+))?\]")


def disassemble(data: bytes, base: int, start: int, stop: int, thumb: bool) -> dict[int, tuple[str, str]]:
    """Address -> (mnemonic, operands), via objdump on a slice of the module."""
    with tempfile.NamedTemporaryFile(suffix=".bin") as tmp:
        tmp.write(data[start - base:stop - base])
        tmp.flush()
        args = ["arm-none-eabi-objdump", "-D", "-b", "binary", "-marm", "-EL",
                f"--adjust-vma={start:#x}", tmp.name]
        if thumb:
            args.insert(1, "-Mforce-thumb")
        out = subprocess.run(args, capture_output=True, text=True, check=True).stdout
    d = {}
    for line in out.splitlines():
        m = DIS.match(line)
        if m:
            d[int(m.group(1), 16)] = (m.group(2), m.group(3).split(";")[0].strip())
    return d


def pool_start(f: fn.Func, dis: dict) -> int:
    """Offset where the literal pool starts: the lowest address any
    pc-relative load in the function reads from."""
    step = 2 if f.mode == "thumb" else 4
    pool = f.end
    for a in range(f.addr, f.end, step):
        mn, ops = dis.get(a, ("?", ""))
        if a >= pool:
            break
        m = PC_LOAD.search(ops)
        if m and mn.startswith("ldr"):
            pc = (a + 4) & ~3 if f.mode == "thumb" else a + 8
            pool = min(pool, max(a + step, pc + int(m.group(1) or 0)))
    return pool - f.addr


def build() -> dict:
    # Imported here: it costs 30 ms, and `fn.py check` never builds.
    from concurrent.futures import ThreadPoolExecutor
    mods = fn.all_modules()
    bins = module_parts()

    def mod_dis(name):
        mod = mods[name]
        d = {}
        for data, base in bins[name]:
            arm = [f for f in mod.funcs if f.mode == "arm" and base <= f.addr < base + len(data)]
            if arm:
                d.update(disassemble(data, base, min(f.addr for f in arm), max(f.end for f in arm), False))
        for f in mod.funcs:
            if f.mode == "thumb":
                d.update(disassemble(*part_of(bins[name], f.addr), f.addr, f.end, True))
        return d

    names = [n for n in mods if n in bins and mods[n].funcs]
    with ThreadPoolExecutor() as ex:
        dis = dict(zip(names, ex.map(mod_dis, names)))

    funcs = []
    for name in names:
        mod = mods[name]
        complete = [(s, e, p) for p, s, e, c in mod.files if c]
        started = [(s, e, p) for p, s, e, c in mod.files if not c]
        for f in mod.funcs:
            data, base = part_of(bins[name], f.addr)
            raw = data[f.addr - base:f.end - base]
            relocs = []
            for frm, kind, to, dest in fn.relocs_in(f):
                add = mod.addends.get(frm, 0)
                label = fn.name_at(dest, to) + (f"{add:+#x}" if add else "")
                relocs.append((frm - f.addr, kind, to + add, dest, label))
            pool = pool_start(f, dis[name])
            step = 2 if f.mode == "thumb" else 4
            # One string, not a list: the index loads three times faster.
            mnemonics = " ".join(dis[name].get(a, ("?",))[0] for a in range(f.addr, f.addr + pool, step))
            status, path = "none", None
            for s, e, p in complete:
                if s <= f.addr < e:
                    status, path = "complete", p
            for s, e, p in started:
                if s <= f.addr < e:
                    status, path = "started", p
            funcs.append({
                "name": f.name, "module": name, "mode": f.mode, "addr": f.addr, "size": f.size,
                "raw": raw.hex(), "relocs": relocs, "mhash": masked_hash(f.mode, raw, relocs),
                "pool": pool, "mnemonics": mnemonics, "status": status, "file": path,
            })
    symbols: dict[str, list] = {}
    for mod in mods.values():
        for line in (mod.dir / "symbols.txt").read_text().splitlines():
            m = fn.ANY_SYM.match(line)
            if m:
                symbols.setdefault(m.group(1), []).append([mod.name, int(m.group(3), 16)])
    return {"version": VERSION, "functions": funcs, "symbols": symbols}


def inputs() -> list[Path]:
    """Files the index is built from. delinks.txt is left out on purpose:
    it changes on every `fn.py add`, and only `status` depends on it."""
    out = [p for d in (fn.CONFIG, fn.CONFIG7) for p in d.rglob("*.txt") if p.name in ("symbols.txt", "relocs.txt")]
    return out + [EXTRACT / "arm9/arm9.bin", EXTRACT / "arm9_overlays/overlays.yaml"]


def write(path: Path = INDEX) -> dict:
    index = build()
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(".tmp")
    tmp.write_text(json.dumps(index, separators=(",", ":")))
    tmp.replace(path)
    return index


@dataclass
class Index:
    by_name: dict[str, list[dict]]
    symbols: dict[str, list[tuple[str, int]]]

    def func(self, name: str, module: str | None = None) -> dict:
        hits = [f for f in self.by_name.get(name, []) if module in (None, f["module"])]
        if not hits:
            raise KeyError(f"no function {name} in the index")
        return hits[0]


def load(path: Path = INDEX) -> Index:
    """Load the index, rebuilding it first if it is missing or older than
    symbols.txt, relocs.txt or extract/."""
    if not path.exists() or path.stat().st_mtime < max(p.stat().st_mtime for p in inputs()):
        print(f"fnindex: rebuilding {path.relative_to(ROOT)}", file=sys.stderr)
        data = write(path)
    else:
        data = json.loads(path.read_text())
    if data.get("version") != VERSION:
        data = write(path)
    by_name: dict[str, list[dict]] = {}
    for f in data["functions"]:
        by_name.setdefault(f["name"], []).append(f)
    symbols = {k: [tuple(x) for x in v] for k, v in data["symbols"].items()}
    return Index(by_name, symbols)


# ---------------------------------------------------------------- check

R_ARM_PC24, R_ARM_ABS32, R_ARM_THM_CALL, R_ARM_CALL, R_ARM_JUMP24 = 1, 2, 10, 28, 29
ELF_CLASS = {R_ARM_PC24: "arm_call", R_ARM_CALL: "arm_call", R_ARM_JUMP24: "arm_call",
             R_ARM_THM_CALL: "thumb_call", R_ARM_ABS32: "load"}
# What the CPU adds to a branch's offset: the pc reads 8 (ARM) or 4 (Thumb)
# bytes ahead, and the addend in the object already subtracts that.
PC_BIAS = {"arm_call": 8, "thumb_call": 4, "load": 0}


@dataclass
class ElfSymbol:
    name: str
    value: int
    size: int
    shndx: int
    kind: int  # STT_*


def read_elf(data: bytes):
    """Sections as [(name, bytes)] by index, symbols, and RELA entries by
    the index of the section they patch: [(offset, type, symbol, addend)]."""
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2e)
    secs = [struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize) for i in range(shnum)]

    def body(s):
        return data[s[4]:s[4] + s[5]] if s[1] != 8 else bytes(s[5])  # SHT_NOBITS
    shstr = body(secs[shstrndx])

    def cstr(tab, off):
        return tab[off:tab.index(b"\0", off)].decode()
    names = [cstr(shstr, s[0]) for s in secs]
    syms: list[ElfSymbol] = []
    relas: dict[int, list] = {}
    for s in secs:
        if s[1] == 2:  # SHT_SYMTAB
            strtab = body(secs[s[6]])
            tab = body(s)
            for i in range(0, len(tab), 16):
                st_name, value, size, info, _, shndx = struct.unpack_from("<IIIBBH", tab, i)
                syms.append(ElfSymbol(cstr(strtab, st_name), value, size, shndx, info & 0xf))
    for s in secs:
        if s[1] == 4:  # SHT_RELA
            tab = body(s)
            relas[s[7]] = [(off, info & 0xff, info >> 8, addend)
                           for off, info, addend in struct.iter_unpack("<IIi", tab)]
    return [(n, body(s)) for n, s in zip(names, secs)], syms, relas


@dataclass
class Result:
    name: str
    ok: bool
    units: int          # instructions (ARM words or Thumb halfwords) compared
    differ: int         # of those, how many differ
    bytes_differ: int
    message: str = ""
    # Relocations that reach the right address under another name, like
    # Table+0x1c for dsd's data_xxx (issue 0017). They count as matching.
    renamed: int = 0
    compiler_output: str = ""

    def __str__(self):
        if self.ok:
            return f"{self.name}: match"
        if not self.units:
            return f"{self.name}: {self.message}"
        return (f"{self.name}: {self.differ} of {self.units} instructions differ "
                f"({self.bytes_differ} bytes){'; ' + self.message if self.message else ''}")


def compile_c(src: Path, obj: Path, module: str | None = None) -> subprocess.CompletedProcess:
    """The `cc` rule from tools/configure.py (`cc7` for an ARM7 function or a
    file under src/arm7/), with the object in a temp dir."""
    cc, cflags = configure.compiler_for(src, module)
    cmd = [str(ROOT / cc), "-c", *cflags.split(), "-gccdep", "-MD",
           "-o", str(obj), str(src)]
    return subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)


def compare(index: Index, obj_data: bytes, name: str, module: str | None = None) -> Result:
    """Compare function `name` in a compiled object with the original."""
    want = index.func(name, module)
    raw = bytes.fromhex(want["raw"])
    unit = 2 if want["mode"] == "thumb" else 4
    nunits = (len(raw) + unit - 1) // unit
    secs, syms, relas = read_elf(obj_data)
    sym = next((s for s in syms if s.name == name and s.shndx not in (0, 0xfff1) and s.kind == 2), None)
    if sym is None:
        return Result(name, False, nunits, nunits, len(raw), "the object does not define this function")
    sec_data = secs[sym.shndx][1]
    ours = sec_data[sym.value:sym.value + sym.size]

    def resolve(s: ElfSymbol, addend: int) -> set[tuple[str, int, str]]:
        """(module, address, name) candidates for a relocation's target."""
        label = s.name + (f"{addend:+#x}" if addend else "")
        if s.name in index.symbols and s.kind != 3:  # not a section symbol
            return {(m, a + addend, label) for m, a in index.symbols[s.name]}
        if s.shndx == sym.shndx:
            # Defined next to our function in the same section: the offset
            # holds if the section is laid out as in the original.
            return {(want["module"], want["addr"] + s.value - sym.value + addend, label)}
        return set()

    # Our relocations inside the function: offset -> (class, targets).
    # A word relocated to an absolute symbol (configure.absolute_symbols)
    # matches an original word of that value, which has no relocation.
    got, absolute_bytes = {}, set()
    absolute = configure.absolute_symbols(want["module"])
    for off, rtype, si, addend in relas.get(sym.shndx, []):
        if sym.value <= off < sym.value + sym.size:
            cls = ELF_CLASS.get(rtype, f"elf type {rtype}")
            value = absolute.get(syms[si].name)
            at = off - sym.value
            if (cls == "load" and value is not None and syms[si].shndx == 0
                    and raw[at:at + 4] == ((value + addend) & 0xFFFFFFFF).to_bytes(4, "little")):
                absolute_bytes.update(range(at, at + 4))
                continue
            got[off - sym.value] = (cls, resolve(syms[si], addend + PC_BIAS.get(cls, 0)))
    exp = {}
    for off, kind, to, dest, label in want["relocs"]:
        exp[off] = (reloc_class(kind), kind, to, set(fn.target_module(dest)), label)

    # Bytes a relocation fills in. An ARM call keeps its top byte (condition
    # and bl/b), except that the linker turns bl into blx for a Thumb target.
    skip = set(absolute_bytes)
    for off in set(exp) | set(got):
        cls = (exp.get(off) or got[off])[0]
        skip.update(range(off, off + (3 if cls == "arm_call" else 4)))
    blx = {off + 3 for off, e in exp.items() if e[1] == "arm_call_thumb"}

    bad_units = set()
    bad_bytes = renamed = 0
    notes = []
    if len(ours) != len(raw):
        notes.append(f"size {len(ours):#x}, original {len(raw):#x}")
    for i in range(max(len(ours), len(raw))):
        a = ours[i] if i < len(ours) else None
        b = raw[i] if i < len(raw) else None
        if i in skip or (i in blx and a == 0xeb and b in (0xfa, 0xfb)):
            continue
        if a != b:
            bad_bytes += 1
            bad_units.add(i // unit)
    for off in sorted(set(exp) | set(got)):
        e, g = exp.get(off), got.get(off)
        if e is None or g is None or e[0] != g[0]:
            bad_units.add(off // unit)
            continue
        _, _, to, mods, label = e
        hits = [n for m, addr, n in g[1] if m in mods and addr == to]
        if not hits:
            bad_units.add(off // unit)
        elif label not in hits:
            renamed += 1
    units = max(nunits, (len(ours) + unit - 1) // unit)
    ok = not bad_units and len(ours) == len(raw)
    return Result(name, ok, units, len(bad_units), bad_bytes, "; ".join(notes), renamed)


def compile_object(src: Path, tmpdir: Path | None = None, module: str | None = None) -> tuple[bytes | None, str]:
    """Compile src; return the object's bytes (None on an error) and the
    compiler's output."""
    with tempfile.TemporaryDirectory(dir=tmpdir) as d:
        obj = Path(d) / "check.o"
        r = compile_c(Path(src).resolve(), obj, module)
        return (None if r.returncode else obj.read_bytes()), r.stdout + r.stderr


def check(index: Index, src: Path, name: str, module: str | None = None, tmpdir: Path | None = None) -> Result:
    """Compile src and compare function `name` with the original."""
    try:
        target_module = index.func(name, module)["module"]
    except KeyError:
        target_module = module
    obj, output = compile_object(src, tmpdir, target_module)
    if obj is None:
        return Result(name, False, 0, 0, 0, "compile error", 0, output)
    return compare(index, obj, name, module)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--out", type=Path, default=INDEX)
    args = ap.parse_args()
    t = time.time()
    index = write(args.out)
    out = args.out.resolve()
    shown = out.relative_to(ROOT) if out.is_relative_to(ROOT) else out
    print(f"fnindex: {len(index['functions'])} functions -> {shown} ({time.time() - t:.1f} s)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
