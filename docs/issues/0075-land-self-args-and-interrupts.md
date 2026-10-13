# 0075: `land_self.py --help` started a real landing, and a killed run left a replay open

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-13
- **Milestone:** M1

## Problem

- Two batch 41 agents ran `land_self.py --help` and got a real landing, cut off during the replay. Neither tool read its arguments except `--onto`, `--continue` and `--abort`.
- Another agent's first `land_self.py` run was killed partway (its output went through `head`) and left `rebase-onto-mainline.json` behind, which blocked the next run until `rebase_onto_mainline.py --abort`.

## Cause

`land_self.py` ignored `sys.argv` and went straight to git and the locks. `rebase_onto_mainline.py` took the worktree lock before looking at its arguments and treated any unknown word as "start". `apply_commit.py` passed `--help` to `git rev-parse`. No tool handled SIGTERM, SIGHUP or a closed pipe, so the replay stayed open.

## Fixed (2026-10-13)

- All three tools print their usage on `-h` and `--help` and exit 0; `land_self.py` and `rebase_onto_mainline.py` refuse other arguments with exit 2, and `apply_commit.py` refuses unknown options, before any lock or git change.
- `land_self.py` turns SIGTERM, SIGHUP, SIGINT and a closed output pipe into a stop. If a replay that this run started is open, it runs `rebase_onto_mainline.py --abort` and says so (exit 130); if that fails, the message names the command. A replay already open at start is left alone.
- Tests: `test_help_and_unknown_arguments` and `test_sigterm_aborts_the_replay` in `tools/tests/test_landing.py`.
- Open: a SIGKILL cannot be caught; recover with `rebase_onto_mainline.py --abort`.
