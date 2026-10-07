# 0007: `dsd init` aborts on AWRE

- **Status:** worked around
- **Area:** tools
- **Opened:** 2026-10-06
- **Milestone:** later

## Problem

`dsd init` (0.12.0 and 0.12.1) stops with:

```
[ERROR] Local function call from 0x021347a6 in ARM9 main to 0x021341aa leads to no function
```

## Notes

Cause, workaround and its cost are in `docs/tool-issues.md`. Short version: `tools/dsd_init.py` runs `init` on a copy with 0x2100 bytes of `.rodata` zeroed. The 29 pointers lost that way (exception table) were added to `relocs.txt` by hand. Not reported upstream, by choice.

## Done when

A pinned dsd release runs `init` on AWRE without the workaround, or the config no longer needs regenerating.
