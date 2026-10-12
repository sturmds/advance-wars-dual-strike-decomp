#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* FALSE when the map has exactly three objects of kind 9. Name kept: no caller found. */
BOOL func_ov001_02351c58(void) {
  MapObject* object = STATE->objects;
  int count = 0;

  while (object->kind != 0) {
    if (object->kind == 9) {
      count++;
    }
    object++;
  }
  return count != 3;
}
