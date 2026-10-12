#include "types.h"
#include "game/map_object.h"

/* Two halfwords, copied as a unit (needed to match). */
typedef struct HalfPair {
  u16 first;
  u16 second;
} HalfPair;

/* Removes an object from the list by moving each later entry down one place. */
void MapObject_Remove(MapObject* object) {
  if (object->kind == 0) {
    return;
  }
  do {
    HalfPair* dst = (HalfPair*)object;

    dst[0] = dst[2];
    dst[1] = dst[3];
    object++;
  } while (object->kind != 0);
}
