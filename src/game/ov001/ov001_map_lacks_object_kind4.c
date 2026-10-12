#include "types.h"
#include "game/map_object.h"

/* 1 when the map has an object of the given kind (MapObject.kind) that is not destroyed. */

BOOL Ov001_MapLacksObjectKind4(void) { return MapObjects_HasKind(4) == 0; }
