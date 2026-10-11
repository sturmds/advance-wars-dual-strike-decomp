#include "types.h"

void Vram_ClearAllBankControl(void);
void GX_SetBankForTex(int mask);
void GX_SetBankForBG(int mask);
void func_0210ab74(int mask);
void func_0210aaf8(int mask);
void GX_SetBankForOBJ(int mask);
void GX_SetBankForTexPltt(int mask);
void GX_SetBankForBGExtPltt(int mask);
void func_0210aa6c(int mask);

void func_0202f94c(void) {
    Vram_ClearAllBankControl();
    GX_SetBankForTex(1);
    GX_SetBankForBG(2);
    func_0210ab74(4);
    func_0210aaf8(8);
    GX_SetBankForOBJ(16);
    GX_SetBankForTexPltt(32);
    GX_SetBankForBGExtPltt(64);
    func_0210aa6c(128);
}
