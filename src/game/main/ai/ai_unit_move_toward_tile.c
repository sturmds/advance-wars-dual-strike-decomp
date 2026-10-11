#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void RangeMap_SetBuffer(void *tileMap);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_FillUnitMoves(MapUnit *unit);
BOOL AiTile_IsSafeStop(s16 x, s16 y);
int Map_AxisDistanceGap(s16 x0, s16 y0, s16 x1, s16 y1);
BOOL Ai_HasVisibleEnemyWithin(s16 x, s16 y, int radius);
int Unit_IsEnemyTile(int unitIndex, s16 x, s16 y);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

#define UNLIMITED_MOVE 110
#define NO_TILE 0x7fff
#define WORST_DANGER 768

/* Moves the acting unit as far toward (targetX, targetY) as it can this
 * turn and waits there. MapTiles.rangeScratch gets the acting unit type's move
 * cost from the target. Of the tiles in reach (AiTile_IsSafeStop accepts them)
 * it takes the one with the lowest cost, then the lowest danger
 * (MapTiles.threatMax plus twice threatSum), then the shortest
 * Map_AxisDistanceGap distance to the target. A Sub or Stealth first tries the
 * same pick among tiles that are not enemy-owned and have no visible
 * enemy next to them; if one is found the unit goes there. */
void AiUnit_MoveTowardTile(int targetX, int targetY)
{
    int prefCost;
    int bestX;
    int bestY;
    int prefX;
    int prefY;
    int bestDist;
    int prefDist;
    int bestCost;
    int bestDanger;
    int prefDanger;
    int cost;
    int danger;
    int dist;
    int x;
    int y;

    bestY = 0;
    bestX = -1;
    prefX = bestX;
    prefY = bestY;
    bestDist = bestY;
    prefDist = bestY;
    bestDanger = WORST_DANGER;
    prefDanger = bestDanger;
    RangeMap_SetBuffer(MAP->rangeScratch);
    RangeMap_FillMoves(targetX, targetY, STATE->actingUnit->type, UNLIMITED_MOVE, 0);
    RangeMap_SetBuffer(MAP->rangeMapTiles);
    RangeMap_FillUnitMoves(STATE->actingUnit);
    if (AiTile_IsSafeStop(STATE->actingUnit->x, STATE->actingUnit->y)) {
        bestCost = MAP->rangeScratch[TILE_INDEX(STATE->actingUnit->x, STATE->actingUnit->y)];
    } else {
        bestCost = NO_TILE;
    }
    prefCost = bestCost;
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (STATE->rangeMap[y][x] < 0) {
                continue;
            }
            if (MAP->rangeScratch[TILE_INDEX(x, y)] > bestCost) {
                continue;
            }
            if (!AiTile_IsSafeStop(x, y)) {
                continue;
            }
            danger = MAP->threatMax[TILE_INDEX(x, y)] + MAP->threatSum[TILE_INDEX(x, y)] * 2;
            dist = Map_AxisDistanceGap(targetX, targetY, x, y);
            if (STATE->actingUnit->type == UNIT_SUB || STATE->actingUnit->type == UNIT_STEALTH) {
                if (Unit_IsEnemyTile(STATE->actingUnitIndex, x, y) != 1 && Ai_HasVisibleEnemyWithin(x, y, 1) != 1) {
                    cost = MAP->rangeScratch[TILE_INDEX(x, y)];
                    if (prefCost == cost) {
                        if (danger > prefDanger) {
                            goto pickBest;
                        }
                        if (danger == prefDanger && dist > prefDist) {
                            goto pickBest;
                        }
                    }
                    prefCost = cost;
                    prefX = x;
                    prefY = y;
                    prefDanger = danger;
                    prefDist = dist;
                }
            }
        pickBest:
            cost = MAP->rangeScratch[TILE_INDEX(x, y)];
            if (bestCost == cost) {
                if (danger > bestDanger) {
                    continue;
                }
                if (danger == bestDanger && dist > bestDist) {
                    continue;
                }
            }
            bestCost = cost;
            bestDanger = danger;
            bestDist = dist;
            bestX = x;
            bestY = y;
        }
    }
    if (prefX > -1) {
        AiUnit_PerformAction(prefX, prefY, AI_ACTION_WAIT, 0, 0);
    }
    if (bestX <= -1) {
        return;
    }
    AiUnit_PerformAction(bestX, bestY, AI_ACTION_WAIT, 0, 0);
}
