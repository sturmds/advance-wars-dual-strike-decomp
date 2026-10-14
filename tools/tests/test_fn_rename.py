#!/usr/bin/env python3
"""Check how `fn.py rename` rewrites a data_inc.txt row, on small samples.

    python3 tools/tests/test_fn_rename.py

No build or ROM needed.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import fn  # noqa: E402

errors = []


def expect(name: str, text: str, old: str, new: str, want: str) -> None:
    got = fn.rename_data_inc(text, old, new)
    if got != want:
        errors.append(f"{name}: got {got!r}, want {want!r}")
    else:
        print(f"{name}: ok")


ROW = "ov001     data_ov001_02379290         0x02379290  0x8    {u8,u8,u16,void*}\n"
expect("row renamed, columns kept", ROW, "data_ov001_02379290", "Ov001_gNoTriggers",
       "ov001     Ov001_gNoTriggers           0x02379290  0x8    {u8,u8,u16,void*}\n")
expect("longer name pushes the columns", ROW, "data_ov001_02379290", "Ov001_gAVeryLongTriggerListName",
       "ov001     Ov001_gAVeryLongTriggerListName 0x02379290  0x8    {u8,u8,u16,void*}\n")
expect("other rows untouched", ROW, "data_ov001_0237929", "x", ROW)
expect("comment untouched", "# data_ov001_02379290 is empty\n", "data_ov001_02379290", "x",
       "# data_ov001_02379290 is empty\n")

if errors:
    print("\n".join(errors))
    sys.exit(1)
