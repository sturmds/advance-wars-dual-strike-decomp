#!/usr/bin/env python3
"""Narrow relocations that dsd marked `module:overlays(A,B,...)`.

Overlays that load at the same address share addresses, so when code points
there, dsd cannot tell which overlay it means and writes every candidate
(issue 0019). The delinked object then names a symbol of the first candidate.
That links the same bytes, but a build where one overlay grows or shrinks
would point at the wrong overlay. This script removes candidates on these
rules and writes `module:overlay(N)` when one is left:

  bss-only    The target lies in a candidate's .bss (zeroed at load, no bytes
              in the ROM), that overlay's own code never points at that
              address, and another candidate claims it: has code or data
              there, points at it from its own code, or starts its .bss
              there. Example: ov004's 0x94820-byte .bss covers all of ov000's
              .rodata, .data and .bss, but ov004 only ever points at the
              starts of its buffers.
  function    Exactly one remaining candidate has a function that starts at
              the target (found by dsd's code analysis, not by this
              relocation), and no other candidate's own code points there.
              Example: a table in main holds 0x02367eac 96 times, the start
              of a 0x14-byte function in ov005 and the middle of .data in
              ov001 and ov002.
  by hand     A decision listed in DECIDED below, with the reason.

Then it tidies symbols.txt: an overlay symbol marked `ambiguous` that no
relocation can still mean in another overlay loses the mark, and a
dsd-named one (`data_ovNNN_ADDR`) that nothing points at any more and no C
file names is removed: dsd made it only for the relocation.

Usage: resolve_overlay_relocs.py [--write]
Without --write it prints what it would change. Run `make release` after.
"""

import argparse
import re
import sys
from collections import Counter

import modmap

# (module, from address): (overlay, reason). For targets with code or data in
# more than one candidate.
DECIDED = {
    ("main", 0x020CE4F8): ("ov000", "UnloadTask_PickTile calls UnitPath_CountSteps"),
    ("main", 0x020CE534): ("ov000", "UnloadTask_PickTile calls UnitPath_CountSteps"),
    ("main", 0x020D0BD4): ("ov000", "UnitMoveSelect_TryMoveTo calls UnitPath_CountSteps"),
    ("main", 0x020D0C18): ("ov000", "UnitMoveSelect_TryMoveTo calls UnitPath_CountSteps"),
    ("main", 0x0216831C): ("ov002", "the scene task of AttackTask_StartScene, which the attack script"
                                    " runs after func_02001564 loads ov002: Battle_StartScreen"),
    ("main", 0x0216A6EC): ("ov005", "the script calls func_020015f4 (loads ov005) first"),
    ("main", 0x0216A7DC): ("ov005", "the script calls func_020015f4 (loads ov005) first"),
    ("main", 0x020DBD04): ("ov006", "every caller of Palette_QueueBlockCopy is in ov006, whose own code uses"
                                    " the address as a 0x100-byte .bss buffer; ov001 has a 20-byte object there"),
    ("ov000", 0x022E5288): ("ov001", "MapHeader.script: map scripts are ov001 .data (docs/maps.md)"),
    ("ov000", 0x022E5328): ("ov001", "MapHeader.script: map scripts are ov001 .data (docs/maps.md)"),
    ("ov000", 0x022E53C8): ("ov001", "MapHeader.script: map scripts are ov001 .data (docs/maps.md)"),
    ("ov000", 0x022E5468): ("ov001", "MapHeader.script: map scripts are ov001 .data (docs/maps.md)"),
    ("ov000", 0x022E5508): ("ov001", "MapHeader.script: map scripts are ov001 .data (docs/maps.md)"),
    ("ov000", 0x022E55A8): ("ov001", "MapHeader.script: map scripts are ov001 .data (docs/maps.md)"),
    ("ov000", 0x022E5648): ("ov001", "MapHeader.script: map scripts are ov001 .data (docs/maps.md)"),
    ("ov000", 0x022E56E8): ("ov001", "MapHeader.script: map scripts are ov001 .data (docs/maps.md)"),
    ("ov000", 0x022E5C88): ("ov001", "MapHeader.script: map scripts are ov001 .data (docs/maps.md)"),
}


def module_field(names: list[str]) -> str:
    ids = [str(int(n[2:])) for n in names]
    return f"overlay({ids[0]})" if len(ids) == 1 else f"overlays({','.join(ids)})"


def self_targets(mod: modmap.Module) -> set[int]:
    return {r.to for r in mod.relocs.values() if r.module == f"overlay({int(mod.name[2:])})"}


