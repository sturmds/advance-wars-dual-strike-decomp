#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Like Map_FindObjectCovering, but only objects of the given kind count. */
MapObject* Map_FindObjectOfKindCovering(int x, int y, int kind) {
  MapObject* object = data_027c027c->unk_48[data_027c0284]->objects;

  for (; object->kind != 0; object++) {
    u8 left = object->x;
    u8 top = object->y;
    int right = left + object->width;
    int bottom = top + object->height;

    if (x >= left && y >= top && x < right && y < bottom &&
        object->kind == kind) {
      return object;
    }
  }
  return NULL;
}
