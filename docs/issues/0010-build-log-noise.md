# 0010: dsd prints `[INFO]` lines on every build

- **Status:** open
- **Area:** build
- **Opened:** 2026-10-07
- **Milestone:** M1

## Problem

Each `make` prints dsd's `[INFO ] Loading ROM from ...` lines for the delink, lcf, rom config and rom build steps. Together with issue 0001 this hides real warnings.

## Notes

dsd writes its log to stderr. Options: check whether dsd honours a log level (`RUST_LOG` or a flag) and set it in `build.ninja`; otherwise filter `[INFO ]` lines in the ninja rules while keeping the exit status (`set -o pipefail` is not available in plain `sh`, so use a small wrapper).

## Done when

A no-change `make` prints only ninja's status, and a real dsd warning still shows.
