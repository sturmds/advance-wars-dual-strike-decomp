#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Map_IsSiloOrCapturableTile(int x, int y);
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

#define UNLIMITED_MOVE 110
#define NO_TILE 0x7fff

/* Run by AiLoadedTransport_Act twice: sends a transport that carries a unit
 * towards a capturable property or silo. The acting unit keeps its old
 * target when it can still reach it, no unit stands there and no other
 * unit claimed it (the claim does not count once AI_STEP_ATTACK_DONE is
 * set). Otherwise it picks the reachable site with the lowest score: its
 * rangeMap value plus 0 for an HQ, 2 for a base or silo, 3 for an airport,
 * 4 for a port or com tower and 6 for other properties; ties go to the
 * later tile in row order. The chosen site is claimed. */
void AiLoadedTransport_HeadForSite(void) {
  int bestScore;
  int x;
  int y;
  int bestX;
  int bestY;
  int score;
  int targetX;
  int targetY;

  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y,
                     STATE->actingUnit->type, UNLIMITED_MOVE, 0);
  targetX = STATE->actingUnit->aiTargetX;
  if (targetX != MAP_UNIT_NO_TARGET) {
    targetY = STATE->actingUnit->aiTargetY;
    if (STATE->rangeMap[targetY][targetX] >= 0 &&
        Map_IsSiloOrCapturableTile(targetX, targetY)) {
      if ((STATE->unk_6088[MAP->unk_1928[TILE_INDEX(targetX, targetY)]]
                   .claimCount == 0 ||
           (gAiTurnState->stepFlags & AI_STEP_ATTACK_DONE)) &&
          MAP->unitAt[TILE_INDEX(targetX, targetY)] == 0) {
        STATE->unk_6088[MAP->unk_1928[TILE_INDEX(targetX, targetY)]]
            .claimCount++;
        AiUnit_MoveTowardTile(targetX, targetY);
      }
    }
  }
  bestX = -1;
  bestY = -1;
  bestScore = NO_TILE;
  STATE->actingUnit->aiTargetX = MAP_UNIT_NO_TARGET;
  STATE->actingUnit->aiTargetY = MAP_UNIT_NO_TARGET;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!Map_IsSiloOrCapturableTile(x, y)) {
        continue;
      }
      if (STATE->unk_6088[MAP->unk_1928[TILE_INDEX(x, y)]].claimCount != 0 &&
          !(gAiTurnState->stepFlags & AI_STEP_ATTACK_DONE)) {
        continue;
      }
      score = STATE->rangeMap[y][x];
      switch (MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(x, y)])) {
        case TERRAIN_HQ:
          break;
        case TERRAIN_BASE:
          score += 2;
          break;
        case TERRAIN_SILO:
          score += 2;
          break;
        case TERRAIN_AIRPORT:
          score += 3;
          break;
        case TERRAIN_PORT:
        case TERRAIN_COM_TOWER:
          score += 4;
          break;
        default:
          score += 6;
          break;
      }
      if (bestScore >= score) {
        bestX = x;
        bestY = y;
        bestScore = score;
      }
    }
  }
  if (bestScore == NO_TILE) {
    return;
  }
  STATE->unk_6088[MAP->unk_1928[TILE_INDEX(bestX, bestY)]].claimCount++;
  STATE->actingUnit->aiTargetX = bestX;
  STATE->actingUnit->aiTargetY = bestY;
  AiUnit_MoveTowardTile(bestX, bestY);
}
