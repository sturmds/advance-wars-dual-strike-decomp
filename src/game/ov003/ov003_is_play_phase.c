#include "types.h"
#include "game/ov003_unit.h"

/* 1 while the overlay runs phase 0 (play) and byte 0x1d7 of the state is 0. */
int Ov003_IsPlayPhase(void) {
  Ov003State* state = data_ov003_02364928;

  if (OV003_UNK_1D7(state) != 0) {
    return 0;
  }
  /* needed to match: a switch, not an if */
  switch (state->phase) {
    case 0:
      return 1;
    default:
      return 0;
  }
}
