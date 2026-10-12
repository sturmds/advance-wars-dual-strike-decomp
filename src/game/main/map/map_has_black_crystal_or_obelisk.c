#include "types.h"
#include "game/map_object.h"

/* TRUE when the map has a live Black Crystal or Black Obelisk. */
BOOL Map_HasHealObject(void) {
  if (MapObjects_HasKind(MAP_OBJECT_BLACK_CRYSTAL)) {
    return TRUE;
  }
  return MapObjects_HasKind(MAP_OBJECT_BLACK_OBELISK) != 0;
}
