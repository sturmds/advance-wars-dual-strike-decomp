#include "types.h"
#include "game/Unk_02080440.h"

int UnitMenu_BoardHidden(void);
int func_020def84(MapUnit* unit, int x, int y);

s8 func_020bf260(void) {
  Unk_02080440_48* state;

  if (UnitMenu_BoardHidden() == 0) {
    return 1;
  }
  state = data_027c027c->unk_48[data_027c0284];
  return func_020def84(state->actingUnit, state->actingUnitDestX,
                       state->actingUnitDestY) != 1;
}
