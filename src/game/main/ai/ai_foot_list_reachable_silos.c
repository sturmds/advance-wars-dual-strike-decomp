#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_reach_tile.h"
#include "game/unit_stats.h"

extern AiReachTile* data_022a6dd0;

BOOL Map_IsSiloTile(s16 x, s16 y);

/* For an acting Infantry or Mech: lists the reachable tiles that
 * Map_IsSiloTile accepts (TERRAIN_SILO) in data_022a6dd0 and returns
 * how many. Other unit types get 0. */
int AiFoot_ListReachableSilos(void) {
  int count;
  AiReachTile* out;
  int x;
  int y;

  out = data_022a6dd0;
  count = 0;
  if (MAP_STATE->actingUnit->type != UNIT_INFANTRY &&
      MAP_STATE->actingUnit->type != UNIT_MECH) {
    return count;
  }
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] >= 0 && Map_IsSiloTile(x, y) == 1) {
        out->x = x;
        out->y = y;
        count++;
        out->cost = MAP_STATE->rangeMap[y][x];
        out++;
      }
    }
  }
  out->cost = -1;
  return count;
}
