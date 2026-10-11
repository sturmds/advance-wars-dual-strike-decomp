#!/usr/bin/env python3
"""Build steps that let dsd delink ARM7 (issue 0013).

dsd 0.12.1 handles only ARM9 programs, and its function parser refuses any
branch outside 0x01ff8000-0x03000000 ("branch outside of program"). Most
ARM7 code runs from ARM7 WRAM at 0x037f8000 or calls into it, so dsd can
neither analyse nor delink it at its real addresses (docs/tool-issues.md).

ARM7 has three parts, read from the module parameters in arm7.bin:

  static   0x02380000  0x170 bytes of crt0 code, loaded by the card loader
  wram     0x037f8000  autoload into ARM7 WRAM
  main     0x027c0000  autoload into the top of main RAM

config/AWRE/arm7/ describes all three at their real addresses, as one
module. To delink, this script writes a stand-in ROM that dsd accepts: a
small made-up ARM9 main module, empty ITCM and DTCM, and the three ARM7
parts as dsd "unknown autoloads", with wram and main moved to addresses
inside dsd's range (FAKE below). At each relocation it rewrites the
original word so that branches and pointers point at the moved copies;
the linker overwrites those words anyway. It also writes a dsd config with
the same addresses moved. dsd delinks that, and the link script (with the
real addresses put back) makes mwldarm place every object at its real
address. Then `bin` puts arm7.bin together from the three linked parts and
the autoload table.

    arm7.py prepare      write build/arm7/rom/ and build/arm7/config/
    arm7.py lcf          build/arm7/arm9.lcf -> build/arm7/arm7.lcf (real addresses)
    arm7.py bin          build/arm7/arm7.elf and parts -> build/build/arm7.bin
    arm7.py romconfig    point build/build/rom_config.yaml at the built arm7.bin
"""

import re
import struct
import sys
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from fxhash import fxhash64  # noqa: E402

EXTRACT = ROOT / "extract"
ARM7_BIN = EXTRACT / "arm7/arm7.bin"
CONFIG = ROOT / "config/AWRE/arm7"
BUILD = ROOT / "build/arm7"

STATIC_BASE = 0x02380000
# The crt0 literal pool holds the module parameters: autoload list start and
# end, autoload start, static .bss start and end.
PARAMS = 0x158
# Where the stand-in ROM puts each part. Inside dsd's range, above the
# 0x01ff8000-0x02400000 window dsd uses to spot tables of pointers, and
# clear of every real ARM7 address.
FAKE = {"static": STATIC_BASE, "wram": 0x02a00000, "main": 0x02c00000}
# dsd's autoload index for each part in the stand-in ROM (0 and 1 are the
# empty ITCM and DTCM).
INDEX = {"static": 2, "wram": 3, "main": 4}

ADDR_FIELD = re.compile(r"\b(addr|start|end|from|to):(0x[0-9a-f]+)")
MODULE_FIELD = re.compile(r"\bmodule:(\S+)")
SECTION = re.compile(r"^\s+(\.\w+)\s+start:(0x[0-9a-f]+) end:(0x[0-9a-f]+)")


@dataclass
class Region:
    name: str
    base: int
    size: int
    bss: int
    offset: int  # in arm7.bin

    @property
    def fake(self) -> int:
        return FAKE[self.name]

    @property
    def index(self) -> int:
        return INDEX[self.name]

    @property
    def end(self) -> int:
        """End of the region's .bss, which starts 32-byte aligned."""
        return align(self.base + self.size, 32) + self.bss

    def holds(self, addr: int) -> bool:
        return self.base <= addr <= self.end


def align(x: int, n: int) -> int:
    return (x + n - 1) & ~(n - 1)


def regions(arm7: bytes) -> list[Region]:
    list_start, list_end, autoload_start, bss_start, bss_end = struct.unpack_from("<5I", arm7, PARAMS)
    out = [Region("static", STATIC_BASE, autoload_start - STATIC_BASE, bss_end - bss_start, 0)]
    offset = autoload_start - STATIC_BASE
    table = arm7[list_start - STATIC_BASE:list_end - STATIC_BASE]
    names = {0x037f8000: "wram", 0x027c0000: "main"}
    for base, size, bss in struct.iter_unpack("<3I", table):
        out.append(Region(names[base], base, size, bss, offset))
        offset += size
    if offset != list_start - STATIC_BASE or list_end - STATIC_BASE != len(arm7):
        sys.exit("arm7.py: arm7.bin does not have the expected layout")
    return out


