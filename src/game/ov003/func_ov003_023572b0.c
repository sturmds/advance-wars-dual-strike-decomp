#include "types.h"
#include "game/ov003_unit.h"

int Ov003_IsPhase0Or1(void);

int func_ov003_023572b0(void) {
  if (Ov003_IsPhase0Or1() == 0) {
    return 0;
  }
  return data_ov003_02364928->slots[data_ov003_02364928->currentSlot].state ==
         2;
}
