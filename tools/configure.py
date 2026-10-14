#!/usr/bin/env python3
"""Write build.ninja. Run from the Makefile; paths are relative to the repo root.

Pipeline:
  orig/AWRE.nds --dsd rom extract--> extract/
  config/AWRE/arm9 + extract --dsd delink--> build/delinks/**.o (original bytes)
  assets/maps/ + extracted/AWRE/assets/maps/ --tools/maps.py pack--> build/gen/maps/ (C included by src/game/ov000/map/)
  config/AWRE/data_inc.txt + extract --tools/data_inc.py write--> extracted/AWRE/data/ (values that C files include)
  config/AWRE/bss_gen.txt + delinks.txt + symbols.txt + include/ --tools/bss.py gen--> build/gen/bss/
                             (.bss definitions included by the files bss_gen.txt lists)
  src/**.c marked `complete` in a delinks.txt --mwccarm--> build/src/**.o
                             --dsd lcf-----> build/arm9.lcf, build/objects.txt
                             --lcf_absolute.py--> ARM9_ABSOLUTE_SYMBOLS added to build/arm9.lcf
                             --maps.py lcf--> the symbols inside Ov000_gMapHeaders added to build/arm9.lcf
                             --bss.py lcf--> .bss.runN sections and the symbols inside .bss variables added
                             --lcf_align2.py lcf--> ALIGNALL(2) around objects at 2 mod 4
                             --lcf_align2.py objects--> build/objects.link.txt, build/align2/**.o
  compiled objects, plus delinked ones for everything else --mwldarm--> build/arm9.elf, build/build/*.bin (one per module)
  ELF --dsd rom config--> build/build/rom_config.yaml (files from extract/)
  --dsd rom build--> build/AWRE.raw.nds --fix_header.py--> build/AWRE.nds

ARM7 (tools/arm7.py says why it takes extra steps):
  config/AWRE/arm7 + extract --arm7.py prepare--> build/arm7/rom/, build/arm7/config/ (stand-in ROM and dsd config)
  --dsd delink--> build/arm7/delinks/**.o
  src/arm7/**.c marked `complete` --mwccarm (ARM7 flags)--> build/arm7/src/arm7/**.o
  --dsd lcf, arm7.py lcf, lcf_absolute.py, bss.py lcf --arm7,
    lcf_align2.py lcf--> build/arm7/arm7.lcf, build/arm7/objects.txt
  --lcf_align2.py objects--> build/arm7/objects.link.txt, build/arm7/align2/**.o
  --mwldarm--> build/arm7/arm7.elf, build/arm7/build/*.bin --arm7.py bin--> build/build/arm7.bin
  rom_config.yaml points at build/build/arm7.bin (arm7.py romconfig).
"""

import re
import sys
import tomllib
from pathlib import Path

import data_inc
import maps

ROOT = Path(__file__).resolve().parent.parent
CONFIG = Path("config/AWRE/arm9")
CONFIG7 = Path("config/AWRE/arm7")
ARM7_CFG = "build/arm7/config/config.yaml"

DSD = ".cache/tools/dsd/dsd"
SKREW_BIN = ".cache/tools/metroskrew/bin"

# Fingerprint result (docs/awre-facts.md): mwccarm 2.0 builds 72-82 produce
# the same code for AWRE. 2.0-72 and mwldarm 2.0-72 make up the CodeWarrior
# for DS 1.2 base package.
CC = f"{SKREW_BIN}/mwccarm-2.0-72.exe"
LD = f"{SKREW_BIN}/mwldarm-2.0-72.exe"
CFLAGS = "-O4,p -proc arm946e -interworking -nostdinc -i include -i build/gen -i extracted/AWRE -msgstyle gcc"
LDFLAGS = "-proc arm946e -interworking -map closure,unused -msgstyle gcc -nodead -nostdlib -m Entry"

# Some MSL files were built by an older compiler, mwccarm 2.0 build 56
# (issue 0061). config/AWRE/compilers.txt lists them; tools/wine_cc.py runs
# that compiler under wine with the same flags.
COMPILERS = Path("config/AWRE/compilers.txt")
CC_BY_VERSION = {"2.0-72": CC, "2.0-56": "tools/wine_cc.py"}

