#include "types.h"
#include "sdk/gx.h"

void func_0210b1b8(int mask);
void func_0210ab74(int mask);
void func_0210aaf8(int mask);
void func_0210b04c(int mask);
void func_0210af38(int mask);
void func_0210aa6c(int mask);

void func_ov004_022bb9c4(void)
{
    Vram_ClearAllBankControl();
    *REG_POWCNT1 &= ~POWCNT1_DISPLAY_SWAP;
    GX_SetBankForTex(1);
    func_0210b1b8(2);
    func_0210ab74(4);
    func_0210aaf8(8);
    func_0210b04c(16);
    GX_SetBankForTexPltt(32);
    func_0210af38(64);
    func_0210aa6c(128);
}
