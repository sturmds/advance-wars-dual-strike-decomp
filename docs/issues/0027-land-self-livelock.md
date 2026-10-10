# 0027: Many land_self.py runs at once rebuilt after every landing

- **Status:** closed
- **Area:** loop
- **Opened:** 2026-10-09
- **Milestone:** M1

## Problem

2026-10-09 18:41: 11 `land_self.py` runs were under way, the oldest for 1924 s, with 15 `make release` builds on 16 cores (load 26). Each run replayed and built outside any shared lock, then fast-forwarded under `land.lock` only if `mainline` had not moved. With N runs at once, each landing moved `mainline` under the other N-1, which then replayed and built again (up to 15 tries). The work grew with N², and the CPU load made every round slower.

## Fixed (2026-10-09)

`land_self.py` waits for an exclusive lock on `<git common dir>/land-queue.lock` before the replay and holds it through `make release`, the `PROGRESS.md` amend and the fast-forward. Landings run one at a time; a waiting run blocks in `flock` and uses no CPU. While it waits it prints the holder (pid, branch, start time) from the lock file. A restart from the replayed copy (issue 0026) passes the locked file on in `LAND_QUEUE_FD`, so the run keeps its turn.

`rebase_onto_mainline.py --continue` run by hand does not take the queue: it lands nothing, and `land_self.py` replays again inside its turn. Runs of an older `land_self.py` still land without queueing; the `mainline` check under `land.lock` keeps them correct, and a queued run that loses to one replays again.

Tests: `make test` (`LandQueueTest` and `test_keeps_queue_turn_across_restart` in `tools/tests/test_landing.py`).
