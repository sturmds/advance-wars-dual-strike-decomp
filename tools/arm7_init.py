#!/usr/bin/env python3
"""Generate config/AWRE/arm7/ with `dsd init`. Run once; the output is committed.

dsd cannot analyse ARM7 at its real addresses (tools/arm7.py says why), so
this script builds a stand-in ROM the way `arm7.py prepare` does, but
without a config to say where the relocations are. It moves every word
that looks like a pointer into ARM7, and every ARM or Thumb branch that
crosses from one ARM7 part to another, to the moved addresses. Then it runs
`dsd init` on that ROM and moves the result back:

- addresses go back to their real values;
- dsd's names func_XXXXXXXX and data_XXXXXXXX become func_arm7_XXXXXXXX
  and data_arm7_XXXXXXXX with the real address, so they cannot clash with
  ARM9 names (ARM9's DTCM is also at 0x027c0000);
- relocations into ARM7 get `module:arm7`.

Then it fixes what dsd's search gets wrong on ARM7:

1. A "function" whose words are all pointers to ARM7 functions is a table:
   it becomes a data symbol.
2. Runs of Thumb functions (the BIOS call stubs) are split again, one
   function per `bx lr`; dsd starts functions only at multiples of 4.
3. A call target with no function, and a pointer into a gap in .text that
   starts with an unconditional ARM instruction, get a function up to the
   next symbol; .text grows to cover them. dsd stops searching at the first
   data it meets in a part, so it misses code after it.
4. A word that was moved above but got no relocation gets one when it points
   at a function start (with the Thumb bit for a Thumb function) or at a
   place outside every function inside a section; the target gets a data
   symbol if it has none. Others are constants, or pointers to the end of a
   section, and keep their bytes.

The words still without a relocation are listed at the end. `arm7.py
prepare` moves only words with a relocation, so those keep their original
bytes, and `make check` shows the build still matches.

Usage: arm7_init.py [--force]
"""

import re
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import arm7  # noqa: E402

DSD = ROOT / ".cache/tools/dsd/dsd"
CONFIG = arm7.CONFIG
FUNC = re.compile(r"^(\S+) kind:function\((arm|thumb),size=(0x[0-9a-f]+)\) addr:(0x[0-9a-f]+)$")
BX_LR = 0x4770


def moved_by_guess(data: bytearray, r: arm7.Region, regs: list[arm7.Region]) -> set[int]:
    """Move pointer-like words and cross-part branches in one part. Returns
    the addresses changed."""
    changed = set()
    for off in range(0, len(data) - 3, 4):
        word = struct.unpack_from("<I", data, off)[0]
        pc = r.base + off
        tr = arm7.region_of(regs, word & ~1)
        if tr and tr.fake != tr.base:
            struct.pack_into("<I", data, off, word + tr.fake - tr.base)
            changed.add(pc)
        elif (word >> 25) & 7 == 5:
            kind = "arm_call_thumb" if word >> 28 == 0xf else "arm_call"
            target = arm7.branch_target(word, pc, kind)
            tr = arm7.region_of(regs, target)
            if tr and tr is not r and move_branch(data, off, word, pc, target, kind, regs):
                changed.add(pc)
    for off in range(0, len(data) - 3, 2):
        hi, lo = struct.unpack_from("<2H", data, off)
        if hi & 0xf800 != 0xf000 or lo & 0xe800 != 0xe800:
            continue
        kind = "thumb_call" if lo & 0xf800 == 0xf800 else "thumb_call_arm"
        pc = r.base + off
        word = hi | lo << 16
        target = arm7.branch_target(word, pc, kind)
        tr = arm7.region_of(regs, target)
        if tr and tr is not r and move_branch(data, off, word, pc, target, kind, regs):
            changed.add(pc)
    return changed


def move_branch(data, off, word, pc, target, kind, regs) -> bool:
    """Rewrite one branch for the moved parts. Data that only looks like a
    branch can end up out of range; it stays as it is."""
    try:
        moved = arm7.encode_branch(word, arm7.to_fake(regs, pc), arm7.to_fake(regs, target), kind)
    except ValueError:
        return False
    struct.pack_into("<I", data, off, moved)
    return True


def rename(line: str, regs: list[arm7.Region]) -> str:
    def real(m):
        addr = arm7.to_real(regs, int(m.group(2), 16))
        return f"{m.group(1)}{'arm7_' if m.group(1) != '.L_' else ''}{addr:08x}"
    return re.sub(r"^(func_|data_|\.L_)([0-9a-f]{8})\b", real, line)


class Image:
    """The original ARM7 bytes by address."""

    def __init__(self, data: bytes, regs: list[arm7.Region]):
        self.data, self.regs = data, regs

    def read(self, addr: int, size: int) -> int:
        r = arm7.region_of(self.regs, addr)
        off = r.offset + addr - r.base
        if r is None or addr + size > r.base + r.size:
            return -1
        return int.from_bytes(self.data[off:off + size], "little")


