#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* For objects of kinds 1 to 5 whose unk_06 is still 0, copies in the 4-bit
 * value stored with the object (startState). */
void MapObjects_InitState(void) {
  MapObject* object = data_027c027c->unk_48[data_027c0284]->objects;

  for (; object->kind != 0; object++) {
    switch (object->kind) {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
        if (object->state == 0) {
          object->state = object->startState;
        }
        break;
    }
  }
}
