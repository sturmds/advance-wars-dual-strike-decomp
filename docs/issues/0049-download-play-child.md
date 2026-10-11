# 0049: The DS Download Play child program has no C and is not built

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-11
- **Milestone:** later

## Problem

The cartridge's file system holds a second DS program, `files/boot/mb_combat_sgn.srl` (935,624 B). It is the DS Download Play child: the program the cartridge sends over wireless to a second DS, so two people can play with one cartridge. The build copies it from `extract/` as one file. None of its code is in C, and no tool delinks it. Code completion includes it.

## Notes

What its header says:

| Part | Size | Load address | Notes |
|---|---:|---|---|
| ARM9 | 758,776 B | 0x02000000 | entry 0x02000850, no overlays |
| ARM7 | 156,884 B | 0x02380000 | a different build from the cartridge's ARM7: 6,752 of 39,221 words match at the same offset |

- Game code `NTRJ`, maker `01`, title `NINTENDO`. It has a banner and no file system of its own (FNT 9 bytes, FAT empty).
- The last 0x88 bytes start with `ac 01 00`: the Download Play signature block, an RSA signature that a retail DS checks before it runs a downloaded child. The `_sgn` in the file name says the same.
- **How much is decompiled already.** 26.6% of the child's ARM9, in 32-byte windows at 4-byte steps, also appears byte for byte somewhere in the cartridge's ARM9, itcm or overlays. Relocated pointers differ between the two programs, so the shared code is likely larger than that. Those functions can come from our C through `propagate.py` once the child is delinked. Its ARM7 likely shares most functions with the cartridge's ARM7 (issue 0042), built from a different SDK release.

### Modding

The goal of the project is to mod the game (plan M3). Things to find out before deciding how much of the child has to be built from objects:

- **The signature.** A retail DS refuses a child whose signature does not match. A changed child would run in emulators that skip the check, and on hardware only with the check patched out. If so, mods may leave the child unchanged, and a matching child is enough.
- **What the child depends on.** If the child keeps its own copy of game rules (unit stats, damage tables) and the host sends only moves, a mod that changes rules on the host would make the two DSes disagree. If the host sends the rules, an unchanged child stays compatible. Answer this from the child's code.
- **Linking.** The cartridge build does not need the child linked: it embeds a file. The child can match as a separate program (its own extract, delink, link and hash) without blocking anything else, and the cartridge build would then embed the built `.srl` in place of the extracted one.

### Requirement: PROGRESS.md tracks the child

- The child gets its own rows, separate from the cartridge's ARM9 and ARM7: one for child ARM9 and one for child ARM7, in both the totals and the by-module tables.
- The totals table ends with a combined row for all four (cartridge ARM9, cartridge ARM7, child ARM9, child ARM7). The cartridge-only total from issue 0042 stays above it.
- Once the child is delinked, its functions count in the denominators even while none are in C.
- `tools/progress.py` and `build/report.json` follow the same split.

## Done when

The child is built from objects and matches. Its rows are in `PROGRESS.md`, and the modding questions above are answered here. Split the C work into its own issue at that point if it is large.
