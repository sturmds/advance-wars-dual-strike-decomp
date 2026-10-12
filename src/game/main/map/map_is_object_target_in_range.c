#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

MapObject* Map_FindObjectCovering(int x, int y);
BOOL MapObject_GetTargetTile(MapObject* object, s16* pos);

/* TRUE when the object covering (x, y) has a target tile that the range map reaches (not negative). */
BOOL Map_IsObjectTargetInRange(int x, int y) {
  s16 pos[2];
  MapObject* object = Map_FindObjectCovering(x, y);

  if (object == NULL) {
    return FALSE;
  }
  if (!MapObject_GetTargetTile(object, pos)) {
    return FALSE;
  }
  return STATE->mapTiles
             ->rangeMapTiles[pos[0] + STATE->mapTiles->rowStart[pos[1]]] >= 0;
}
