#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/range_map.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_ov000_022f4fd7[];       /* Ov000_gUnitStats[0].moveGroup, indexed by type * sizeof(UnitStats) */
extern u8 gAiTransportsSentToUnit[64];

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
BOOL func_0209f3ec(s16 x, s16 y, int arg);
s16 func_020a756c(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* For a land transport: picks an own Infantry or Mech it can reach this turn
 * that no other transport has claimed (gAiTransportsSentToUnit), preferring
 * the one it already chose and tiles where func_0209f3ec holds. Claims the
 * foot unit, stores its slot in the transport's aiTargetX and writes the tile
 * to move to. If the foot unit has a target of its own, the tile is the one
 * the transport reaches that is closest to that target.
 * WIP: 20 instructions differ, register choice in the last loop only. */
BOOL func_020aa4dc(s16 *tile)
{
    MapUnit *unit;
    int col;
    int cost;
    int bestX;
    int row;
    int reach;
    int bestY;
    int best;
    int slot;
    int x;
    int y;
    bestX = -1;
    bestY = -1;
    best = 0x7fff;

    if (data_ov000_022f4fd7[STATE->actingUnit->type * sizeof(UnitStats)] & MOVE_GROUP_SEA) {
        return FALSE;
    }
    RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y, STATE->actingUnit->type, func_020a756c(), 0);
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (STATE->rangeMap[y][x] < 0) {
                continue;
            }
            slot = STATE->mapTiles->unitAt[TILE(x, y)];
            if (slot == 0 || !Unit_IsCurrentPlayers(slot)) {
                continue;
            }
            if (gAiTransportsSentToUnit[slot & 63] != 0) {
                continue;
            }
            unit = &STATE->units[slot];
            if (unit->type > UNIT_MECH) {
                continue;
            }
            cost = STATE->rangeMap[y][x];
            if ((slot & 63) != STATE->actingUnit->aiTargetX || !unit->unk_0c_6) {
                cost += 256;
            }
            if (!func_0209f3ec(x, y, 5)) {
                cost += 512;
            }
            if (best >= cost) {
                bestX = x;
                bestY = y;
                best = cost;
            }
        }
    }
    if (best == 0x7fff) {
        STATE->actingUnit->aiTargetX = MAP_UNIT_NO_TARGET;
        return FALSE;
    }
    gAiTransportsSentToUnit[STATE->mapTiles->unitAt[TILE(bestX, bestY)] & 63]++;
    STATE->actingUnit->aiTargetX = STATE->mapTiles->unitAt[TILE(bestX, bestY)] & 63;
    unit = &STATE->units[STATE->mapTiles->unitAt[TILE(bestX, bestY)]];
    unit->unk_0c_6 = 1;
    if (unit->aiTargetX != MAP_UNIT_NO_TARGET) {
        RangeMap_SetBuffer(STATE->mapTiles->rangeScratch);
        RangeMap_FillMoves(unit->aiTargetX, unit->aiTargetY, STATE->actingUnit->type, 110, 0);
        RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
        RangeMap_FillMoves(bestX, bestY, STATE->actingUnit->type, gAiTurnState->unk_05, 0);
        reach = STATE->mapTiles->rangeScratch[TILE(bestX, bestY)];
        if (reach >= 0) {
            for (row = 0; row < STATE->unk_1c4->mapHeight; row++) {
                for (col = 0; col < STATE->unk_1c4->mapWidth; col++) {
                    if (STATE->rangeMap[row][col] < 0) {
                        continue;
                    }
                    cost = STATE->mapTiles->rangeScratch[TILE(col, row)];
                    if (reach >= cost) {
                        bestX = col;
                        bestY = row;
                        reach = cost;
                    }
                }
            }
        }
    }
    tile[0] = bestX;
    tile[1] = bestY;
    return TRUE;
}
