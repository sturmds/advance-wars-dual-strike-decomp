# decomp-permuter

- **Project:** decomp-permuter, a tool that applies random rewrites to a C function and keeps the variants whose compiled code is closer to a target object.
- **Authors:** Simon Lindholm and contributors (`LICENSE`).
- **URL:** https://github.com/simonlindholm/decomp-permuter
- **Version we use:** commit `059609d4aec73eb0650726772954e1ad575825f8` (2026-09-07), pinned in `tools.lock` as a source archive with its SHA-256. `make setup` unpacks it into `.cache/tools/decomp-permuter/`. We picked a commit a month old rather than the newest.
- **License:** MIT (`LICENSE` in the archive). It includes a patched copy of pycparser (BSD), taken from m2c, under `perm_pycparser/`.

## How we use it

`tools/permuter.py` runs it unchanged, as a program, on one function at a time (issue 0052). The wrapper writes the input folder decomp-permuter expects (preprocessed C, the original object from dsd, a compile script with our flags), and gives it an objdump command that rewrites relocations to addresses, so the original's and our symbol names compare equal. Its variants are rescored with `fn.py check` before anyone trusts them.

- It needs the `toml` package; the wrapper puts a shim over Python's `tomllib` on its path instead of installing anything.
- It needs `arm-none-eabi-objdump` (the `binutils-arm-none-eabi` package).
- We do not use permuter@home (its network mode), its `import.py`, or its Docker worker.

## What we do not take

None of its code is copied into this repo. Variants it finds are our C, written back into our files by hand.
