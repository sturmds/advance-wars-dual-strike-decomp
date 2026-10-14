#include "types.h"
#include "sdk/gx.h"

u16 gGxVramBankMasks[13];

/* Clears the bank masks and the control register of every VRAM bank. */
void Vram_ClearAllBankControl(void) {
  gGxVramBankMasks[0] = 0;
  gGxVramBankMasks[1] = 0;
  gGxVramBankMasks[2] = 0;
  gGxVramBankMasks[3] = 0;
  gGxVramBankMasks[4] = 0;
  gGxVramBankMasks[5] = 0;
  gGxVramBankMasks[6] = 0;
  gGxVramBankMasks[7] = 0;
  gGxVramBankMasks[8] = 0;
  gGxVramBankMasks[9] = 0;
  gGxVramBankMasks[10] = 0;
  gGxVramBankMasks[11] = 0;
  gGxVramBankMasks[12] = 0;
  *REG_VRAMCNT_ABCD = 0;
  *REG_VRAMCNT_E = 0;
  *REG_VRAMCNT_F = 0;
  *REG_VRAMCNT_G = 0;
  *IO_VRAM_BANKS_H_I_CTRL = 0;
}