def region_of(regs: list[Region], addr: int) -> Region | None:
    for r in regs:
        if r.holds(addr):
            return r
    return None


def to_fake(regs: list[Region], addr: int) -> int:
    r = region_of(regs, addr)
    return addr - r.base + r.fake if r else addr


def fake_region_of(regs: list[Region], addr: int) -> Region | None:
    for r in regs:
        if r.fake <= addr <= r.fake + (r.end - r.base):
            return r
    return None


def to_real(regs: list[Region], addr: int) -> int:
    r = fake_region_of(regs, addr)
    return addr - r.fake + r.base if r else addr


def move_fields(line: str, fn) -> str:
    return ADDR_FIELD.sub(lambda m: f"{m.group(1)}:0x{fn(int(m.group(2), 16)):08x}", line)


# ---------------------------------------------------------------- branches

def branch_target(word: int, pc: int, kind: str) -> int | None:
    """Target of the ARM or Thumb branch at pc, for relocation kinds."""
    if kind in ("arm_call", "arm_branch", "arm_call_thumb"):
        imm = word & 0xffffff
        if imm & 0x800000:
            imm -= 1 << 24
        h = (word >> 23) & 2 if word >> 28 == 0xf else 0
        return pc + 8 + imm * 4 + h
    if kind in ("thumb_call", "thumb_call_arm"):
        hi, lo = word & 0xffff, word >> 16
        imm = ((hi & 0x7ff) << 12) | ((lo & 0x7ff) << 1)
        if imm & 0x400000:
            imm -= 1 << 23
        target = pc + 4 + imm
        return target & ~3 if kind == "thumb_call_arm" else target
    return None


def encode_branch(word: int, pc: int, target: int, kind: str) -> int:
    if kind in ("arm_call", "arm_branch", "arm_call_thumb"):
        imm = (target - pc - 8) >> 2
        if not -(1 << 23) <= imm < 1 << 23:
            raise ValueError(f"ARM branch at 0x{pc:08x} cannot reach 0x{target:08x}")
        if word >> 28 == 0xf:  # BLX: bit 24 holds bit 1 of the Thumb target
            return (word & 0xfe000000) | ((target & 2) << 23) | (imm & 0xffffff)
        return (word & 0xff000000) | (imm & 0xffffff)
    base = pc + 4
    if kind == "thumb_call_arm":
        base &= ~3
    imm = target - base
    if not -(1 << 22) <= imm < 1 << 22:
        raise ValueError(f"Thumb branch at 0x{pc:08x} cannot reach 0x{target:08x}")
    hi = (word & 0xf800) | ((imm >> 12) & 0x7ff)
    lo = (word >> 16 & 0xf800) | ((imm >> 1) & 0x7ff)
    return hi | lo << 16


def moved_word(regs: list[Region], word: int, frm: int, kind: str, to: int) -> int:
    """The word at frm as it reads once the regions are moved to FAKE."""
    tr = region_of(regs, to)
    if kind == "load":
        return (word + (tr.fake - tr.base)) & 0xffffffff if tr else word
    target = branch_target(word, frm, kind)
    if target is None:
        return word
    return encode_branch(word, to_fake(regs, frm), to_fake(regs, target), kind)


# ---------------------------------------------------------------- stand-in ROM

