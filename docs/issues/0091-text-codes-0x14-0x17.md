# 0091: Text codes 0x14 to 0x17 have no names

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-15
- **Milestone:** later

## Problem

`tools/text.py` writes most control codes of the game's strings by name (`{wait}`, `{color:1}`), but 0x14, 0x15, 0x16 and 0x17 only as `{0x14}` and so on, since what they do is not known (`docs/text.md`, "Encoding").

## Notes

- `TextPrinter_ReadControlCode`: 0x14, 0x16 and 0x17 start the script `data_02158520`, set its `textEntry`, `unk_28`, `unk_2c` and `unk_30`, set `unk_1e` to 1 for 0x17 and 0 for the others, and move the cursor 8 tiles right. 0x15 is drawn as a glyph (7 pixels wide in `Ov000_gGlyphWidths`).
- In the US text, 0x16 (4 strings) and 0x17 (17 strings) end yes/no questions ("Are you sure you want to quit?" with 0x16, "Save current data?" with 0x17), so `data_02158520` likely draws the Yes and No choices, and `unk_1e` picks the choice the cursor starts on. One 0x16 ends a line of campaign dialogue (`0x26000063`). 0x14 and 0x15 are not used.

## Done when

The script's functions are read, the codes have names in `text.py` (`SIMPLE`) and `docs/text.md`, and `text.py check` still passes.
