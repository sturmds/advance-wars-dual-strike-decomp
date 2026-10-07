# 0005: Names the agents guessed, to confirm or change

- **Status:** open
- **Area:** decomp
- **Opened:** 2026-10-07
- **Milestone:** M1

## Problem

Some names come from an agent's reading of the code, not from proof. They are fine as working names, but sturmds should confirm or change them.

| Name | Where | Reading | Evidence |
|---|---|---|---|
| `HasTwoOrMoreTeams` | main 0x02066818 | Returns TRUE when player slots use at least two of four values 0-3 | Versus mode has teams A–D and needs two teams to start; not confirmed in game |
| `Ov003_RandNext` | ov003 0x023517e4 | Steps a random state and returns it | Callers take the result modulo a range |
| `Ov003_RandSeed` | ov003 0x02351814 | Stores the seed for `Ov003_RandNext` | Writes the same state variable |
| `gOv003RandState` | ov003 data | State for the two above | |
| `BgControl_*` | main 0x0200a7b0-0x0200a7f0 | Set fields of a BGxCNT register value | Field positions and units match GBATEK |
| `gRomFileArchive` | main 0x022ab854 | The ROM file system archive | Used by `FS_GetOverlayFileID`; the real SDK name may differ |
| `MTX_RotX43_` | main 0x0210969c | 4x3 rotation about X | Matrix layout in the stores; SDK name not confirmed |

Several functions kept address names and carry a guess in their commit message instead (func_02002620 "sprite packing", func_ov006_0235f690 "units per army").

## Done when

Each row is confirmed, renamed (`fn.py rename`), or moved to a list of names that stay provisional.
