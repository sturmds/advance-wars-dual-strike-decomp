#include "types.h"
#include "game/map_object.h"

/* 1 when the map has an object of the given kind (MapObject.kind) that is not destroyed. */

BOOL Ov001_MapLacksObjectKinds11To13(void) {
  if (MapObjects_HasKind(11)) {
    return 0;
  }
  if (MapObjects_HasKind(12)) {
    return 0;
  }
  return MapObjects_HasKind(13) == 0;
}
