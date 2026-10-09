#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL func_020b0d94(void);
void func_020b81a8(void);
BOOL func_020a9c78(s16 *site);
void func_01ffb200(s8 *map);     /* points rangeMap's rows into map, so range fills write there */
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_FillUnitMoves(MapUnit *unit);
BOOL Unit_CanRepairAt(MapUnit *unit, int x, int y);
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit *unit, int x, int y);
int Ai_IsTileNeededByAlly(s16 x, s16 y);
BOOL Ai_CanEndMoveAt(s16 x, s16 y);
void Ai_ClearActingUnitTarget(void);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE(x, y) ((x) + MAP->rowStart[y])

#define FAR_AWAY 110                   /* movement of the fill from the site, and the distance of tiles it misses */

/* Like AiUnit_RetreatToRepair, for a unit that func_020b0d94 says needs repair
 * (guess). func_020a9c78 picks a site (the unit's own tile when it finds
 * none); MapTiles.unk_2d48 gets the distance from there. The unit moves to
 * the reachable free tile with the lowest threat where Unit_CanRepairAt or
 * Unit_IsBlackHoleOnFlaggedTile holds, ties to the shorter distance (110 when the fill
 * missed the tile). */
void AiUnit_RetreatTowardSite(void)
{
    int bestX;
    int bestY;
    int bestThreat;
    int bestDistance;
    int threat;
    int x;
    int y;
    int distance;
    int tile;
    s16 site[2];

    bestX = -1;
    bestY = bestX;
    bestThreat = 0x7fff;
    bestDistance = bestThreat;
    if (!func_020b0d94()) {
        return;
    }
    func_020b81a8();
    if (!func_020a9c78(site)) {
        site[0] = STATE->actingUnit->x;
        site[1] = STATE->actingUnit->y;
    }
    func_01ffb200(MAP->unk_2d48);
    RangeMap_FillMoves(site[0], site[1], STATE->actingUnit->type, FAR_AWAY, 0);
    func_01ffb200(MAP->unk_2840);
    RangeMap_FillUnitMoves(STATE->actingUnit);
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (STATE->rangeMap[y][x] < 0) {
                continue;
            }
            if (!Unit_CanRepairAt(STATE->actingUnit, x, y)) {
                if (!Unit_IsBlackHoleOnFlaggedTile(STATE->actingUnit, x, y)) {
                    continue;
                }
            }
            if (MAP->unitAt[TILE(x, y)] != 0) {
                continue;
            }
            if (Ai_IsTileNeededByAlly(x, y) == 1) {
                continue;
            }
            if (!Ai_CanEndMoveAt(x, y)) {
                continue;
            }
            tile = TILE(x, y);
            threat = MAP->threatMax[tile] + MAP->threatSum[tile] * 2;
            distance = MAP->unk_2d48[tile];
            if (distance < 0) {
                distance = FAR_AWAY;
            }
            if (threat > bestThreat) {
                continue;
            }
            if (threat == bestThreat && distance > bestDistance) {
                continue;
            }
            bestX = x;
            bestY = y;
            bestThreat = threat;
            bestDistance = distance;
        }
    }
    if (bestX == -1) {
        return;
    }
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(bestX, bestY, 2, 0, 0);
}
