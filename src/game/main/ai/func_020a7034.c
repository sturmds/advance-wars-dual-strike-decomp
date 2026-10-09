#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType);
int func_0209e230(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAX_RANGE() func_0209e230(STATE->currentPlayer, STATE->actingUnit->type)

/* For an indirect unit about to attack the tile in pos: replaces pos with
 * the reachable tile, 0 to 110 move cost, that is in the unit's firing range
 * of it, nearest first and then cheapest to reach. Returns FALSE when no
 * tile qualifies. Other unit kinds keep pos and return TRUE.
 * WIP: the compiler keeps xMax and yMax in registers and spills the zero
 * for dist; the original does the opposite. */
BOOL func_020a7034(s16 *pos)
{
    int xMin;
    int cost;
    int yMax;
    int bestX;
    int bestDist;
    int y;
    int xMax;
    int x;
    int dist;
    int bestY;

    if (UnitType_GetAttackKind(STATE->actingUnit->type) != ATTACK_KIND_INDIRECT) {
        return TRUE;
    }
    bestY = 9999;
    bestX = bestY;
    bestDist = MAX_RANGE() + 1;
    xMin = pos[0] - MAX_RANGE() < 0 ? 0 : pos[0] - MAX_RANGE();
    y = pos[1] - MAX_RANGE() < 0 ? 0 : pos[1] - MAX_RANGE();
    xMax = STATE->unk_1c4->mapWidth - 1;
    if (pos[0] + MAX_RANGE() < xMax) {
        xMax = pos[0] + MAX_RANGE();
    }
    yMax = STATE->unk_1c4->mapHeight - 1;
    if (pos[1] + MAX_RANGE() < yMax) {
        yMax = pos[1] + MAX_RANGE();
    }
    for (; y <= yMax; y++) {
        for (x = xMin; x <= xMax; x++) {
            dist = 0;
            dist += pos[0] - x > 0 ? pos[0] - x : x - pos[0];
            dist += pos[1] - y > 0 ? pos[1] - y : y - pos[1];
            if (dist < Ov000_gUnitStats[STATE->actingUnit->type].minRange) {
                continue;
            }
            if (dist > MAX_RANGE() || dist > bestDist) {
                continue;
            }
            cost = STATE->rangeMap[y][x];
            if (cost < 0 || cost > 110) {
                continue;
            }
            if (bestX != 9999 && dist == bestDist && cost > STATE->rangeMap[bestY][bestX]) {
                continue;
            }
            bestDist = dist;
            bestX = x;
            bestY = y;
        }
    }
    if (bestX == 9999) {
        return FALSE;
    }
    pos[0] = bestX;
    pos[1] = bestY;
    return TRUE;
}
