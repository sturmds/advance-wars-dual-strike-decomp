#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/terrain.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit *unit);
void RangeMap_MarkAdjacentTiles(int arg);
BOOL Map_IsSiloOrCapturableTile(int x, int y);
BOOL AiUnit_FindStandTileNextTo(int x, int y, s16 *stand); /* guess: picks where the transport stops to drop a unit on (x, y) */
BOOL Ai_IsCaptureSiteTaken(int x, int y);
BOOL Ai_FindCaptureSiteNear(int x, int y, int distance, s16 *property); /* guess: a capturable property near (x, y) */
int Tile_GetDirection(int fromX, int fromY, int toX, int toY); /* direction: 0 same tile, 1 up, 2 right, 3 down, 4 left */
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* MapTiles.rangeScratch2 bits read here, for each foot unit type; set by
 * AiTurn_BuildCaptureMap. */
#define TILE_FLAG_INFANTRY 0x80
#define TILE_FLAG_MECH     0x40

/* A transport carrying a unit drops it for a capture. First pass: a free
 * property in reach (or next to it) that the cargo can capture, scored by
 * its rangeMap value plus a bonus per terrain (HQ highest), ties to the
 * lower threatMax + threatSum. Second pass, only for Infantry and Mech:
 * a free flagged tile with a property near it (within 3 tiles for
 * Infantry, 2 for Mech; with AI_STEP_ATTACK_DONE the tile must be a
 * property itself). Each pass that finds a tile sets the cargo's AI
 * target and queues the unload, so a second-pass result replaces the
 * first. */
