#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_reach_tile.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern AiReachTile* data_022a6dd0;

BOOL Map_IsSiloTile(s16 x, s16 y);

#define STATE (data_027c027c->unk_48[data_027c0284])

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
  if (STATE->actingUnit->type != UNIT_INFANTRY &&
      STATE->actingUnit->type != UNIT_MECH) {
    return count;
  }
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] >= 0 && Map_IsSiloTile(x, y) == 1) {
        out->x = x;
        out->y = y;
        count++;
        out->cost = STATE->rangeMap[y][x];
        out++;
      }
    }
  }
  out->cost = -1;
  return count;
}
