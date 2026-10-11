#!/usr/bin/env python3
"""Start ROMs in DeSmuME without a window and compare what can be seen.

    emu_smoke.py [--seconds 20] ROM...

For each ROM it runs /usr/games/desmume-cli under `timeout` with SDL's
offscreen video driver and dummy audio, so no window opens and no sound
plays, and with HOME pointing at a new, empty directory under build/emu/, so
DeSmuME's settings and save files stay out of the real home directory. No
input is given and no pixels are read. It reports per ROM:

  - the exit status (124 means DeSmuME was still running when `timeout`
    stopped it, the expected result);
  - the log lines DeSmuME printed, minus the ones that are the same for
    every ROM;
  - the save file the game wrote (DeSmuME's .dsv: the game's backup memory
    plus a footer): size and SHA-1 of the game's part.

The game formats its save on first boot, so a matching save hash says the
game ran its start-up code the same way up to that point. This is only a
smoke test: a game stuck in a loop or drawing garbage keeps running and may
write the same save. Real checks need scripted input and RAM or screen
comparisons (issue 0006).

Exit 0 when every ROM was still running at the timeout and all saves match
the first ROM's; 1 otherwise.
"""

import argparse
import hashlib
import os
import shutil
import subprocess
import sys
from pathlib import Path

DESMUME = "/usr/games/desmume-cli"
ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "build/emu"
DSV_FOOTER = b"|<--Snip above here to create a raw sav by excluding this DeSmuME savedata footer:"


def run_one(rom: Path, seconds: int, index: int) -> dict:
    home = OUT / f"{index}-{rom.stem}"
    if home.exists():
        shutil.rmtree(home)
    home.mkdir(parents=True)
    env = {
        "PATH": "/usr/bin:/bin",
        "HOME": str(home),
        "XDG_CONFIG_HOME": str(home / ".config"),
        "SDL_VIDEODRIVER": "offscreen",
        "SDL_AUDIODRIVER": "dummy",
    }
    cmd = ["timeout", str(seconds), DESMUME, "--disable-sound", "--nojoy", "--num-cores", "1",
           str(rom.resolve())]
    proc = subprocess.run(cmd, env=env, cwd=home, capture_output=True, text=True, errors="replace")
    saves = sorted(home.rglob("*.dsv"))
    save = saves[0].read_bytes() if saves else b""
    game = save.split(DSV_FOOTER)[0] if save else b""
    return {
        "rom": rom,
        "status": proc.returncode,
        "log": [line for line in (proc.stdout + proc.stderr).splitlines() if line.strip()],
        "save_size": len(game),
        "save_sha1": hashlib.sha1(game).hexdigest() if game else "none",
    }


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--seconds", type=int, default=20)
    ap.add_argument("roms", nargs="+", type=Path)
    args = ap.parse_args()
    if not os.access(DESMUME, os.X_OK):
        raise SystemExit(f"error: {DESMUME} not found (issue 0006 lists the emulator options)")
    results = [run_one(rom, args.seconds, i) for i, rom in enumerate(args.roms)]
    common = set(results[0]["log"]).intersection(*(r["log"] for r in results[1:]))
    ok = True
    for r in results:
        same = r["save_sha1"] == results[0]["save_sha1"]
        ok &= r["status"] == 124 and same
        print(f"{r['rom']}: exit {r['status']}, save {r['save_size']} bytes sha1 {r['save_sha1']}"
              f"{'' if same else ' (differs from the first ROM)'}")
        for line in r["log"]:
            if line not in common:
                print(f"    {line}")
    print(f"log lines shared by all: {len(common)}")
    print("emu: same behaviour seen" if ok else "emu: differences seen")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
