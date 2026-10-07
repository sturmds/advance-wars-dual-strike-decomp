#!/usr/bin/env python3
"""Generate config/AWRE/arm9/** with `dsd init`. Run once; the output is committed.

dsd 0.12.1 aborts on AWRE: after the end of main's .text (0x02129760) its
function search keeps going for up to 0x2000 bytes into .rodata, decodes
graphics data as Thumb code, and stops at a call that leads nowhere. To get
past that, this script runs `dsd init` on a temporary copy of the extract
with 0x2100 bytes zeroed at 0x02129760. Then it points the config at the
real extract and writes main's real code hash.

Side effect: dsd sees no pointers in .rodata 0x02129760-0x0212b860, so the
config has no relocations there. The bytes still match because they are
copied as they are; it only matters once code has to move (plan M3).

Usage: dsd_init.py [--force]
"""

import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from fxhash import fxhash64  # noqa: E402

DSD = ROOT / ".cache/tools/dsd/dsd"
EXTRACT = ROOT / "extract"
CONFIG = ROOT / "config/AWRE"
ARM9_BASE = 0x02000000
ZERO_START = 0x02129760
ZERO_SIZE = 0x2100


def main() -> int:
    out = CONFIG / "arm9"
    if out.exists() and "--force" not in sys.argv:
        sys.exit(f"error: {out.relative_to(ROOT)} exists. Pass --force to regenerate it, "
                 "then review `git diff`.")
    if not (EXTRACT / "config.yaml").exists():
        sys.exit("error: extract/ is missing. Run `make extract` first.")

    (ROOT / "build").mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(dir=ROOT / "build") as tmp:
        tmp = Path(tmp)
        patched = tmp / "extract"
        shutil.copytree(EXTRACT, patched, ignore=shutil.ignore_patterns("files"))
        os.symlink(EXTRACT / "files", patched / "files")
        arm9 = bytearray((patched / "arm9/arm9.bin").read_bytes())
        start = ZERO_START - ARM9_BASE
        arm9[start:start + ZERO_SIZE] = bytes(ZERO_SIZE)
        (patched / "arm9/arm9.bin").write_bytes(arm9)

        subprocess.run([DSD, "init", "-r", patched.relative_to(ROOT) / "config.yaml",
                        "-o", tmp.relative_to(ROOT), "-b", "build"], check=True, cwd=ROOT)

        cfg_path = tmp / "arm9/config.yaml"
        cfg = cfg_path.read_text()
        # dsd writes paths relative to tmp/arm9. Rewrite them for their final
        # place, and point rom_config at the real extract.
        def moved(m):
            target = (tmp / "arm9" / m.group(2)).resolve()
            return m.group(1) + os.path.relpath(target, out)
        cfg = re.sub(r"^(\s*(?:build_path|delinks_path|object): )(\S+)$", moved, cfg, flags=re.M)
        cfg = re.sub(r"^rom_config: .*$", "rom_config: ../../../extract/config.yaml", cfg, flags=re.M)
        main_hash = "%016x" % fxhash64((EXTRACT / "arm9/arm9.bin").read_bytes())
        cfg, n = re.subn(r"(main_module:\n(?:  .*\n)*?  hash: )[0-9a-f]+", r"\g<1>" + main_hash, cfg)
        if n != 1:
            sys.exit("error: could not find main_module hash in the generated config.yaml")
        cfg_path.write_text(cfg)

        if out.exists():
            shutil.rmtree(out)
        CONFIG.mkdir(parents=True, exist_ok=True)
        shutil.move(tmp / "arm9", out)

    print(f"dsd_init: wrote {out.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
