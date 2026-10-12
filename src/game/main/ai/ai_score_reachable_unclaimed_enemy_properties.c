#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gTerrainIsProperty[]; /* per terrain id: 1 for properties */

BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])

/* Like Ai_ScoreReachableEnemyProperties, but a property that AI units are
 * already heading for to capture counts only if it is an HQ: 1 for each
 * other property on a reached tile that the acting unit's team does not own,
 * 30 for an HQ. */
int Ai_ScoreReachableUnclaimedEnemyProperties(void) {
  int score = 0;
  int x;
  int y;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!gTerrainIsProperty[MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(x, y)])]) {
        continue;
      }
      if (Unit_IsTileOwnedByTeam(STATE->actingUnitIndex,
                                 MAP->tiles[TILE_INDEX(x, y)])) {
        continue;
      }
      if (STATE->unk_6088[MAP->unk_1928[TILE_INDEX(x, y)]].claimCount != 0 &&
          MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(x, y)]) != TERRAIN_HQ) {
        continue;
      }
      if (MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(x, y)]) == TERRAIN_HQ) {
        score += 30;
      } else {
        score += 1;
      }
    }
  }
  return score;
}
