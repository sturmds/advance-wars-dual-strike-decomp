#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

BOOL Ov006_IsBridgeCell(int x, int y);

/* A bridge tile (terrain 12) whose cell is a bridge piece. */
BOOL Ov006_IsBridgeTile(int x, int y) {
  if (MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]] ==
      TERRAIN_BRIDGE) {
    if (Ov006_IsBridgeCell(x, y)) {
      return TRUE;
    }
  }
  return FALSE;
}
