#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_01ffa44c(MapUnit *unit);
BOOL func_020a770c(s16 x, s16 y);
int func_020a7fe0(s16 x, s16 y);
void func_020b0794(void);
int func_020c3954(int player, int unitType, int terrain); /* terrain defence stars */
void Ai_ClearActingUnitTarget(void);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Moves the acting unit to the free tile it can reach with the lowest
 * threat (MapTiles.unk_3250 + 2 * unk_3758). Ties go to more terrain stars,
 * then to the lower unk_d0 value. With gAiTurnState->unk_08 bit 0 set, tiles
 * with a negative MapTiles.unk_2d48 get -1 in unk_d0, and nothing is
 * queued when the best tile is the one the unit stands on. */
void AiUnit_MoveToSafestTile(void)
{
    int bestX;
    int bestY;
    int bestThreat;
    int bestStars;
    int bestReach;
    int threat;
    int x;
    int y;
    int stars;
    int reach;                         /* unk_d0 value of the tile */
    int tile;

    bestX = -1;
    bestY = bestX;
    bestThreat = 0x7fff;
    bestReach = bestThreat;
    bestStars = bestX;
    if (!(gAiTurnState->unk_08 & 0x10)) {
        func_020b0794();
    }
    if ((gAiTurnState->unk_08 & 1) && (gAiTurnState->unk_08 & 2)) {
        return;
    }
    func_01ffa44c(STATE->actingUnit);
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (STATE->unk_d0[y][x] < 0) {
                continue;
            }
            if (gAiTurnState->unk_08 & 1) {
                if (STATE->mapTiles->unk_2d48[TILE(x, y)] < 0) {
                    STATE->unk_d0[y][x] = -1;
                    continue;
                }
            }
            if (STATE->mapTiles->unitAt[TILE(x, y)] != 0) {
                continue;
            }
            if (func_020a7fe0(x, y) == 1) {
                continue;
            }
            if (!func_020a770c(x, y)) {
                continue;
            }
            tile = TILE(x, y);
            threat = STATE->mapTiles->unk_3250[tile] + STATE->mapTiles->unk_3758[tile] * 2;
            if (gAiTurnState->unk_08 & 0x10) {
                stars = 0;
            } else {
                stars = func_020c3954(STATE->currentPlayer, STATE->actingUnit->type,
                                      MAP_TILE_TERRAIN(STATE->mapTiles->tiles[tile]));
            }
            reach = STATE->unk_d0[y][x];
            if (threat > bestThreat) {
                continue;
            }
            if (threat == bestThreat && stars < bestStars) {
                continue;
            }
            if (threat == bestThreat && stars == bestStars && reach > bestReach) {
                continue;
            }
            bestX = x;
            bestY = y;
            bestThreat = threat;
            bestStars = stars;
            bestReach = reach;
        }
    }
    if (bestX == -1) {
        return;
    }
    if ((gAiTurnState->unk_08 & 1) && bestX == STATE->actingUnit->x && bestY == STATE->actingUnit->y) {
        return;
    }
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(bestX, bestY, 2, 0, 0); /* action 2: move and wait (guess) */
}
