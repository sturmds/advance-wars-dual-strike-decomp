#include "types.h"
#include "sdk/gx.h"

void GX_SetBankForSubBG(int mask);
void func_0210aaf8(int mask);
void GX_SetBankForSubBGExtPltt(int mask);

/* Like Ov004_InitVramBanks, but without the screen swap: each of the eight
 * per-bank setup calls (guess) gets one bank bit, 1 to 128 in order. */
void Ov004_InitVramBanksInOrder(void) {
  Vram_ClearAllBankControl();
  GX_SetBankForTex(1);
  GX_SetBankForBG(2);
  GX_SetBankForSubBG(4);
  func_0210aaf8(8);
  GX_SetBankForOBJ(16);
  GX_SetBankForTexPltt(32);
  GX_SetBankForBGExtPltt(64);
  GX_SetBankForSubBGExtPltt(128);
}
