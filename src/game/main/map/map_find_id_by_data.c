#include "types.h"
#include "game/map_header.h"

/* Returns the id of the map whose header points at mapData, or MAP_COUNT
 * if none does. */
u16 Map_FindIdByData(const void* mapData) {
  u16 mapId;

  for (mapId = 0; mapId < MAP_COUNT; mapId++) {
    if (Ov000_gMapHeaders[mapId].mapData == mapData) {
      break;
    }
  }
  return mapId;
}