# ARM7 (docs/awre-facts.md, "ARM7"): the same compiler, built for size.
CC7 = CC
CFLAGS7 = "-O4,s -proc arm7tdmi -interworking -nostdinc -i include -i build/gen -i extracted/AWRE -msgstyle gcc"
LDFLAGS7 = "-proc arm7tdmi -interworking -map closure,unused -msgstyle gcc -nodead -nostdlib -m Entry"

# Symbols with a fixed value, written into build/arm9.lcf after dsd lcf
# (tools/lcf_absolute.py). The original link script defines these link-time
# constants; the ROM's literal pools hold their values. The SDK code tests
# them at run time, so our C must reference them as symbols (OS_InitThread,
# func_0210e36c). `fn.py diff` and `fn.py check` count a relocation to one of
# these as matching a plain word of the same value.
ARM9_ABSOLUTE_SYMBOLS = {
    "SDK_SYS_STACKSIZE": 0x00000000,
    "SDK_IRQ_STACKSIZE": 0x00000400,
    "SDK_SECTION_ARENA_DTCM_START": 0x027C12E0,
}

# The same for ARM7 (build/arm7/arm7.lcf). Its SDK build has a 0x400-byte
# system stack where ARM9's is 0 (Arena_DefaultHighEnd_Arm7).
ARM7_ABSOLUTE_SYMBOLS = {
    "SDK_SYS_STACKSIZE": 0x00000400,
    "SDK_IRQ_STACKSIZE": 0x00000400,
}


def absolute_symbols(module: str | None) -> dict[str, int]:
    """The link-time constants of a module's lcf: ARM7's for `arm7`,
    ARM9's for every other module."""
    return ARM7_ABSOLUTE_SYMBOLS if module == "arm7" else ARM9_ABSOLUTE_SYMBOLS


# The map tables (tools/maps.py, docs/maps.md): what `maps.py pack` writes,
# and the sources that include it.
MAP_OUTPUTS = [f"build/gen/maps/{n}" for n in ("map_data.h", "map_headers.inc", "map_tiles.inc", "map_units.inc")]
MAP_SOURCES = [f"src/game/ov000/map/map_{n}.c" for n in ("headers", "tiles", "units")]


def map_inputs() -> list[str]:
    """The committed legend and glue files, the unpacked maps (`make setup`) and
    the stamp unpack writes last, so a missing unpack fails the build by name."""
    files = sorted((ROOT / maps.ASSETS).glob("*.yaml")) + sorted((ROOT / maps.ASSETS).glob("*.txt"))
    files += sorted((ROOT / maps.EXTRACTED).rglob("*.yaml"))
    return [p.relative_to(ROOT).as_posix() for p in files] + [(maps.EXTRACTED / ".unpacked").as_posix()]


# Data from the ROM (tools/data_inc.py, docs/workflow.md): the .inc.c files
# `data_inc.py write` makes, and the C files that include each one.
DATA_INC_INCLUDE = re.compile(r'^#include "(data/[^"]+\.inc\.c)"', re.M)


def data_inc_users(sources: list[str]) -> dict[str, list[str]]:
    """Source file -> the data_inc outputs it includes."""
    users = {}
    for src in sources:
        found = DATA_INC_INCLUDE.findall((ROOT / src).read_text(errors="replace"))
        if found:
            users[src] = [f"extracted/AWRE/{rel}" for rel in found]
    return users


# Generated .bss definitions (tools/bss.py gen, docs/bss-tool.md): the files
# config/AWRE/bss_gen.txt lists, each with the build/gen/bss/<stem>.inc.c it
# includes. Read here rather than imported from bss.py, which imports this.
BSS_GEN = Path("config/AWRE/bss_gen.txt")


def bss_gen_outputs() -> dict[str, str]:
    """Listed source file -> its generated .inc.c."""
    path = ROOT / BSS_GEN
    if not path.exists():
        return {}
    return {line.strip(): f"build/gen/bss/{Path(line.strip()).stem}.inc.c"
            for line in path.read_text().splitlines() if line.strip() and not line.startswith("#")}


def bss_gen_inputs(listed: list[str]) -> list[str]:
    """What `bss.py gen` reads: the list, the delinks.txt and symbols.txt of
    each module that has a listed file, and every header."""
    inputs = [BSS_GEN.as_posix()]
    for delinks in sorted((ROOT / "config/AWRE").rglob("delinks.txt")):
        names = {line[:-1] for line in delinks.read_text().splitlines()
                 if line and not line[0].isspace() and line.endswith(":")}
        if names & set(listed):
            rel = delinks.relative_to(ROOT)
            inputs += [rel.as_posix(), (rel.parent / "symbols.txt").as_posix()]
    inputs += sorted(p.relative_to(ROOT).as_posix() for p in (ROOT / "include").rglob("*.h"))
    return inputs


