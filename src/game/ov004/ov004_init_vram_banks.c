#include "types.h"
#include "sdk/gx.h"

void func_0210ac20(int mask);
void GX_SetBankForSubBG(int mask);
void func_0210aaf8(int mask);

/* Puts the main engine on the top screen, then calls what look like per-bank
 * VRAM setup functions with bank bit masks (guess). */
void Ov004_InitVramBanks(void) {
  Vram_ClearAllBankControl();
  *REG_POWCNT1 |= POWCNT1_DISPLAY_SWAP;
  GX_SetBankForTex(1);
  GX_SetBankForBG(6);
  func_0210ac20(8);
  GX_SetBankForOBJ(16);
  GX_SetBankForTexPltt(32);
  GX_SetBankForBGExtPltt(64);
  GX_SetBankForSubBG(128);
  func_0210aaf8(256);
}