FAKE_MAIN = 0x02000000
# A made-up ARM9 crt0 that dsd's ARM9 analysis accepts: a secure area of
# zeros, an entry point that calls a static-initializer runner and jumps to
# main, an empty .ctor list, and build info. It links into a part we throw
# away.
FAKE_MAIN_CODE = [
    0xeb000002, 0xe59f1000, 0xe12fff11, FAKE_MAIN + 0x81c,  # entry: bl inits; ldr r1, =main; bx r1
    0xe59f0000, 0xe12fff1e, FAKE_MAIN + 0x824,               # inits: ldr r0, =ctor; bx lr
    0xe12fff1e,                                              # main: bx lr
    0xe12fff1e,                                              # autoload callback: bx lr
    0,                                                       # .ctor end
]
FAKE_MAIN_CONFIG = {
    "delinks.txt": """\
    .text       start:0x02000000 end:0x02000824 kind:code align:32
    .ctor       start:0x02000824 end:0x02000824 kind:rodata align:4
    .data       start:0x02000840 end:0x0200084c kind:data align:32
    .bss        start:0x02000860 end:0x02000860 kind:bss align:32
""",
    "symbols.txt": """\
Entry kind:function(arm,size=0x10) addr:0x02000800
func_02000810 kind:function(arm,size=0xc) addr:0x02000810
main kind:function(arm,size=0x4) addr:0x0200081c
AutoloadCallback kind:function(arm,size=0x4) addr:0x02000820
BuildInfo kind:data(any) addr:0x02000828
""",
    "relocs.txt": """\
from:0x02000800 kind:arm_call to:0x02000810 module:main
from:0x0200080c kind:load to:0x0200081c module:main
from:0x02000818 kind:link_time_const(ARM9_CTOR_START)
""",
    "itcm/delinks.txt": """\
    .text       start:0x01ff8000 end:0x01ff8004 kind:code align:32
    .bss        start:0x01ff8020 end:0x01ff8020 kind:bss align:32
""",
    "itcm/symbols.txt": "func_01ff8000 kind:function(arm,size=0x4) addr:0x01ff8000\n",
    "itcm/relocs.txt": "",
    "dtcm/delinks.txt": """\
    .data       start:0x027e0000 end:0x027e0004 kind:data align:32
    .bss        start:0x027e0020 end:0x027e0020 kind:bss align:32
""",
    "dtcm/symbols.txt": "",
    "dtcm/relocs.txt": "",
}
ITCM = (0x01ff8000, struct.pack("<I", 0xe12fff1e))
DTCM = (0x027e0000, bytes(4))


def fake_main() -> tuple[bytes, int]:
    code = bytes(0x800) + struct.pack(f"<{len(FAKE_MAIN_CODE)}I", *FAKE_MAIN_CODE)
    build_info = len(code)
    end = FAKE_MAIN + build_info + 0x24
    code += struct.pack("<9I", 0, 0, end, end, end, 0, 0, 0xdec00621, 0x2106c0de)
    return code, build_info


def write_if_changed(path: Path, data: bytes | str) -> None:
    data = data.encode() if isinstance(data, str) else data
    if not path.exists() or path.read_bytes() != data:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)


def write_rom(out: Path, parts: dict[str, bytes], regs: list[Region]) -> None:
    """A stand-in extract directory for `dsd init`/`dsd delink`. Header and
    banner come from the real extract; the file system is left empty."""
    main, build_info = fake_main()
    write_if_changed(out / "arm9.bin", main)
    end = FAKE_MAIN + len(main)
    write_if_changed(out / "arm9.yaml", f"""\
base_address: {FAKE_MAIN}
entry_function: {FAKE_MAIN + 0x800}
build_info: {build_info}
autoload_callback: {FAKE_MAIN + 0x820}
overlay_signatures: 0
encrypted: false
compressed: false
bss_start: {end}
bss_end: {end}
sdk_version: 0
""")

    def autoload(name, base, data, bss, kind):
        write_if_changed(out / f"{name}.bin", data)
        write_if_changed(out / f"{name}.yaml",
                         f"base_address: {base}\ncode_size: {len(data)}\nbss_size: {bss}\nkind: {kind}\n")

    autoload("itcm", ITCM[0], ITCM[1], 0, "Itcm")
    autoload("dtcm", DTCM[0], DTCM[1], 0, "Dtcm")
    unknown = ""
    for r in regs:
        autoload(f"autoload_{r.index}", r.fake, parts[r.name], r.bss, f"!Unknown {r.index}")
        unknown += f"  - index: {r.index}\n    bin: autoload_{r.index}.bin\n    config: autoload_{r.index}.yaml\n"
    write_if_changed(out / "overlays.yaml", "table_signed: false\noverlays: []\n")
    write_if_changed(out / "files/.keep", "")
    write_if_changed(out / "path_order.txt", "")
    cfg = (EXTRACT / "config.yaml").read_text()
    rel = "../" * len(out.relative_to(ROOT).parts) + "extract"
    for key in ("header", "header_logo", "arm7_bin", "arm7_config", "banner", "arm9_hmac_sha1_key"):
        cfg = re.sub(rf"^{key}: (\S+)$", lambda m: f"{key}: {rel}/{m.group(1)}", cfg, flags=re.M)
    cfg = re.sub(r"^arm9_bin: .*$", "arm9_bin: arm9.bin", cfg, flags=re.M)
    cfg = re.sub(r"^arm9_config: .*$", "arm9_config: arm9.yaml", cfg, flags=re.M)
    cfg = re.sub(r"^files_dir: .*$", "files_dir: files/", cfg, flags=re.M)
    cfg = re.sub(r"^path_order: .*$", "path_order: path_order.txt", cfg, flags=re.M)
    cfg = cfg.replace("arm9/itcm.", "itcm.").replace("arm9/dtcm.", "dtcm.")
    cfg = re.sub(r"^arm9_overlays: .*$", "unknown_autoloads:\n" + unknown + "arm9_overlays: overlays.yaml",
                 cfg, flags=re.M)
    write_if_changed(out / "config.yaml", cfg)


