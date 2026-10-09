#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/strike_target.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_01ffa44c(MapUnit *unit);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
BOOL func_0209e1ac(int x, int y);
int func_0209f560(MapUnit *unit); /* turns the unit needs to capture a property; 20 at 0 hp */
void Ai_ClearActingUnitTarget(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* AiUnit_PerformAction actions; names are guesses from this function */
#define AI_ACTION_CAPTURE     3
#define AI_ACTION_LAUNCH_SILO 22 /* arg3, arg4: target x, y */

/* Infantry and mechs: picks a free property tile in reach to capture, scored by
 * its unk_d0 value plus a bonus per terrain, or a missile silo to launch. A unit
 * that would need 5 or more turns to capture only looks at silos. Ties go to
 * the tile with the lower unk_3250 + unk_3758 (enemy threat, a guess), then to
 * the roll. */
void AiFoot_PickCaptureTarget(void)
{
    /* needed to match: declaration order sets the registers */
    int captureX;
    int captureY;
    int siloX;
    int siloY;
    int bestScore;
    int bestThreat;
    int siloScore;
    int terrain;
    int x;
    int y;
    int siloOnly;
    int score;
    int threat;
    s16 site[2];

    if (STATE->actingUnit->type > UNIT_MECH) {
        return;
    }
    func_01ffa44c(STATE->actingUnit);
    captureY = 9999;
    captureX = captureY;
    siloY = captureY;
    siloX = captureY;
    bestScore = -1;
    siloScore = bestScore;
    bestThreat = 0x7fff;
    siloOnly = func_0209f560(STATE->actingUnit) >= 5;
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (STATE->unk_d0[y][x] < 0) {
                continue;
            }
            if (STATE->mapTiles->unitAt[TILE(x, y)] != 0) {
                continue;
            }
            terrain = MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)]);
            if (siloOnly == 1 && terrain != TERRAIN_SILO) {
                continue;
            }
            if (!func_0209e1ac(x, y)) {
                continue;
            }
            if (x == STATE->actingUnit->aiTargetX && y == STATE->actingUnit->aiTargetY) {
                score = 4; /* the tile the unit was already heading for */
            } else {
                score = STATE->unk_d0[y][x];
            }
            switch (terrain) {
            case TERRAIN_HQ:
                score += 25;
                break;
            case TERRAIN_BASE:
                score += 20;
                break;
            case TERRAIN_SILO:
                score += 15;
                break;
            case TERRAIN_AIRPORT:
                score += 10;
                break;
            case TERRAIN_PORT:
            case TERRAIN_COM_TOWER:
                score += 5;
                break;
            }
            if (terrain != TERRAIN_SILO) {
                if (bestScore > score) {
                    continue;
                }
                threat = STATE->mapTiles->unk_3250[TILE(x, y)] + STATE->mapTiles->unk_3758[TILE(x, y)];
                if (bestScore == score) {
                    if (bestThreat < threat) {
                        continue;
                    }
                    if (bestThreat == threat && gAiTurnState->roll < 50) {
                        continue;
                    }
                }
                captureX = x;
                bestScore = score;
                bestThreat = threat;
                captureY = y;
            } else if (siloScore <= score) {
                siloX = x;
                siloScore = score;
                siloY = y;
            }
        }
    }
    if (siloScore > bestScore) {
        site[0] = siloX;
        site[1] = siloY;
        if (Strike_FindBestSite(site, STATE->currentPlayer, 1, STRIKE_MODE_AI_FUNDS) == 1) {
            AiUnit_PerformAction(siloX, siloY, AI_ACTION_LAUNCH_SILO, site[0], site[1]);
            return;
        }
    }
    if (bestScore == -1) {
        return;
    }
    if (func_0209f560(STATE->actingUnit) >= 2) {
        STATE->unk_6088[STATE->mapTiles->unk_1928[TILE(captureX, captureY)]].unk_03++;
    }
    Ai_ClearActingUnitTarget();
    AiUnit_PerformAction(captureX, captureY, AI_ACTION_CAPTURE, 0, 0);
}
