#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

/* For the Laser, volcano, cannons and Deathray whose state is still 0, copies
 * in the 4-bit value stored with the object (startState). */
void MapObjects_InitState(void) {
  MapObject* object = data_027c027c->unk_48[data_027c0284]->objects;

  for (; object->kind != MAP_OBJECT_NONE; object++) {
    switch (object->kind) {
      case MAP_OBJECT_LASER:
      case MAP_OBJECT_VOLCANO:
      case MAP_OBJECT_BLACK_CANNON:
      case MAP_OBJECT_MINICANNON:
      case MAP_OBJECT_DEATHRAY:
        if (object->state == 0) {
          object->state = object->startState;
        }
        break;
    }
  }
}
