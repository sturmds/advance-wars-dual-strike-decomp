# 0001: mwldarm warns about `.exceptix` for every C function

- **Status:** open
- **Area:** build
- **Opened:** 2026-10-07
- **Milestone:** M1

## Problem

Every file compiled from `src/` adds two warnings to the link:

```
mwldarm-2.0-72.exe: warning: @EX@func_02002620(.exceptix) in file func_02002620.o is referenced but has not been written.
mwldarm-2.0-72.exe: warning: Check your linker command file.
```

The ROM still matches, but the warnings grow with each function (16 functions → 32 lines today) and will hide real linker messages.

## Notes

- mwccarm emits exception-table data (`.exceptix`) for C functions. dsd's LCF has `KEEP_SECTION { .exceptix }` but no output section that places it, so the linker drops it and warns.
- In the retail ROM only the MSL/runtime region has exception table entries (28 entries at 0x02129768, see `docs/awre-facts.md`). Game and SDK code have none, which suggests they were built with exception tables off.
- First thing to try: add `-Cpp_exceptions off` to `CFLAGS` in `tools/configure.py` (the EU repo's notes list that flag). Check that all matched functions still match and the warnings stop.
- If the flag does not help: place `.exceptix` in the LCF, or filter the warning in the link rule (last resort).

## Done when

`make` prints no `.exceptix` warnings and `make release` still matches.