def original_parts(arm7: bytes, regs: list[Region]) -> dict[str, bytearray]:
    return {r.name: bytearray(arm7[r.offset:r.offset + r.size]) for r in regs}


# ---------------------------------------------------------------- config

@dataclass
class Config:
    sections: list[str]          # delinks.txt header lines
    files: list[list[str]]       # delinks.txt file blocks, as lines
    symbols: list[str]
    relocs: list[str]


def load_config(d: Path = CONFIG) -> Config:
    head, _, rest = (d / "delinks.txt").read_text().partition("\n\n")
    files = [b.splitlines() for b in rest.strip("\n").split("\n\n") if b.strip()]
    return Config([ln for ln in head.splitlines() if ln.strip()], files,
                  [ln for ln in (d / "symbols.txt").read_text().splitlines() if ln.strip()],
                  [ln for ln in (d / "relocs.txt").read_text().splitlines() if ln.strip()])


def first_addr(line: str) -> int:
    m = ADDR_FIELD.search(line)
    return int(m.group(2), 16)


def reloc_fields(line: str) -> tuple[int, str, int | None]:
    frm = int(re.search(r"from:(0x[0-9a-f]+)", line).group(1), 16)
    kind = re.search(r"kind:(\S+)", line).group(1)
    to = re.search(r"\bto:(0x[0-9a-f]+)", line)
    return frm, kind, int(to.group(1), 16) if to else None


def fake_config(cfg: Config, regs: list[Region], out: Path, parts: dict[str, bytes]) -> None:
    """Write the dsd config for the stand-in ROM: each region in its own
    autoload directory, addresses moved to FAKE."""
    per = {r.name: {"sections": [], "files": [], "symbols": [], "relocs": []} for r in regs}

    def where(addr: int) -> str:
        r = region_of(regs, addr)
        if r is None:
            sys.exit(f"arm7.py: address 0x{addr:08x} is outside ARM7")
        return r.name

    for line in cfg.sections:
        per[where(first_addr(line))]["sections"].append(move_fields(line, lambda a: to_fake(regs, a)))
    for block in cfg.files:
        addrs = [first_addr(ln) for ln in block if SECTION.match(ln)]
        names = {where(a) for a in addrs}
        if len(names) != 1:
            sys.exit(f"arm7.py: {block[0]} must have sections in exactly one ARM7 region")
        per[names.pop()]["files"].append("\n".join(move_fields(ln, lambda a: to_fake(regs, a)) for ln in block))
    for line in cfg.symbols:
        per[where(first_addr(line))]["symbols"].append(move_fields(line, lambda a: to_fake(regs, a)))
    for line in cfg.relocs:
        frm, kind, to = reloc_fields(line)
        moved = move_fields(line, lambda a: to_fake(regs, a))
        if to is not None and (tr := region_of(regs, to)):
            moved = MODULE_FIELD.sub(f"module:autoload({tr.index})", moved)
        per[where(frm)]["relocs"].append(moved)

    for name, text in FAKE_MAIN_CONFIG.items():
        write_if_changed(out / name, text)
    autoloads = ""
    for r in regs:
        p = per[r.name]
        d = f"autoload_{r.index}"
        write_if_changed(out / d / "delinks.txt",
                         "\n".join(p["sections"]) + "\n\n" + "\n\n".join(p["files"]) + ("\n" if p["files"] else ""))
        write_if_changed(out / d / "symbols.txt", "".join(s + "\n" for s in p["symbols"]))
        write_if_changed(out / d / "relocs.txt", "".join(s + "\n" for s in p["relocs"]))
        autoloads += f"""\
  - kind:
      Unknown: {r.index}
    name: {d}
    object: ../build/{d}.bin
    hash: {fxhash64(parts[r.name]):016x}
    delinks: {d}/delinks.txt
    symbols: {d}/symbols.txt
    relocations: {d}/relocs.txt
"""
    main, _ = fake_main()
    write_if_changed(out / "config.yaml", f"""\
rom_config: ../rom/config.yaml
build_path: ..
delinks_path: ../delinks
main_module:
  name: main
  object: ../build/arm9.bin
  hash: {fxhash64(main):016x}
  delinks: delinks.txt
  symbols: symbols.txt
  relocations: relocs.txt
autoloads:
  - kind: Itcm
    name: itcm
    object: ../build/itcm.bin
    hash: {fxhash64(ITCM[1]):016x}
    delinks: itcm/delinks.txt
    symbols: itcm/symbols.txt
    relocations: itcm/relocs.txt
  - kind: Dtcm
    name: dtcm
    object: ../build/dtcm.bin
    hash: {fxhash64(DTCM[1]):016x}
    delinks: dtcm/delinks.txt
    symbols: dtcm/symbols.txt
    relocations: dtcm/relocs.txt
{autoloads}overlays:
  []
""")


