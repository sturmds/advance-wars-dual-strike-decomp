#!/usr/bin/env python3
"""Boot ROMs in melonDS and watch the ARM9 through its GDB stub.

    melonds_check.py [--frames 3600] [--press FRAME:KEY,...] [--save FILE] [--jobs 4] ROM[=ELF] ...

Each ROM runs in its own melonDS (env MELONDS, default
melonDS/build/melonDS beside the repo's main checkout), with no window
(QT_QPA_PLATFORM=offscreen), no sound (volume 0, SDL_AUDIODRIVER=dummy) and
a config directory, home and save directory of its own under build/melonds/,
so the user's melonDS settings and home directory are neither read nor
written. The save starts empty unless --save gives one. The frame limiter
is off: a ROM runs about four times faster than a DS.

The tool connects to the ARM9 GDB stub (tools/gdb_rsp.py) before the first
instruction runs, then lets the game run for --frames VBlanks of emulated
time, counted by the VBlank counter the SDK keeps at 0x027ffc3c. With no
save the game formats one, plays its opening and shows the title screen at
frame 2697; the default --press holds START twice to open the main menu
(frame 2950). Keys are given by changing the keys argument of
Keypad_UpdateState. It reports:

  - exceptions: breakpoints on the ARM9 exception vectors at 0xffff0004
    (undefined instruction), 0xffff000c (prefetch abort) and 0xffff0010
    (data abort). The DS ARM9 uses the high vectors, in the BIOS. The
    faulting address comes from lr_und or lr_abt.
  - a hang: MainLoop_RunFrame (ITCM) runs once per game frame. If it does
    not run for 600 VBlanks (retail waits at most 167, while it formats the
    save), or the VBlank counter stops, the game is hung.
  - the heap start: when Mem_InitHeaps starts, the main arena's low end
    (OS arena 0, stored at 0x027ffda0) must lie at or past the end of every
    overlay in the ROM's overlay table (RAM address + size + .bss size).
    Otherwise loading an overlay overwrites the heap.
  - progress: the frame at which each function in MARKERS first runs (the
    heap set-up, the opening's first scene, the title screen, the main menu).

Symbol addresses come from ELF (an arm9.elf linked by the build), from the
arm9.elf next to the ROM, or from config/AWRE/arm9/ when the ROM is
retail. A ROM fails when it hits an exception, hangs, puts the heap inside
overlay memory, or misses a marker that the first ROM on the command line
reached.

The launch, config and symbol code is shared with tools/emu.py, in
tools/melonds.py. Facts about melonDS (config keys, exception entry):
docs/third-party/melonds.md.

Exit 0 when every ROM passes, 1 otherwise, 2 when melonDS could not be run.
"""

import argparse
import concurrent.futures
import os
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from gdb_rsp import GdbClient, ProtocolError  # noqa: E402
from melonds import (MELONDS, ROOT, SIGILL, VBLANK_COUNT, VECTORS, Symbols, describe_exception,  # noqa: E402
                     free_ports, overlay_ranges, parse_keys, parse_rom_arg, start_melonds, stop_melonds)

OUT = ROOT / "build/melonds"

ARENA_LO = 0x027FFDA0  # OS arena low ends, one word per arena; arena 0 is main RAM
HEARTBEAT = "MainLoop_RunFrame"
HANG_FRAMES = 600
MARKERS = ("Mem_InitHeaps", "Ov004Task_InitScene", "Ov004Task_RunPressStartScreen", "MainMenu_Setup")
KEYPAD = "Keypad_UpdateState"  # takes the keys held this frame in r1 (include/game/keypad.h bits)
HOLD = 4  # frames a scripted key stays down
# START on the title screen, twice: the first skips its animation, the second opens the
# main menu (MainMenu_Setup, at frame 2950 in retail).
PRESSES = "2760:start,2910:start"
POLL = 0.25  # seconds of wall time between stops


# One run

