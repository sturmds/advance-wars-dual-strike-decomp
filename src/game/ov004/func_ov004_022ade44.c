#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

void Ov004_InitVramBanks(void);

u32 func_ov004_022ade44(void) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  Ov004_InitVramBanks();
  return state->unk_000;
}
