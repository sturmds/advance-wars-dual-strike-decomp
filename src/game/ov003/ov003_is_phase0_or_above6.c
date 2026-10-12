#include "types.h"
#include "game/ov003_unit.h"

/* 1 when the overlay runs phase 0 or a phase above 6, and byte 0x1d7 of the state is 0. */
int Ov003_IsPhase0OrAbove6(void) {
  Ov003State* state = data_ov003_02364928;

  if (OV003_UNK_1D7(state) != 0) {
    return 0;
  }
  switch (state->phase) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      return 0;
    default:
      return 1;
  }
}
