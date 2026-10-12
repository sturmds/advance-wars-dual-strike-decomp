#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gTerrainIsProperty[]; /* per terrain id: 1 for properties */

BOOL Unit_IsTileOwnedByTeam(int unitIndex, int tile);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE(x, y) (MAP->tiles[(x) + MAP->rowStart[y]])

/* Value of the properties the acting unit can reach: 1 for each property
 * on a reached tile that its team does not own, 30 for an HQ. */
int Ai_ScoreReachableEnemyProperties(void) {
  int score = 0;
  int x;
  int y;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!gTerrainIsProperty[MAP_TILE_TERRAIN(TILE(x, y))]) {
        continue;
      }
      if (Unit_IsTileOwnedByTeam(STATE->actingUnitIndex, TILE(x, y))) {
        continue;
      }
      if (MAP_TILE_TERRAIN(TILE(x, y)) == TERRAIN_HQ) {
        score += 30;
      } else {
        score += 1;
      }
    }
  }
  return score;
}
