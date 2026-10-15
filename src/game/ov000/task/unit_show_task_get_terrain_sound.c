#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_show_task.h"
#include "game/terrain.h"

/* Sound effect id for the terrain under the task's tile; 0: none. Plains
 * sound depends on the map's tileset. The case order is needed to match. */
int UnitShowTask_GetTerrainSound(UnitShowTask* task) {
  int tileset;
  int terrain = MAP_TILE_TERRAIN(
      MAP_STATE->mapTiles
          ->tiles[MAP_STATE->mapTiles->rowStart[(task->y + 8) / 16] +
                  (task->x + 8) / 16]);

  switch (terrain) {
    case TERRAIN_MOUNTAIN:
      return 91;
    case TERRAIN_WOOD:
      return 88;
    case TERRAIN_SHOAL:
      return 95;
    case TERRAIN_ROAD:
      return 93;
    case TERRAIN_PLAIN:
      tileset = MAP_STATE->unk_1c4->tileset & 0x7f;
      switch (tileset) {
        default:
          return tileset != 3 ? 89 : 94;
        case 1:
          return 90;
        case 2:
          return 95;
      }
    case TERRAIN_CITY:
    case TERRAIN_HQ:
    case TERRAIN_AIRPORT:
    case TERRAIN_PORT:
    case 12:
    case TERRAIN_BASE:
      return 89;
    case TERRAIN_RIVER:
    case TERRAIN_SEA:
    case 19:
      return 92;
    default:
      return 0;
  }
}
