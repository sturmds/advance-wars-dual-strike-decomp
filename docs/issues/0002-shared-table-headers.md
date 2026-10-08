# 0002: Agents create separate struct headers for the same table

- **Status:** in progress
- **Area:** loop
- **Opened:** 2026-10-07
- **Milestone:** M1

## Problem

Each agent defines the structs it needs in a new `include/game/Unk_<address>.h`. When several functions use the same global table, agents working on them in parallel will each create their own copy of the entry struct. That breaks TENETS 3 (one shared definition) and makes merges messy.

Example: `data_0229b3b0` (80 entries of 0x3c bytes) has 9 users, including func_02025b6c, func_02025bb4 and func_02025c48. `Unk_02025bfc` already describes one entry.

## Notes

- Option A: before a batch, the orchestrator groups functions by shared data and gives each group one header name in the prompt.
- Option B: `fn.py show` prints any existing header that already describes a data symbol (grep `include/` for the symbol name or address).
- Option C: run all users of one table in a single agent, one after another.

- 2026-10-08: the CO table and the map header table have shared headers before any agent reaches them: `include/game/co_data.h` (`CoRecord`, `CoPowerBlock`, `CoStatRow`, `CoTagRecord`, with `gCoTable`) and `include/game/map_header.h` (`MapHeader`, with `Ov000_gMapHeaders`). The decomp skill now tells agents to look in `include/game/` for an existing header first. Other tables still have the problem.

## Done when

Two agents working on users of the same table end up with one header, without hand merging.