def cmd_prepare() -> int:
    arm7 = ARM7_BIN.read_bytes()
    regs = regions(arm7)
    cfg = load_config()
    parts = original_parts(arm7, regs)
    for line in cfg.relocs:
        frm, kind, to = reloc_fields(line)
        if to is None:
            continue
        r = region_of(regs, frm)
        off = frm - r.base
        # A Thumb BL is two halfwords; every relocated field fits in 4 bytes.
        word = int.from_bytes(parts[r.name][off:off + 4], "little")
        parts[r.name][off:off + 4] = moved_word(regs, word, frm, kind, to).to_bytes(4, "little")
    write_rom(BUILD / "rom", parts, regs)
    fake_config(cfg, regs, BUILD / "config", parts)
    (BUILD / "prepare.stamp").touch()
    return 0


# ---------------------------------------------------------------- after the link

def cmd_lcf() -> int:
    regs = regions(ARM7_BIN.read_bytes())
    text = (BUILD / "arm9.lcf").read_text()
    for r in regs:
        text, n = re.subn(rf"(AUTOLOAD_{r.index} : ORIGIN = )0x[0-9a-f]+", rf"\g<1>0x{r.base:08x}", text)
        if n != 1:
            sys.exit(f"arm7.py: no AUTOLOAD_{r.index} region in build/arm7/arm9.lcf")
    write_if_changed(BUILD / "arm7.lcf", text)
    return 0


def elf_symbols(path: Path) -> dict[str, int]:
    """Global symbol values of a 32-bit little-endian ELF."""
    data = path.read_bytes()
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", data, 0x2e)
    sections = [struct.unpack_from("<10I", data, shoff + i * shentsize) for i in range(shnum)]
    out = {}
    for sh in sections:
        if sh[1] != 2:  # SHT_SYMTAB
            continue
        strtab = sections[sh[6]]
        for off in range(sh[4], sh[4] + sh[5], 16):
            name, value = struct.unpack_from("<II", data, off)
            end = data.index(b"\0", strtab[4] + name)
            out[data[strtab[4] + name:end].decode()] = value
    return out


def cmd_bin() -> int:
    regs = regions(ARM7_BIN.read_bytes())
    syms = elf_symbols(BUILD / "arm7.elf")
    out = b""
    table = b""
    for r in regs:
        n = f"AUTOLOAD_{r.index}"
        size = syms[f"{n}_DATA_END"] - r.base
        bss = syms[f"{n}_BSS_END"] - syms[f"{n}_BSS_START"]
        part = (BUILD / f"build/autoload_{r.index}.bin").read_bytes()[:size]
        if len(part) != size:
            sys.exit(f"arm7.py: build/arm7/build/autoload_{r.index}.bin is shorter than {n}_DATA_END")
        out += part
        if r.name != "static":
            table += struct.pack("<3I", r.base, size, bss)
    write_if_changed(ROOT / "build/build/arm7.bin", out + table)
    return 0


def cmd_romconfig() -> int:
    path = ROOT / "build/build/rom_config.yaml"
    text, n = re.subn(r"^arm7_bin: .*$", "arm7_bin: arm7.bin", path.read_text(), flags=re.M)
    if n != 1:
        sys.exit("arm7.py: no arm7_bin line in build/build/rom_config.yaml")
    path.write_text(text)
    return 0


def main() -> int:
    cmds = {"prepare": cmd_prepare, "lcf": cmd_lcf, "bin": cmd_bin, "romconfig": cmd_romconfig}
    if len(sys.argv) != 2 or sys.argv[1] not in cmds:
        sys.exit(__doc__)
    return cmds[sys.argv[1]]()


if __name__ == "__main__":
    sys.exit(main())