void AiTransport_UnloadForCapture(void)
{
    /* needed to match: declaration order sets registers and stack slots,
     * and the second pass needs its own loop variables */
    int unloadX;
    int x2;
    int bestScore;
    int unloadY;
    MapUnit *cargo;
    s16 stand[2];
    int standX;
    int threat;
    int standY;
    int captureX;
    int captureY;
    int bestThreat;
    int unloadX2;
    int y2;
    int unloadY2;
    int score;
    int standX2;
    int x;
    int y;
    int standY2;
    s16 capture[2];

    unloadX = -1;
    unloadY = -1;
    standX = -1;
    standY = -1;
    captureX = -1;
    captureY = -1;
    bestScore = -1;
    bestThreat = 0x7fff;
    cargo = &STATE->units[STATE->actingUnit->cargo[0]];
    if (STATE->actingUnit->cargo[0] == 0) {
        return;
    }
    RangeMap_FillUnitMoves(STATE->actingUnit);
    RangeMap_MarkAdjacentTiles(111);
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            if (STATE->rangeMap[y][x] < 0) {
                continue;
            }
            if (STATE->mapTiles->unitAt[TILE(x, y)] != 0) {
                continue;
            }
            if (!Map_IsSiloOrCapturableTile(x, y)) {
                continue;
            }
            if (!AiUnit_FindStandTileNextTo(x, y, stand)) {
                continue;
            }
            if (STATE->unk_6088[STATE->mapTiles->unk_1928[TILE(x, y)]].claimCount != 0
                && !(gAiTurnState->stepFlags & AI_STEP_ATTACK_DONE)
                && Ai_IsCaptureSiteTaken(x, y) == TRUE) {
                continue;
            }
            score = STATE->rangeMap[y][x];
            threat = STATE->mapTiles->threatMax[TILE(x, y)] + STATE->mapTiles->threatSum[TILE(x, y)];
            switch (MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)])) {
            case TERRAIN_HQ:
                score += 100;
                break;
            case TERRAIN_BASE:
                score += 80;
                break;
            case TERRAIN_SILO:
                score += 60;
                break;
            case TERRAIN_AIRPORT:
                score += 40;
                break;
            case TERRAIN_PORT:
            case TERRAIN_COM_TOWER:
                score += 20;
                break;
            }
            if (bestScore > score) {
                continue;
            }
            if (bestScore == score && bestThreat < threat) {
                continue;
            }
            bestScore = score;
            bestThreat = threat;
            standX = stand[0];
            standY = stand[1];
            unloadX = x;
            unloadY = y;
        }
    }
    if (bestScore != -1) {
        cargo->aiTargetX = unloadX;
        cargo->aiTargetY = unloadY;
        STATE->unk_6088[STATE->mapTiles->unk_1928[TILE(unloadX, unloadY)]].claimCount++;
        AiUnit_PerformAction(standX, standY, AI_ACTION_UNLOAD, Tile_GetDirection(standX, standY, unloadX, unloadY), 0);
    }

    unloadY2 = -1;
    unloadX2 = unloadY2;
    standY2 = unloadY2;
    standX2 = unloadY2;
    bestScore = unloadY2;
    bestThreat = 0x7fff;
    for (y2 = 0; y2 < STATE->unk_1c4->mapHeight; y2++) {
        for (x2 = 0; x2 < STATE->unk_1c4->mapWidth; x2++) {
            if (STATE->rangeMap[y2][x2] < 0) {
                continue;
            }
            if (STATE->mapTiles->unitAt[TILE(x2, y2)] != 0) {
                continue;
            }
            if (!AiUnit_FindStandTileNextTo(x2, y2, stand)) {
                continue;
            }
            switch (cargo->type) {
            case UNIT_INFANTRY: /* needed to match: the flag byte is read unsigned */
                if (!((u8)STATE->mapTiles->rangeScratch2[TILE(x2, y2)] & TILE_FLAG_INFANTRY)) {
                    continue;
                }
                if (!(gAiTurnState->stepFlags & AI_STEP_ATTACK_DONE)) {
                    if (!Ai_FindCaptureSiteNear(x2, y2, 3, capture)) {
                        continue;
                    }
                } else {
                    if (!Map_IsSiloOrCapturableTile(x2, y2)) {
                        continue;
                    }
                    capture[0] = x2;
                    capture[1] = y2;
                }
                break;
            case UNIT_MECH:
                if (!((u8)STATE->mapTiles->rangeScratch2[TILE(x2, y2)] & TILE_FLAG_MECH)) {
                    continue;
                }
                if (!(gAiTurnState->stepFlags & AI_STEP_ATTACK_DONE)) {
                    if (!Ai_FindCaptureSiteNear(x2, y2, 2, capture)) {
                        continue;
                    }
                } else {
                    if (!Map_IsSiloOrCapturableTile(x2, y2)) {
                        continue;
                    }
                    capture[0] = x2;
                    capture[1] = y2;
                }
                break;
            default:
                continue;
            }
            score = STATE->rangeMap[y2][x2];
            threat = STATE->mapTiles->threatMax[TILE(x2, y2)] + STATE->mapTiles->threatSum[TILE(x2, y2)];
            switch (MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x2, y2)])) {
            case TERRAIN_HQ:
                score += 100;
                break;
            case TERRAIN_BASE:
                score += 80;
                break;
            case TERRAIN_SILO:
                score += 60;
                break;
            case TERRAIN_AIRPORT:
                score += 40;
                break;
            case TERRAIN_PORT:
            case TERRAIN_COM_TOWER:
                score += 20;
                break;
            }
            if (bestScore > score) {
                continue;
            }
            if (bestScore == score && bestThreat < threat) {
                continue;
            }
            bestScore = score;
            standX2 = stand[0];
            standY2 = stand[1];
            bestThreat = threat;
            unloadX2 = x2;
            captureX = capture[0];
            unloadY2 = y2;
            captureY = capture[1];
        }
    }
    if (bestScore == -1) {
        return;
    }
    cargo->aiTargetX = captureX;
    cargo->aiTargetY = captureY;
    STATE->unk_6088[STATE->mapTiles->unk_1928[TILE(captureX, captureY)]].claimCount++;
    AiUnit_PerformAction(standX2, standY2, AI_ACTION_UNLOAD, Tile_GetDirection(standX2, standY2, unloadX2, unloadY2), 0);
}
