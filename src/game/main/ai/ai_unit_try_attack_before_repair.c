#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int UnitType_GetAttackKind(int unitType); /* 0 direct, 1 indirect, 2 unarmed */
void RangeMap_FillUnitMoves(MapUnit *unit);
void RangeMap_MarkAdjacentTiles(int arg);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
BOOL Map_IsTileVisible(s16 x, s16 y);
int AiUnit_TeammateOutdamages(MapUnit *target);
BOOL FindAttackTile(int x, int y, s16 *pos);
int AiAttack_Score(int arg, s16 *pos);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* First step of AiUnit_HeadForRepairTile. A direct-attack unit looks for an
 * enemy unit it can hit from a tile in reach, scored by AiAttack_Score with
 * AI_ATTACK_HEAVY_HIT. As in AiFoot_PickAttackTarget, when
 * AiUnit_TeammateOutdamages holds for the target it needs two or more open
 * sides. If it finds one it attacks and gives up its repair request. */
void AiUnit_TryAttackBeforeRepair(void)
{
    int maxY;
    int minX;
    int attackX;
    int attackY;
    int bestScore;
    int score;
    int bestSlot;
    int x;
    int y;
    MapUnit *target;
    MapUnit *neighbour;
    s16 pos[2];
    int maxX;
    int minY;
    int openSides;
    int xx;
    int yy;
    int dist;

    attackX = -1;
    bestScore = 0;
    attackY = attackX;
    bestSlot = bestScore;
    if (UnitType_GetAttackKind(STATE->actingUnit->type) != 0) {
        return;
    }
    RangeMap_FillUnitMoves(STATE->actingUnit);
    RangeMap_MarkAdjacentTiles(111);
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (STATE->rangeMap[y][x] < 0) {
                continue;
            }
            target = &STATE->units[STATE->mapTiles->unitAt[TILE(x, y)]];
            if (target->type == UNIT_NONE) {
                continue;
            }
            if (STATE->players[(STATE->actingUnitIndex >> 6) + 1].team == STATE->players[MAP_UNIT_OWNER(STATE, target)].team) {
                continue;
            }
            if (!Map_IsTileVisible(x, y)) {
                continue;
            }
            if (!FindAttackTile(x, y, pos)) {
                continue;
            }
            score = AiAttack_Score(AI_ATTACK_HEAVY_HIT | STATE->mapTiles->unitAt[TILE(x, y)], pos);
            if (score == 0) {
                continue;
            }
            if (score < bestScore) {
                continue;
            }
            if (AiUnit_TeammateOutdamages(target) == 1) {
                openSides = 0;
                minX = target->x - 1;
                if (minX < 0) {
                    minX = 0;
                }
                minY = target->y - 1;
                if (minY < 0) {
                    minY = 0;
                }
                maxX = STATE->unk_1c4->mapWidth - 1;
                if (target->x + 1 < maxX) {
                    maxX = target->x + 1;
                }
                maxY = STATE->unk_1c4->mapHeight - 1;
                if (target->y + 1 < maxY) {
                    maxY = target->y + 1;
                }
                for (yy = minY; yy <= maxY; yy++) {
                    for (xx = minX; xx <= maxX; xx++) {
                        dist = 0;
                        if (target->x - xx > 0) {
                            dist += target->x - xx;
                        } else {
                            dist += xx - target->x;
                        }
                        if (target->y - yy > 0) {
                            dist += target->y - yy;
                        } else {
                            dist += yy - target->y;
                        }
                        if (dist < 1 || dist > 1) {
                            continue;
                        }
                        if (STATE->rangeMap[yy][xx] < 0) {
                            continue;
                        }
                        neighbour = &STATE->units[STATE->mapTiles->unitAt[TILE(xx, yy)]];
                        if (neighbour->type != UNIT_NONE && (neighbour->flags & MAP_UNIT_WAITED)) {
                            continue;
                        }
                        openSides++;
                    }
                }
                if (openSides <= 1) {
                    continue;
                }
            }
            attackX = pos[0];
            attackY = pos[1];
            bestSlot = STATE->mapTiles->unitAt[TILE(x, y)];
            bestScore = score;
        }
    }
    if (bestScore == 0) {
        return;
    }
    AI_TARGET_FLAGS->unk_1 = 0;
    STATE->actingUnit->serviceWanted = 0; /* no longer asks for repairs */
    AiUnit_PerformAction(attackX, attackY, AI_ACTION_ATTACK_UNIT, bestSlot, 0);
}