def config_inputs() -> list[str]:
    files = sorted((ROOT / CONFIG).rglob("*.txt")) + [ROOT / CONFIG / "config.yaml"]
    return [p.relative_to(ROOT).as_posix() for p in files]


def delinks_sources(config: Path = CONFIG) -> list[str]:
    """Every file path in a delinks.txt that exists under src/. All of them
    are compiled, so a work-in-progress file can be diffed. Only the ones
    marked `complete` replace the original bytes in the link (dsd lcf
    decides that)."""
    sources = []
    for delinks in sorted((ROOT / config).rglob("delinks.txt")):
        for line in delinks.read_text().splitlines():
            if line and not line[0].isspace() and line.endswith(":"):
                if (ROOT / line[:-1]).exists():
                    sources.append(line[:-1])
    return sources


def is_arm7_source(src) -> bool:
    p = Path(src)
    if p.is_absolute() and p.is_relative_to(ROOT):
        p = p.relative_to(ROOT)
    return p.parts[:2] == ("src", "arm7")


def compiler_table() -> dict[str, str]:
    """Source path -> compiler for the files in config/AWRE/compilers.txt."""
    table = {}
    for n, line in enumerate((ROOT / COMPILERS).read_text().splitlines(), 1):
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        path, version = line.split()
        if version not in CC_BY_VERSION:
            sys.exit(f"error: {COMPILERS}:{n}: unknown compiler {version!r}")
        table[path] = CC_BY_VERSION[version]
    return table


def compiler_for(src, module: str | None = None) -> tuple[str, str]:
    """(compiler, flags) for a source file: ARM7's for a file under src/arm7/
    or for code checked against an `arm7` function, ARM9's otherwise. The
    compiler is the one compilers.txt names for the file, or 2.0-72."""
    if module == "arm7" or (module is None and is_arm7_source(src)):
        return CC7, CFLAGS7
    p = Path(src)
    if p.is_absolute() and p.is_relative_to(ROOT):
        p = p.relative_to(ROOT)
    return compiler_table().get(p.as_posix(), CC), CFLAGS


def obj_of(src: str) -> str:
    """Where a source file's object goes. ARM7 objects sit under build/arm7/,
    the build directory of ARM7's dsd config, where dsd lcf looks for them."""
    if is_arm7_source(src):
        return "build/arm7/" + str(Path(src).with_suffix(".o"))
    return "build/" + str(Path(src).with_suffix(".o"))


