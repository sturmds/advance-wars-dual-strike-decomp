#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType);
int func_0209e230(int player, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAX_RANGE() func_0209e230(STATE->currentPlayer, STATE->actingUnit->type)

BOOL func_020a7034(s16 *pos)
{
    int bestX;
    int bestY;
    int bestDist;
    int xMin;
    int xMax;
    int yMax;
    int x;
    int y;
    int dist;
    int cost;

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
