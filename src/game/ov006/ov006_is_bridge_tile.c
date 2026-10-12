#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL Ov006_IsBridgeCell(int x, int y);

/* A bridge tile (terrain 12) whose cell is a bridge piece. */
BOOL Ov006_IsBridgeTile(int x, int y) {
  if (STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]] ==
      TERRAIN_BRIDGE) {
    if (Ov006_IsBridgeCell(x, y)) {
      return TRUE;
    }
  }
  return FALSE;
}
