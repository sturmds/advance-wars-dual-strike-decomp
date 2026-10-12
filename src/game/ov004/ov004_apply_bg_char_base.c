#include "types.h"
#include "sdk/gx.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;

void Ov004_ApplyBgCharBase(void) {
  int offset = data_ov004_022bf158->bgCharBase;

  *REG_DISPCNT = (offset << 24) | (*REG_DISPCNT & ~0x7000000);
}
