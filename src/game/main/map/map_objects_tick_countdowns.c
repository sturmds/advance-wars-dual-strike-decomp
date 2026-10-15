#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/player.h"

/* Step of the map object turn script, before MapObjectTurnTask_TargetNext,
 * which lets an object act when its state count is 0. On player 1's turn a
 * volcano counts down; once at 0 it restarts from its stored value
 * (startState) less one, except on day 0. The Laser, cannons and
 * Deathray count down on Black Hole's turn, with no stop at 0. */
void MapObjects_TickCountdowns(void) {
  MapObject* object = MAP_STATE->objects;

  for (; object->kind != MAP_OBJECT_NONE; object++) {
    switch (object->kind) {
      case MAP_OBJECT_VOLCANO:
        if (MAP_STATE->currentPlayer == 1) {
          if (object->state != 0) {
            object->state--;
          } else if (MAP_STATE->day != 0) {
            object->state = object->startState;
            object->state--;
          }
        }
        break;
      case MAP_OBJECT_LASER:
      case MAP_OBJECT_BLACK_CANNON:
      case MAP_OBJECT_MINICANNON:
      case MAP_OBJECT_DEATHRAY:
        if (MAP_STATE->players[MAP_STATE->currentPlayer].armyColour ==
            ARMY_BLACK_HOLE) {
          object->state--;
        }
        break;
    }
  }
}
