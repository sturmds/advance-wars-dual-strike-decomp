#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

/* Counts as present only while the object is standing. */
BOOL MapObjects_HasKind(int kind) {
  MapObject* object = data_027c027c->unk_48[data_027c0284]->objects;
  int count = 0;

  for (; object->kind != 0; object++) {
    if (object->kind == kind) {
      if (MapObject_IsStanding(object)) {
        count++;
      }
    }
  }
  return count != 0;
}
