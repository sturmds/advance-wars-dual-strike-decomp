#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Step of the map object turn script, before MapObjectTurnTask_TargetNext,
 * which lets an object act when its state count is 0. On player 1's turn a
 * volcano counts down; once at 0 it restarts from its stored value
 * (startState) less one, except on day 0. Objects of kinds 1, 3, 4 and 5
 * count down on Black Hole's turn, with no stop at 0. */
void MapObjects_TickCountdowns(void)
{
    MapObject *object = STATE->objects;

    for (; object->kind != 0; object++) {
        switch (object->kind) {
        case MAP_OBJECT_VOLCANO:
            if (STATE->currentPlayer == 1) {
                if (object->state != 0) {
                    object->state--;
                } else if (STATE->day != 0) {
                    object->state = object->startState;
                    object->state--;
                }
            }
            break;
        case 1:
        case MAP_OBJECT_BLACK_CANNON:
        case MAP_OBJECT_MINICANNON:
        case MAP_OBJECT_DEATHRAY:
            if (STATE->players[STATE->currentPlayer].armyColour == ARMY_BLACK_HOLE) {
                object->state--;
            }
            break;
        }
    }
}
