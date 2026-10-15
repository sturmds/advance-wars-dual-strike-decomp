#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_reach_tile.h"
#include "game/unit_stats.h"

extern AiReachTile* data_022a6dd0;
BOOL Map_IsCapturableByActingUnit(s16 x, s16 y);

int func_020dc1a8(void) {
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
      if (MAP_STATE->rangeMap[y][x] >= 0 &&
          Map_IsCapturableByActingUnit(x, y) == 1) {
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
