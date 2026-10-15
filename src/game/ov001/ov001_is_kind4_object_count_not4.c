#include "types.h"
#include "game/Unk_02080440.h"

/* Nonzero unless the map has exactly four objects of kind 4. */
BOOL Ov001_IsKind4ObjectCountNot4(void) {
  MapObject* object;
  int count = 0;

  for (object = MAP_STATE->objects; object->kind != 0; object++) {
    if (object->kind == 4) {
      count++;
    }
  }
  return count != 4;
}
