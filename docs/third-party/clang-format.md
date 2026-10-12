# clang-format and the Google style

- **Project:** clang-format, the C and C++ formatter from LLVM, as the `clang-format` wheel on PyPI (a static Linux binary in a Python package).
- **Authors:** the LLVM project; the wheel is packaged by ssciwr (https://github.com/ssciwr/clang-format-wheel).
- **URL:** https://pypi.org/project/clang-format/
- **Version we use:** 23.1.1 (2026-09-10), pinned in `tools.lock` with its SHA-256. `make setup` unpacks it into `.cache/tools/clang-format/`. We picked a release a month old rather than the newest.
- **License:** Apache-2.0 with LLVM exception.

## The style

`.clang-format` starts from `BasedOnStyle: Google`, the Google C++ style built into clang-format (https://google.github.io/styleguide/cppguide.html). clang-format formats C with its C++ rules; options for namespaces, templates and classes never apply.

On top of it we take four settings from kehanXue/google-style-clang-format (https://github.com/kehanXue/google-style-clang-format): no single-line `if` or loops, no comment reflow, blank lines kept at the start of blocks, includes kept in their blocks. That repo is GPL-3.0; we copy none of its file, only the choice of those options. We add two of our own: includes are never sorted (header order matters here), and every file uses `T* p` instead of each file's majority.

## What we do not take

No code. `tools/format.py` runs the binary unchanged.
