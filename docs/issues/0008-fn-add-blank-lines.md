# 0008: `fn.py add` leaves an extra blank line in `delinks.txt`

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-07
- **Milestone:** M1

## Problem

An agent working in overlay 3 found two blank lines before an existing file block after `fn.py add`, and removed one by hand. The stray edit then shows up in its diff.

## Notes

`cmd_add` in `tools/fn.py` splits the file on blank lines and rejoins blocks with one blank line. The header part keeps whatever trailing newlines it had. Normalise the header and blocks with `strip("\n")` before joining.

## Done when

`fn.py add` followed by `git diff` shows only the new block.