def main() -> int:
    tools = tomllib.loads((ROOT / "tools.lock").read_text())
    cfg = (CONFIG / "config.yaml").as_posix()
    sources = delinks_sources()
    objs = [obj_of(src) for src in sources]
    sources7 = delinks_sources(CONFIG7)
    objs7 = [obj_of(src) for src in sources7]
    data_inc_rows = data_inc.read_list()
    data_inc_outputs = [(data_inc.OUT / r.rel).as_posix() for r in data_inc_rows]
    data_inc_deps = data_inc_users(sources + sources7)
    bss_gen = bss_gen_outputs()
    for src, out in bss_gen.items():
        data_inc_deps.setdefault(src, []).append(out)
    config7 = " ".join(f"{CONFIG7.as_posix()}/{n}.txt" for n in ("delinks", "symbols", "relocs"))
    lines = [
        "ninja_required_version = 1.10",
        f"# dsd {tools['dsd']['version']}, metroskrew {tools['metroskrew']['version']}",
        f"python = {sys.executable}",
        f"dsd = {DSD}",
        f"cc = {CC}",
        f"cflags = {CFLAGS}",
        f"ld = {LD}",
        f"ldflags = {LDFLAGS}",
        f"cc7 = {CC7}",
        f"cflags7 = {CFLAGS7}",
        f"ldflags7 = {LDFLAGS7}",
        "",
        "rule extract",
        # Replaces everything in extract/ except README.md and .gitkeep, which are in git.
        "  command = rm -rf extract.tmp && $dsd rom extract -r $in -o extract.tmp > /dev/null"
        " && mkdir -p extract && find extract -mindepth 1 -maxdepth 1 ! -name README.md ! -name .gitkeep -exec rm -rf {} +"
        " && mv extract.tmp/* extract/ && rmdir extract.tmp",
        "  description = EXTRACT $in",
        "",
        # -gccdep -MD writes build/src/x.d next to the object.
        "rule cc",
        "  command = $cc -c $cflags -gccdep -MD -o $out $in",
        "  description = CC $in",
        "  depfile = $out_d",
        "  deps = gcc",
        "",
        "rule cc7",
        "  command = $cc7 -c $cflags7 -gccdep -MD -o $out $in",
        "  description = CC $in",
        "  depfile = $out_d",
        "  deps = gcc",
        "",
        "rule maps",
        "  command = $python tools/maps.py pack",
        "  description = MAPS",
        "  restat = 1",
        "",
        "rule data_inc",
        "  command = $python tools/data_inc.py write --quiet",
        "  description = DATA INC",
        "  restat = 1",
        "",
        "rule bss_gen",
        "  command = $python tools/bss.py gen build/gen",
        "  description = BSS GEN",
        "  restat = 1",
        "",
        "rule objdiff_json",
        "  command = $dsd objdiff -c $cfg -o . > /dev/null",
        "  description = OBJDIFF JSON",
        "",
        "rule delink",
        "  command = rm -rf build/delinks && $dsd delink -c $cfg > /dev/null && touch $out",
        "  description = DELINK",
        "",
        "rule lcf",
        "  command = $dsd lcf -c $cfg > /dev/null && $python tools/lcf_absolute.py $lcf $absolute"
        " && $python tools/maps.py lcf $lcf"
        " && $python tools/bss.py lcf $lcf"
        " && $python tools/lcf_align2.py lcf $lcf config/AWRE/arm9/delinks.txt",
        "  description = LCF",
        "",
        # tools/link.py runs mwldarm from a short directory under /tmp: its
        # link time grows with objects^2 x path length (docs/tool-issues.md).
        "rule link",
        # tools/lcf_align2.py copies the objects that start at 2 mod 4 with
        # .text alignment 2 and lists the copies in build/objects.link.txt.
        "  command = $python tools/lcf_align2.py objects build/arm9.lcf build/objects.txt"
        " build/objects.link.txt build/align2"
        " && $python tools/link.py $ld $out build/arm9.lcf build/objects.link.txt $ldflags",
        "  description = LINK $out",
        "",
        "rule arm7_prepare",
        "  command = $python tools/arm7.py prepare",
        "  description = ARM7 PREPARE",
        "",
        "rule arm7_delink",
        f"  command = rm -rf build/arm7/delinks && $dsd delink -c {ARM7_CFG} > /dev/null && touch $out",
        "  description = ARM7 DELINK",
        "",
        "rule arm7_lcf",
        f"  command = $dsd lcf -c {ARM7_CFG} > /dev/null && $python tools/arm7.py lcf"
        " && $python tools/lcf_absolute.py build/arm7/arm7.lcf $absolute"
        " && $python tools/bss.py lcf --arm7 build/arm7/arm7.lcf"
        " && $python tools/lcf_align2.py lcf build/arm7/arm7.lcf config/AWRE/arm7/delinks.txt"
        " AUTOLOAD_2 AUTOLOAD_3 AUTOLOAD_4",
        "  description = ARM7 LCF",
        "",
        "rule arm7_link",
        "  command = $python tools/lcf_align2.py objects build/arm7/arm7.lcf build/arm7/objects.txt"
        " build/arm7/objects.link.txt build/arm7/align2"
        " && $python tools/link.py $ld $out build/arm7/arm7.lcf build/arm7/objects.link.txt $ldflags7",
        "  description = LINK $out",
        "",
        "rule arm7_bin",
        "  command = $python tools/arm7.py bin",
        "  description = ARM7 BIN $out",
        "",
        "rule arm7_objdiff_json",
        f"  command = $dsd objdiff -c {ARM7_CFG} -o build/arm7 > /dev/null",
        "  description = OBJDIFF JSON (ARM7)",
        "",
        "rule romconfig",
        "  command = $dsd rom config -e $in -c $cfg > /dev/null && $python tools/arm7.py romconfig",
        "  description = ROM CONFIG",
        "",
        "rule rombuild",
        "  command = $dsd rom build -c $in -o $out > /dev/null",
        "  description = ROM BUILD $out",
        "",
        "rule fixheader",
        "  command = $python tools/fix_header.py $in orig/AWRE.nds $out",
        "  description = FIX HEADER $out",
        "",
        "build extract/config.yaml: extract orig/AWRE.nds",
        "",
        f"build build/delink.stamp: delink {' '.join(config_inputs())} | extract/config.yaml $dsd",
        f"  cfg = {cfg}",
        "",
        "build build/arm9.lcf build/objects.txt: lcf build/delink.stamp | $dsd tools/lcf_absolute.py tools/lcf_align2.py tools/maps.py tools/bss.py",
        f"  cfg = {cfg}",
        "  lcf = build/arm9.lcf",
        "  absolute = " + " ".join(f"{k}={v:#x}" for k, v in ARM9_ABSOLUTE_SYMBOLS.items()),
        "",
        f"build {' '.join(MAP_OUTPUTS)}: maps {' '.join(map_inputs())} | tools/maps.py",
        "",
        f"build {' '.join(data_inc_outputs)}: data_inc {data_inc.LIST.as_posix()}"
        f" {' '.join(data_inc.config_inputs(data_inc_rows))} | extract/config.yaml tools/data_inc.py tools/arm7.py",
        "",
        *([f"build {' '.join(sorted(set(bss_gen.values())))}: bss_gen {' '.join(bss_gen_inputs(list(bss_gen)))}"
           " | tools/bss.py tools/mwcc_order.py", ""] if bss_gen else []),
        f"build build/arm9.elf: link build/arm9.lcf build/objects.txt | $ld tools/link.py tools/lcf_align2.py {' '.join(objs)}",
        "",
        f"build objdiff.json: objdiff_json build/delink.stamp",
        f"  cfg = {cfg}",
        "",
        f"build build/arm7/prepare.stamp: arm7_prepare {config7} | extract/config.yaml tools/arm7.py tools/fxhash.py",
        "",
        "build build/arm7/delink.stamp: arm7_delink build/arm7/prepare.stamp | $dsd",
        "",
        "build build/arm7/arm7.lcf build/arm7/objects.txt: arm7_lcf build/arm7/delink.stamp"
        " | $dsd tools/arm7.py tools/lcf_align2.py tools/lcf_absolute.py tools/bss.py",
        "  absolute = " + " ".join(f"{k}={v:#x}" for k, v in ARM7_ABSOLUTE_SYMBOLS.items()),
        "",
        "build build/arm7/arm7.elf: arm7_link build/arm7/arm7.lcf build/arm7/objects.txt"
        f" | $ld tools/link.py tools/lcf_align2.py {' '.join(objs7)}",
        "",
        "build build/build/arm7.bin: arm7_bin build/arm7/arm7.elf | tools/arm7.py",
        "",
        "build build/arm7/objdiff.json: arm7_objdiff_json build/arm7/delink.stamp",
        "",
        "build build/build/rom_config.yaml: romconfig build/arm9.elf | $dsd build/build/arm7.bin tools/arm7.py",
        f"  cfg = {cfg}",
        "",
        "build build/AWRE.raw.nds: rombuild build/build/rom_config.yaml | $dsd",
        "",
        "build build/AWRE.nds: fixheader build/AWRE.raw.nds | orig/AWRE.nds tools/fix_header.py",
        "",
        "default build/AWRE.nds",
        "",
    ]
    table = compiler_table()
    for src, obj in zip(sources, objs):
        implicit = MAP_OUTPUTS if src in MAP_SOURCES else []
        implicit = implicit + data_inc_deps.get(src, [])
        if src in table:
            implicit = implicit + [table[src], COMPILERS.as_posix()]
        generated = f" | {' '.join(implicit)}" if implicit else ""
        lines += [f"build {obj}: cc {src}{generated}", f"  out_d = {obj[:-2]}.d"]
        if src in table:
            lines.append(f"  cc = {table[src]}")
        lines.append("")
    for src, obj in zip(sources7, objs7):
        generated = f" | {' '.join(data_inc_deps[src])}" if src in data_inc_deps else ""
        lines += [f"build {obj}: cc7 {src}{generated}", f"  out_d = {obj[:-2]}.d", ""]
    # The Makefile runs this before every build (new src/ files have to be
    # picked up); writing only on change keeps ninja from redoing work.
    text = "\n".join(lines)
    out = ROOT / "build.ninja"
    if not out.exists() or out.read_text() != text:
        out.write_text(text)
    return 0


if __name__ == "__main__":
    sys.exit(main())
