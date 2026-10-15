#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

#define HEAD_OBJECT_COUNT 16

/* Zeroes the first HEAD_OBJECT_COUNT object entries except the variant. The chained assignments are needed to match. */
void MapObjects_ClearHead(void) {
  MapObject* object = data_027c027c->unk_48[data_027c0284]->objects;
  int i;

  for (i = 0; i < HEAD_OBJECT_COUNT; i++, object++) {
    object->height = 0;
    object->width = object->height;
    object->y = object->width;
    object->x = object->y;
    object->kind = 0;
    object->hp = 0;
    object->unk_05 = 0;
    object->startState = 0;
    object->state = 0;
    object->unk_07 = 0;
  }
}
