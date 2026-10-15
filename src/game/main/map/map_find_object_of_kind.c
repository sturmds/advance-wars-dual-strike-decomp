#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

/* Returns the first object of the given kind, or NULL when there is none. */
MapObject* Map_FindObjectOfKind(int kind) {
  MapObject* object = data_027c027c->unk_48[data_027c0284]->objects;

  for (; object->kind != 0; object++) {
    if (object->kind == kind) {
      return object;
    }
  }
  return NULL;
}
