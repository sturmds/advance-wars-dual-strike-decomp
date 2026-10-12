#include "types.h"
#include "game/Unk_02080440.h"

void UnitShowTask_StartPath(u8* nibbles);
void Unit_SubtractMoveFuel(MapUnit* unit, int moveCost);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Passes the move's nibbles on to an overlay 0 task (UnitShowTask_StartPath),
 * then, when the map has fog or the weather is rain, takes the fuel the
 * acting unit's move costs off the unit. AiShowAction_Step1 calls it. Rain
 * also brings fog of war, so both cases are moves the AI makes out of sight. */
void func_020c8290(u8* nibbles) {
  UnitShowTask_StartPath(nibbles);
  if (STATE->unk_1c4->fog == 0 && STATE->unk_1c4->weather != WEATHER_RAIN) {
    return;
  }
  Unit_SubtractMoveFuel(STATE->actingUnit, STATE->unk_46);
}
