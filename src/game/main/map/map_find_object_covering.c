#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

/* Returns the first object whose footprint (width by height tiles from its
 * x and y) covers tile (x, y), or NULL when none does. */
MapObject* Map_FindObjectCovering(int x, int y) {
  MapObject* object = data_027c027c->unk_48[data_027c0284]->objects;

  for (; object->kind != 0; object++) {
    u8 left = object->x;
    u8 top = object->y;
    int right = left + object->width;
    int bottom = top + object->height;

    if (x >= left && y >= top && x < right && y < bottom) {
      return object;
    }
  }
  return NULL;
}
