#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Nonzero unless the map has exactly four objects of kind 4. */
BOOL Ov001_IsKind4ObjectCountNot4(void) {
  MapObject* object;
  int count = 0;

  for (object = STATE->objects; object->kind != 0; object++) {
    if (object->kind == 4) {
      count++;
    }
  }
  return count != 4;
}
