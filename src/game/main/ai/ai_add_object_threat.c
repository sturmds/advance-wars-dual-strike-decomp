#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

void func_01ffb200(void *tileMap);
BOOL func_0209dae4(int unitType);
void func_020da068(MapObject *object, int value);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* With objects attackable (AiTargetFlags) and a unit type that cares
 * (func_0209dae4), points the range map at threatSum and hands each active
 * object (unk_06 is 1) to func_020da068 with -1. */
void Ai_AddObjectThreat(void)
{
    MapObject *object;

    if (!AI_TARGET_FLAGS->attackObjects) {
        return;
    }
    if (!func_0209dae4(STATE->actingUnit->type)) {
        return;
    }
    func_01ffb200(STATE->mapTiles->threatSum);
    for (object = STATE->objects; object->kind != 0; object++) {
        if (object->unk_06 == 1) {
            func_020da068(object, -1);
        }
    }
    func_01ffb200(STATE->mapTiles->unk_2840);
}
