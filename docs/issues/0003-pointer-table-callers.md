# 0003: `fn.py show` finds no callers for functions reached through pointer tables

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-07
- **Milestone:** M1

## Problem

`fn.py show` only counts direct calls (`arm_call`, `thumb_call`, ...) as callers. Functions such as func_02003f44, func_02003e84 and func_0201d320 print "callers: none found", even though something must call them. They are most likely reached through a table of function pointers, which `relocs.txt` records as `kind:load` from a data address.

## Notes

- Fix: in `show`, also list `load` relocations that point at the function, and name the data symbol that holds them ("in table data_0215xxxx at +0x10").
- The table's other entries are often related handlers, which helps naming.

- 2026-10-08: the same applies to the CO table. Its blocks hold pointers to 32 routines (CO Powers, unit filters, unit bonuses, calculation bonuses), and `fn.py show` lists no callers for most of them. A general fix: list every relocation that points at the function, code or data, with the symbol and offset it sits in ("referenced from gCoTable+0xd4"). The data relocations are already in `relocs.txt`.

## Done when

`fn.py show func_0201d320` names the table (or tables) that point at it.
