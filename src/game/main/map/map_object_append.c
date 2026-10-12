#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Writes a new object over the list terminator (the first entry with kind 0) and returns it. */
MapObject* MapObject_Append(int x, int y, int width, int height, int kind,
                            int hp) {
  MapObject* object = data_027c027c->unk_48[data_027c0284]->objects;

  while (object->kind != 0) {
    object++;
  }
  object->x = x;
  object->y = y;
  object->width = width;
  object->height = height;
  object->kind = kind;
  object->hp = hp;
  return object;
}