class Analysis:
    """dsd's result, moved back to real addresses, plus the fixes above."""

    def __init__(self, sections, symbols, relocs, image: Image):
        self.image, self.regs = image, image.regs
        self.sections = sections          # delinks.txt header lines
        self.funcs = {}                   # addr -> (mode, size)
        self.other = {}                   # addr -> symbol line (data, bss, labels)
        for ln in symbols:
            m = FUNC.match(ln)
            if m:
                self.funcs[int(m.group(4), 16)] = (m.group(2), int(m.group(3), 16))
                if m.group(1) != f"func_arm7_{int(m.group(4), 16):08x}":
                    sys.exit(f"arm7_init: unexpected function name {m.group(1)}")
            else:
                self.other[arm7.first_addr(ln)] = ln
        self.relocs = {arm7.reloc_fields(ln)[0]: ln for ln in relocs}
        self.new = set()           # functions added after dsd

    def func_at(self, addr: int):
        for start, (mode, size) in self.funcs.items():
            if start <= addr < start + size:
                return start
        return None

    def section_of(self, addr: int):
        for ln in self.sections:
            m = arm7.SECTION.match(ln)
            start, end = int(m.group(2), 16), int(m.group(3), 16)
            if start <= addr < end:
                return m.group(1)
        return None

    def next_symbol(self, addr: int) -> int:
        later = [a for a in list(self.funcs) + list(self.other) if a > addr]
        r = arm7.region_of(self.regs, addr)
        return min(later + [r.base + r.size])

    def add_func(self, addr: int, mode: str) -> None:
        self.funcs[addr] = (mode, self.next_symbol(addr) - addr)
        self.new.add(addr)

    def trim_added(self) -> None:
        """An added function ends at the next symbol, which may have been
        added after it."""
        for addr in self.new:
            mode, size = self.funcs[addr]
            self.funcs[addr] = (mode, min(size, self.next_symbol(addr) - addr))

    # 1
    def tables_to_data(self) -> None:
        for addr, (mode, size) in list(self.funcs.items()):
            words = [self.image.read(a, 4) for a in range(addr, addr + size - 3, 4)]
            if len(words) >= 2 and all((w & ~1) in self.funcs and w != (w & ~1) + 2 for w in words):
                del self.funcs[addr]
                self.other[addr] = f"data_arm7_{addr:08x} kind:data(any) addr:0x{addr:08x}"

    # 2
    def split_thumb_runs(self) -> None:
        starts = sorted(a for a, (mode, _) in self.funcs.items() if mode == "thumb")
        runs, run = [], []
        for a in starts:
            if run and a > run[-1] + self.funcs[run[-1]][1] + 2:
                runs.append(run)
                run = []
            run.append(a)
        if run:
            runs.append(run)
        for run in runs:
            first, last = run[0], run[-1] + self.funcs[run[-1]][1]
            split, pos = [], first
            while pos < last:
                end, pool_end, q = None, pos, pos
                while q < last:
                    hw = self.image.read(q, 2)
                    if hw & 0xf800 == 0x4800:  # ldr rN, [pc, #imm]
                        pool_end = max(pool_end, ((q + 4) & ~3) + (hw & 0xff) * 4 + 4)
                    if hw == BX_LR:
                        end = q + 2
                        break
                    q += 2
                if end is None:
                    break
                end = max(end, pool_end)
                split.append((pos, end - pos))
                pos = end
            if pos != last:
                continue  # not a run of stubs; leave dsd's split
            for a in run:
                del self.funcs[a]
            for a, size in split:
                self.funcs[a] = ("thumb", size)

    # 3
    def add_missed_functions(self, guessed: set[int]) -> None:
        for frm, ln in list(self.relocs.items()):
            _, kind, to = arm7.reloc_fields(ln)
            if to is None or arm7.region_of(self.regs, to) is None:
                continue
            if "module:none" in ln:
                if to not in self.funcs:
                    self.add_func(to, "thumb" if kind in ("arm_call_thumb", "thumb_call") else "arm")
                self.relocs[frm] = ln.replace("module:none", "module:arm7")
        for frm in sorted(guessed - set(self.relocs)):
            value = self.image.read(frm, 4)
            if (value & 3 == 0 and arm7.region_of(self.regs, value) and value not in self.funcs
                    and self.func_at(value) is None and value not in self.other
                    and self.section_of(value) == ".text" and self.image.read(value, 4) >> 28 == 0xe):
                self.add_func(value, "arm")
        self.trim_added()
        self.stretch_text()

    def stretch_text(self) -> None:
        """Move the end of .text past the last function. The link script starts
        .data 32-byte aligned, so when .text now ends off that alignment and
        the part has no .rodata, the bytes up to the next multiple of 32
        become .rodata."""
        for region in self.regs:
            ends = [arm7.align(a + size, 4) for a, (_, size) in self.funcs.items()
                    if arm7.region_of(self.regs, a) is region]
            if not ends:
                continue
            end = max(ends)
            names = {arm7.SECTION.match(ln).group(1): j for j, ln in enumerate(self.sections)
                     if arm7.region_of(self.regs, arm7.first_addr(ln)) is region}
            ln = self.sections[names[".text"]]
            s_end = int(arm7.SECTION.match(ln).group(3), 16)
            if end <= s_end:
                continue
            self.sections[names[".text"]] = ln.replace(f"end:0x{s_end:08x}", f"end:0x{end:08x}")
            boundary = arm7.align(end, 32)
            if boundary != end and ".rodata" not in names:
                self.sections.append(f"    .rodata     start:0x{end:08x} end:0x{boundary:08x} kind:rodata align:4")
            else:
                boundary = end
            # The next section in this part now starts at the new end.
            for k, t in enumerate(self.sections):
                if f"start:0x{s_end:08x}" in t:
                    self.sections[k] = t.replace(f"start:0x{s_end:08x}", f"start:0x{boundary:08x}")

    # 4
    def add_pointer_relocs(self, guessed: set[int]) -> None:
        for frm in sorted(guessed - set(self.relocs)):
            value = self.image.read(frm, 4)
            target = value & ~1
            if arm7.region_of(self.regs, target) is None:
                continue
            if target in self.funcs:
                if (self.funcs[target][0] == "thumb") != bool(value & 1):
                    continue
            elif value & 1 or self.func_at(target) is not None:
                continue
            else:
                section = self.section_of(target)
                if section is None:
                    continue
                if target not in self.other:
                    kind = "bss" if section == ".bss" else "data(any)"
                    self.other[target] = f"data_arm7_{target:08x} kind:{kind} addr:0x{target:08x}"
            self.relocs[frm] = f"from:0x{frm:08x} kind:load to:0x{target:08x} module:arm7"

    def symbol_lines(self) -> list[str]:
        out = dict(self.other)
        for addr, (mode, size) in self.funcs.items():
            out[addr] = f"func_arm7_{addr:08x} kind:function({mode},size={size:#x}) addr:0x{addr:08x}"
        # Labels that now start a function are dropped.
        return [out[a] for a in sorted(out) if not (out[a].startswith(".L_") and a in self.funcs)]


