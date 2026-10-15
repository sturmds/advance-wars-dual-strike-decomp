#include "types.h"
#include "sdk/gx.h"

void GX_SetBankForSubBG(int mask);
void func_0210aaf8(int mask);
void GX_SetBankForSubBGExtPltt(int mask);

void func_0202f94c(void) {
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
