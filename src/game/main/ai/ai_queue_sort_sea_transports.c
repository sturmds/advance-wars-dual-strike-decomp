#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiQueue_SortDescending(int count);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sorts gAiQueue, largest key first. Units on a port or shoal come
 * before the rest; in each group, empty transports come first, then
 * full ones, then those with one unit aboard. */
void AiQueue_SortSeaTransports(void) {
  int i;
  MapUnit* unit;
  MapTiles* map;
  int terrain;

  /* needed to match: a plain for condition rotates the loop */
  for (i = 0;; i++) {
    if (gAiQueue[i] == 0) {
      break;
    }
    unit = &STATE->units[gAiQueue[i]];
    map = STATE->mapTiles;
    terrain = MAP_TILE_TERRAIN(map->tiles[map->rowStart[unit->y] + unit->x]);
    if (terrain == TERRAIN_PORT || terrain == TERRAIN_SHOAL) {
      if (unit->cargo[0] == 0) {
        gAiQueueSortKeys[i] = 5;
      } else if (unit->cargo[1] != 0) {
        gAiQueueSortKeys[i] = 4;
      } else {
        gAiQueueSortKeys[i] = 3;
      }
    } else {
      if (unit->cargo[0] == 0) {
        gAiQueueSortKeys[i] = 2;
      } else if (unit->cargo[1] != 0) {
        gAiQueueSortKeys[i] = 1;
      } else {
        gAiQueueSortKeys[i] = 0;
      }
    }
  }
  AiQueue_SortDescending(i);
}