def main() -> int:
    if CONFIG.exists() and "--force" not in sys.argv:
        sys.exit(f"error: {CONFIG.relative_to(ROOT)} exists. Pass --force to regenerate it, "
                 "then review `git diff`.")
    if not arm7.ARM7_BIN.exists():
        sys.exit("error: extract/ is missing. Run `make extract` first.")
    data = arm7.ARM7_BIN.read_bytes()
    regs = arm7.regions(data)
    parts = arm7.original_parts(data, regs)
    guessed = set()
    for r in regs:
        guessed |= moved_by_guess(parts[r.name], r, regs)

    (ROOT / "build").mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(dir=ROOT / "build") as tmp:
        tmp = Path(tmp)
        arm7.write_rom(tmp / "rom", parts, regs)
        subprocess.run([DSD, "init", "-r", (tmp / "rom/config.yaml").relative_to(ROOT),
                        "-o", (tmp / "out").relative_to(ROOT), "-b", (tmp / "build").relative_to(ROOT),
                        "--allow-unknown-function-calls"],
                       check=True, cwd=ROOT, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        sections, symbols, relocs = [], [], []
        back = lambda a: arm7.to_real(regs, a)  # noqa: E731
        for r in regs:
            d = tmp / f"out/arm9/autoload_{r.index}"
            sections += [arm7.move_fields(ln, back) for ln in (d / "delinks.txt").read_text().splitlines()
                         if ln.strip()]
            symbols += [rename(arm7.move_fields(ln, back), regs)
                        for ln in (d / "symbols.txt").read_text().splitlines() if ln.strip()]
            relocs += [re.sub(r"module:autoload\(\d\)", "module:arm7", arm7.move_fields(ln, back))
                       for ln in (d / "relocs.txt").read_text().splitlines() if ln.strip()]

    a = Analysis(sections, symbols, relocs, Image(data, regs))
    a.tables_to_data()
    a.split_thumb_runs()
    a.add_missed_functions(guessed)
    a.add_pointer_relocs(guessed)

    key = arm7.first_addr
    if CONFIG.exists():
        shutil.rmtree(CONFIG)
    CONFIG.mkdir(parents=True)
    (CONFIG / "delinks.txt").write_text("\n".join(sorted(a.sections, key=key)) + "\n\n")
    (CONFIG / "symbols.txt").write_text("\n".join(a.symbol_lines()) + "\n")
    (CONFIG / "relocs.txt").write_text("\n".join(a.relocs[f] for f in sorted(a.relocs)) + "\n")

    left = sorted(guessed - set(a.relocs))
    thumb = sum(m == "thumb" for m, _ in a.funcs.values())
    print(f"arm7_init: wrote {CONFIG.relative_to(ROOT)}: {len(a.funcs)} functions ({thumb} Thumb, "
          f"{len(a.new)} added after dsd), {len(a.relocs)} relocations")
    print(f"arm7_init: {len(left)} moved words have no relocation and keep their bytes:")
    for frm in left:
        print(f"  0x{frm:08x}: {a.image.read(frm, 4):08x}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