def narrow(mods, mod_name: str, r: modmap.Reloc, own: dict[str, set[int]],
           starts: dict[str, set[int]]) -> tuple[list[str], str]:
    cands = r.candidates()
    if (mod_name, r.source) in DECIDED:
        ov, why = DECIDED[(mod_name, r.source)]
        assert ov in cands, (mod_name, hex(r.source))
        return [ov], "by hand"
    def claims(c: str) -> bool:
        m = mods[c]
        bss = m.section_at(r.to)
        return m.is_initialised(r.to) or r.to in own[c] or (bss is not None and bss.start == r.to)

    if not any(claims(c) for c in cands):
        return cands, "kept"
    # A candidate whose range does not hold the target at all is kept: dsd
    # listed it for a reason this script does not know.
    keep = [c for c in cands if claims(c) or not mods[c].start <= r.to < mods[c].end]
    rule = "bss-only" if keep != cands else "kept"
    funcs = [c for c in keep if r.to in starts[c]]
    if len(keep) > 1 and len(funcs) == 1 and not any(r.to in own[c] for c in keep if c not in funcs):
        return funcs, "function"
    return keep, rule


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--write", action="store_true", help="rewrite relocs.txt and symbols.txt")
    args = ap.parse_args()
    mods = modmap.load()
    own = {n: self_targets(m) for n, m in mods.items() if n.startswith("ov")}
    starts = {n: {s.addr for s in m.symbols if s.kind.startswith("function") and not s.ambiguous}
              for n, m in mods.items()}

    stats = Counter()
    left = Counter()
    for mod in mods.values():
        path = mod.dir / "relocs.txt"
        lines = path.read_text().splitlines(keepends=True)
        changed = False
        for i, line in enumerate(lines):
            m = modmap.RELOC_RE.match(line.rstrip("\n"))
            r = mod.relocs[int(m.group(1), 16)]
            if not r.module.startswith("overlays("):
                continue
            keep, rule = narrow(mods, mod.name, r, own, starts)
            stats[(r.module, rule, len(keep) == 1)] += 1
            if keep != r.candidates():
                new = module_field(keep)
                lines[i] = line.replace(f"module:{r.module}", f"module:{new}")
                r.module = new
                changed = True
            assert keep, (mod.name, hex(r.source))
            if len(keep) > 1:
                left[(mod.name, r.module, mods[keep[0]].is_initialised(r.to))] += 1
        if changed and args.write:
            path.write_text("".join(lines))

    print("relocations by old module field, rule, and whether one overlay is left:")
    for (old, rule, single), n in sorted(stats.items()):
        print(f"  {n:5}  {old:24} {rule:9} {'decided' if single else 'still ambiguous'}")
    print("still ambiguous, by referencing module:")
    for (name, field, _), n in sorted(left.items()):
        print(f"  {n:5}  {name:6} {field}")

    # Symbols: which (overlay, address) pairs can a remaining relocation mean?
    meant = set()   # (overlay, addr) some relocation may point at
    shared = set()  # (overlay, addr) still pointed at ambiguously
    for mod in mods.values():
        for r in mod.relocs.values():
            for c in r.candidates():
                meant.add((c, r.to))
                if len(r.candidates()) > 1:
                    shared.add((c, r.to))
    src_text = "\n".join(p.read_text() for p in (modmap.ROOT / "src").rglob("*.c"))
    src_text += "\n".join(p.read_text() for p in (modmap.ROOT / "include").rglob("*.h"))
    unflagged = removed = 0
    for mod in mods.values():
        if not mod.name.startswith("ov"):
            continue
        path = mod.dir / "symbols.txt"
        out = []
        changed = False
        for line in path.read_text().splitlines(keepends=True):
            m = modmap.SYMBOL_RE.match(line.rstrip("\n"))
            if not m or " ambiguous" not in m.group(4):
                out.append(line)
                continue
            name, kind, addr = m.group(1), m.group(2), int(m.group(3), 16)
            if (mod.name, addr) in shared:
                out.append(line)
                continue
            changed = True
            auto = re.fullmatch(rf"data_{mod.name}_{addr:08x}", name)
            if (auto and (mod.name, addr) not in meant
                    and not re.search(rf"\b{name}\b", src_text)):
                removed += 1
                continue
            unflagged += 1
            out.append(line.replace(" ambiguous", "", 1))
        if changed and args.write:
            path.write_text("".join(out))
    print(f"symbols: {unflagged} lose `ambiguous`, {removed} unused symbols removed")
    if not args.write:
        print("dry run: pass --write to change the files")
    return 0


if __name__ == "__main__":
    sys.exit(main())
