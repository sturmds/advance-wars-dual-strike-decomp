# decomp.dev

- **Project:** decomp.dev, a website that tracks the progress of decompilation projects from the objdiff reports their GitHub Actions runs upload.
- **Authors:** Luke Street and contributors.
- **URL:** https://github.com/encounter/decomp.dev
- **Version read:** commit `e9c086adb74d2fe569541cd715cd9312d7641313` (2026-09-06).
- **License:** none stated in the repository, so we take facts only.

## What we take

Facts read from its source, used by `tools/decomp_dev_report.py`:

- It reads artifacts whose name matches `^(?P<version>[A-z0-9_.\-]+)[_-]report(?:[_-].*)?$` (`crates/github/src/lib.rs`), from completed `push` runs on the repository's default branch.
- Its project list shows `matched_code_percent` as decompiled and `complete_code_percent` as fully linked (`crates/web/src/handlers/project.rs`).

## What we do not take

No code. We do not upload anything to it; publishing is issue 0071.
