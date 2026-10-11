# khdays-decomp

- **Project:** khdays-decomp, a matching decompilation of *Kingdom Hearts 358/2 Days* (DS).
- **Author:** Yokimitsuro, the only commit author in the repo. Neither its README nor its LICENSE names authors.
- **URL:** https://github.com/Yokimitsuro/khdays-decomp (the `origin` remote of our local copy; the README does not record a URL for the project itself).
- **Version we read:** commit `ba2de37039572431cffda7d1b80e3072519fe6bd` (2026-10-05), local copy in `~/repos/khdays-decomp`.
- **License:** CC0 1.0 Universal, a public domain dedication (`LICENSE` in that repo). sturmds approved reuse with credit.

A different game on the same DS toolchain (dsd, Metrowerks compiler). We read its tools to see how it matches many similar functions at once.

## What we take

Ideas and parts of the code of these tools, rewritten for our dsd and objdiff pipeline:

- `dedupprop.py`, `findtwin.py`, `genfamily.py`: relocation-masked twin families, and copying C from a matched function to its twins. Used in `tools/propagate.py`.
- `nearfam.py`, `port_template.py`: families of same-shape functions, and the ways to render differing immediates. Used in `tools/propagate.py`.
- `auto_trivial.py`: patterns for functions of one to four instructions. Used in `tools/trivial.py`.
- `declperm.py`, `park_permute.py`, `allopt.py`, `allcc.py`: sweeps over declaration order and compiler flags. Used in `tools/permute.py`.
- `pick_family.py`: ranking of family heads and its filters. Used in `tools/fn.py pick --families`.
- `configure.py` (`ABSOLUTE_SYMBOLS`, `add_absolute_symbols`): linker-absolute symbols added to the generated linker script. Used in `tools/lcf_absolute.py`.
- The family index and the check that uses it (`fnindex.py`, `tools/fn.py check`) follow the same twin-family idea.

## What we do not take

- Their C sources, headers, config or names. They are for a different game.
- Their compiler set-up, which uses a patched licence file. We use metroskrew from `tools.lock`.

## How ported tools credit it

Each ported tool has this comment after its usage text, naming the tools it was based on:

```python
# Based on khdays-decomp tools/port_template.py and tools/nearfam.py
# (CC0 1.0, commit ba2de370; see docs/third-party/khdays-decomp.md).
```

Commits that add or change these tools end with the line `Based on khdays-decomp (CC0): <tool names>`. Generated C files need no credit line, because they are copied from our own templates.
