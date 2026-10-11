#!/usr/bin/env python3
"""Write build.ninja. Run from the Makefile; paths are relative to the repo root.

Pipeline:
  orig/AWRE.nds --dsd rom extract--> extract/
  config/AWRE/arm9 + extract --dsd delink--> build/delinks/**.o (original bytes)
  src/**.c marked `complete` in a delinks.txt --mwccarm--> build/src/**.o
                             --dsd lcf-----> build/arm9.lcf, build/objects.txt
                             --lcf_absolute.py--> ARM9_ABSOLUTE_SYMBOLS added to build/arm9.lcf
  compiled objects, plus delinked ones for everything else --mwldarm--> build/arm9.elf, build/build/*.bin (one per module)
  ELF --dsd rom config--> build/build/rom_config.yaml (ARM7 and files from extract/)
  --dsd rom build--> build/AWRE.raw.nds --fix_header.py--> build/AWRE.nds
"""

import sys
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = Path("config/AWRE/arm9")

DSD = ".cache/tools/dsd/dsd"
SKREW_BIN = ".cache/tools/metroskrew/bin"

# Fingerprint result (docs/awre-facts.md): mwccarm 2.0 builds 72-82 produce
# the same code for AWRE. 2.0-72 and mwldarm 2.0-72 make up the CodeWarrior
# for DS 1.2 base package.
CC = f"{SKREW_BIN}/mwccarm-2.0-72.exe"
LD = f"{SKREW_BIN}/mwldarm-2.0-72.exe"
CFLAGS = "-O4,p -proc arm946e -interworking -nostdinc -i include -msgstyle gcc"
LDFLAGS = "-proc arm946e -interworking -map closure,unused -msgstyle gcc -nodead -nostdlib -m Entry"

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


def config_inputs() -> list[str]:
    files = sorted((ROOT / CONFIG).rglob("*.txt")) + [ROOT / CONFIG / "config.yaml"]
    return [p.relative_to(ROOT).as_posix() for p in files]


def delinks_sources() -> list[str]:
    """Every file path in a delinks.txt that exists under src/. All of them
    are compiled, so a work-in-progress file can be diffed. Only the ones
    marked `complete` replace the original bytes in the link (dsd lcf
    decides that)."""
    sources = []
    for delinks in sorted((ROOT / CONFIG).rglob("delinks.txt")):
        for line in delinks.read_text().splitlines():
            if line and not line[0].isspace() and line.endswith(":"):
                if (ROOT / line[:-1]).exists():
                    sources.append(line[:-1])
    return sources


def obj_of(src: str) -> str:
    return "build/" + str(Path(src).with_suffix(".o"))


def main() -> int:
    tools = tomllib.loads((ROOT / "tools.lock").read_text())
    cfg = (CONFIG / "config.yaml").as_posix()
    sources = delinks_sources()
    objs = [obj_of(src) for src in sources]
    lines = [
        "ninja_required_version = 1.10",
        f"# dsd {tools['dsd']['version']}, metroskrew {tools['metroskrew']['version']}",
        f"python = {sys.executable}",
        f"dsd = {DSD}",
        f"cc = {CC}",
        f"cflags = {CFLAGS}",
        f"ld = {LD}",
        f"ldflags = {LDFLAGS}",
        "",
        "rule extract",
        "  command = rm -rf extract && $dsd rom extract -r $in -o extract > /dev/null",
        "  description = EXTRACT $in",
        "",
        # -gccdep -MD writes build/src/x.d next to the object.
        "rule cc",
        "  command = $cc -c $cflags -gccdep -MD -o $out $in",
        "  description = CC $in",
        "  depfile = $out_d",
        "  deps = gcc",
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
        "  command = $dsd lcf -c $cfg > /dev/null && $python tools/lcf_absolute.py $lcf $absolute",
        "  description = LCF",
        "",
        # tools/link.py runs mwldarm from a short directory under /tmp: its
        # link time grows with objects^2 x path length (docs/tool-issues.md).
        "rule link",
        "  command = $python tools/link.py $ld $out build/arm9.lcf build/objects.txt $ldflags",
        "  description = LINK $out",
        "",
        "rule romconfig",
        "  command = $dsd rom config -e $in -c $cfg > /dev/null",
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
        "build build/arm9.lcf build/objects.txt: lcf build/delink.stamp | $dsd tools/lcf_absolute.py",
        f"  cfg = {cfg}",
        "  lcf = build/arm9.lcf",
        "  absolute = " + " ".join(f"{k}={v:#x}" for k, v in ARM9_ABSOLUTE_SYMBOLS.items()),
        "",
        f"build build/arm9.elf: link build/arm9.lcf build/objects.txt | $ld tools/link.py {' '.join(objs)}",
        "",
        f"build objdiff.json: objdiff_json build/delink.stamp",
        f"  cfg = {cfg}",
        "",
        "build build/build/rom_config.yaml: romconfig build/arm9.elf | $dsd",
        f"  cfg = {cfg}",
        "",
        "build build/AWRE.raw.nds: rombuild build/build/rom_config.yaml | $dsd",
        "",
        "build build/AWRE.nds: fixheader build/AWRE.raw.nds | orig/AWRE.nds tools/fix_header.py",
        "",
        "default build/AWRE.nds",
        "",
    ]
    for src, obj in zip(sources, objs):
        lines += [f"build {obj}: cc {src}", f"  out_d = {obj[:-2]}.d", ""]
    # The Makefile runs this before every build (new src/ files have to be
    # picked up); writing only on change keeps ninja from redoing work.
    text = "\n".join(lines)
    out = ROOT / "build.ninja"
    if not out.exists() or out.read_text() != text:
        out.write_text(text)
    return 0


if __name__ == "__main__":
    sys.exit(main())
