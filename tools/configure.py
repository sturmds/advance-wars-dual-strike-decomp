#!/usr/bin/env python3
"""Write build.ninja. Run from the Makefile; paths are relative to the repo root.

Pipeline (M0, no C yet):
  orig/AWRE.nds --dsd rom extract--> extract/
  config/AWRE/arm9 + extract --dsd delink--> build/delinks/*.o
                             --dsd lcf-----> build/arm9.lcf, build/objects.txt
  objects --mwldarm--> build/arm9.elf, build/build/*.bin (one per module)
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
CFLAGS = "-O4,p -proc arm946e -interworking"
LDFLAGS = "-proc arm946e -interworking -map closure,unused -msgstyle gcc -nodead -nostdlib -m Entry"


def config_inputs() -> list[str]:
    files = sorted((ROOT / CONFIG).rglob("*.txt")) + [ROOT / CONFIG / "config.yaml"]
    return [p.relative_to(ROOT).as_posix() for p in files]


def main() -> int:
    tools = tomllib.loads((ROOT / "tools.lock").read_text())
    cfg = (CONFIG / "config.yaml").as_posix()
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
        "rule configure",
        "  command = $python tools/configure.py",
        "  description = CONFIGURE",
        "  generator = 1",
        "",
        "rule extract",
        "  command = rm -rf extract && $dsd rom extract -r $in -o extract > /dev/null",
        "  description = EXTRACT $in",
        "",
        "rule delink",
        "  command = rm -rf build/delinks && $dsd delink -c $cfg > /dev/null && touch $out",
        "  description = DELINK",
        "",
        "rule lcf",
        "  command = $dsd lcf -c $cfg > /dev/null",
        "  description = LCF",
        "",
        # dsd writes objects.txt with quoted paths; mwldarm wants them bare.
        "rule link",
        "  command = tr -d '\"' < build/objects.txt > $out.rsp && $ld $ldflags -o $out build/arm9.lcf @$out.rsp",
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
        "build build.ninja: configure tools/configure.py tools.lock",
        "",
        "build extract/config.yaml: extract orig/AWRE.nds",
        "",
        f"build build/delink.stamp: delink {' '.join(config_inputs())} | extract/config.yaml $dsd",
        f"  cfg = {cfg}",
        "",
        "build build/arm9.lcf build/objects.txt: lcf build/delink.stamp | $dsd",
        f"  cfg = {cfg}",
        "",
        "build build/arm9.elf: link build/arm9.lcf build/objects.txt | $ld",
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
    (ROOT / "build.ninja").write_text("\n".join(lines))
    return 0


if __name__ == "__main__":
    sys.exit(main())
