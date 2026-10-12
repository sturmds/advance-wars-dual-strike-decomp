#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int UnitMenu_BoardHidden(void);
int func_020def74(MapUnit* unit, int x, int y);

/* Purpose unknown. Returns 1 when UnitMenu_BoardHidden is 0; otherwise 1 unless
 * func_020def74 returns 1 for the acting unit and its destination tile. */
s8 func_020bf1f0(void) {
  Unk_02080440_48* state;

  if (UnitMenu_BoardHidden() == 0) {
    return 1;
  }
  state = data_027c027c->unk_48[data_027c0284];
  return func_020def74(state->actingUnit, state->actingUnitDestX,
                       state->actingUnitDestY) != 1;
}
