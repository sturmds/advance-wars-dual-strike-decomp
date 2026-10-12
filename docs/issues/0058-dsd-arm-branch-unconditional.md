# 0058: dsd writes an `arm_branch` relocation in a delinked range as an unconditional `b`

- **Status:** worked around
- **Area:** tools
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

An `arm_branch` entry in `relocs.txt` for a conditional branch (`bmi`) is written as an unconditional `b` (condition 0xe instead of 0x4) when the branch lies in a range that is still delinked, that is, taken from the original bytes. Seen in b26-rtd0 with `_dadd` and `_dsub`: the high byte 0xea replaced 0x4a and the ROM did not match.

## Notes

- Workaround: add the `arm_branch` entry only once both ends are C. Written up in `docs/tool-issues.md` ("dsd: `arm_branch` in a delinked range loses the condition").
- Not reported upstream. The dsd source is in `~/repos/ds-decomp`.

## Done when

- dsd keeps the condition bits, or a check refuses the entry while one end is still delinked.