class Run:
    def __init__(self, rom: Path, elf: Path | None, index: int):
        self.rom = rom
        self.symbols = Symbols.for_rom(rom, elf)
        self.symbol_source = self.symbols.source
        self.work = OUT / f"{index}-{rom.stem}"
        self.problems: list[str] = []
        self.markers: dict[str, int] = {}
        self.frames = 0
        self.max_gap = 0
        self.heap = None
        # The highest end address (RAM address + size + .bss size) of an ARM9 overlay.
        self.overlays_end = max(end for _, end in overlay_ranges(rom).values())
        self.wall = 0.0

    def name(self, addr: int) -> str:
        return self.symbols.name(addr)

    def address(self, name: str) -> int:
        if name not in self.symbols.functions:
            raise SystemExit(f"error: {name} not in {self.symbol_source}")
        return self.symbols.functions[name]

    def run(self, frames: int, ports: tuple[int, int], save: Path | None,
            presses: list[tuple[int, int]]) -> "Run":
        start = time.monotonic()
        limit = int(frames / 60 + 120)
        proc = start_melonds(self.rom, self.work, ports[0], ports[1], limit, save)
        gdb = None
        try:
            try:
                gdb = GdbClient.connect(ports[0], timeout=20)
            except OSError:
                self.problems.append(f"melonDS did not open the GDB port {ports[0]}; see {self.work}/melonds.log")
                return self
            gdb.halt_reason()
            self.watch(gdb, frames, start + limit - 5, presses)
        except (ProtocolError, OSError, TimeoutError) as e:
            self.problems.append(f"GDB connection: {e}")
        finally:
            if gdb is not None:
                gdb.kill()
            stop_melonds(proc)
            self.wall = time.monotonic() - start
        return self

    def watch(self, gdb: GdbClient, frames: int, deadline: float, presses: list[tuple[int, int]]) -> None:
        for vector in VECTORS:
            gdb.set_breakpoint(vector)
        heartbeat = self.address(HEARTBEAT)
        markers = {self.address(m): m for m in MARKERS}
        for addr in markers:
            gdb.set_breakpoint(addr)
        keypad = self.address(KEYPAD)
        keypad_armed = False
        gdb.set_breakpoint(heartbeat)
        armed_at = 0  # frame at which the heartbeat breakpoint was set; None while it is not
        last_frames, last_change = 0, time.monotonic()
        while True:
            gdb.resume()
            signal_ = gdb.wait_stop(POLL)
            if signal_ is None:
                signal_ = gdb.interrupt()
            regs = gdb.registers()
            pc = regs["pc"]
            self.frames = gdb.read_u32(VBLANK_COUNT)
            # Scripted keys: stop in Keypad_UpdateState each frame until the last press ends.
            # Polls are about 100 frames apart, too far apart to set it just before a press.
            want = any(self.frames < start + HOLD for start, _ in presses)
            if want != keypad_armed:
                (gdb.set_breakpoint if want else gdb.clear_breakpoint)(keypad)
                keypad_armed = want
            if pc == keypad:
                keys = 0
                for start, mask in presses:
                    if start <= self.frames < start + HOLD:
                        keys |= mask
                if keys:
                    gdb.write_register("r1", regs["r1"] | keys)
            if armed_at is None:
                # Set again at each poll, not each frame: every stop costs round trips.
                gdb.set_breakpoint(heartbeat)
                armed_at = self.frames
            if signal_ == SIGILL or pc in VECTORS:
                self.exception(pc, regs, signal_)
                return
            if pc == heartbeat and armed_at is not None:
                gdb.clear_breakpoint(heartbeat)
                self.max_gap = max(self.max_gap, self.frames - armed_at)
                armed_at = None
            if pc in markers and markers[pc] not in self.markers:
                self.markers[markers[pc]] = self.frames
                gdb.clear_breakpoint(pc)
                if markers[pc] == "Mem_InitHeaps":
                    self.heap = gdb.read_u32(ARENA_LO)
                    if self.heap < self.overlays_end:
                        self.problems.append(
                            f"heap starts at {self.heap:08x}, inside overlay memory (overlays end at "
                            f"{self.overlays_end:08x})")
            if armed_at is not None and self.frames - armed_at > HANG_FRAMES:
                self.problems.append(f"hang: {HEARTBEAT} did not run for {HANG_FRAMES} frames "
                                     f"(frame {self.frames}, pc {self.name(pc)})")
                return
            now = time.monotonic()
            if self.frames != last_frames:
                last_frames, last_change = self.frames, now
            elif now - last_change > 10:
                self.problems.append(f"hang: the VBlank counter stopped at {self.frames} (pc {self.name(pc)})")
                return
            if self.frames >= frames:
                return
            if now > deadline:
                self.problems.append(f"timeout: only {self.frames} frames before the time limit")
                return

    def exception(self, pc: int, regs: dict[str, int], signal_: int) -> None:
        kind, where = describe_exception(pc, regs, signal_)
        self.problems.append(f"{kind} at {where:08x} ({self.name(where)}), frame {self.frames}")


def parse_presses(text: str) -> list[tuple[int, int]]:
    presses = []
    for item in filter(None, text.split(",")):
        frame, _, keys = item.partition(":")
        try:
            presses.append((int(frame, 0), parse_keys(keys)))
        except ValueError as e:
            raise SystemExit(f"error: {e} in --press") from None
    return presses


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--frames", type=int, default=3600, help="VBlanks of emulated time per ROM (60 per second)")
    ap.add_argument("--jobs", type=int, default=4, help="ROMs run at the same time")
    ap.add_argument("--save", type=Path, help="start each ROM from a copy of this save file instead of none")
    ap.add_argument("--press", default=PRESSES, metavar="FRAME:KEY[+KEY],...",
                    help="keys to hold for 4 frames from the given VBlank counts, e.g. 2760:start,2900:a")
    ap.add_argument("roms", nargs="+", metavar="ROM[=ELF]")
    args = ap.parse_args()
    if not os.access(MELONDS, os.X_OK):
        print(f"error: melonDS not found at {MELONDS} (set MELONDS; docs/workflow.md)", file=sys.stderr)
        return 2
    presses = parse_presses(args.press)
    runs = []
    for i, arg in enumerate(args.roms):
        rom, elf = parse_rom_arg(arg)
        if not rom.exists():
            print(f"error: {rom} not found", file=sys.stderr)
            return 2
        runs.append(Run(rom, elf, i))
    ports = free_ports(2 * len(runs))
    with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, args.jobs)) as pool:
        list(pool.map(lambda i: runs[i].run(args.frames, (ports[2 * i], ports[2 * i + 1]), args.save, presses), range(len(runs))))

    reference = runs[0]
    ok = True
    for r in runs:
        for marker in reference.markers:
            if marker not in r.markers and not r.problems:
                r.problems.append(f"did not reach {marker} (the first ROM did at frame {reference.markers[marker]})")
        ok &= not r.problems
        print(f"{r.rom}: {'FAIL' if r.problems else 'ok'}, {r.frames} frames in {r.wall:.1f} s "
              f"(symbols: {r.symbol_source})")
        if r.heap is not None:
            print(f"    heap start {r.heap:08x}, overlays end {r.overlays_end:08x}")
        for marker in MARKERS:
            print(f"    {marker}: {'frame ' + str(r.markers[marker]) if marker in r.markers else 'not reached'}")
        print(f"    longest wait for {HEARTBEAT}: {r.max_gap} frames")
        for problem in r.problems:
            print(f"    {problem}")
    print("melonds: all ROMs passed" if ok else "melonds: problems found")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
