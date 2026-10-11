#include "types.h"

void Vram_ClearAllBankControl(void);
void GX_SetBankForTex(int mask);
void func_0210b1b8(int mask);
void func_0210b04c(int mask);
void GX_SetBankForTexPltt(int mask);
void func_0210af38(int mask);
void func_0210ab74(int mask);
void func_0210aaf8(int mask);
void func_0210aa6c(int mask);
void func_0210a9e0(int mask);

/* Sets up the VRAM banks: one call per bank, each with
 * its bank bit (1 to 0x100). The calls look like the SDK's per-bank VRAM
 * setup, not matched to their SDK names yet. */
void Vram_InitBanks(void)
{
    Vram_ClearAllBankControl();
    GX_SetBankForTex(1);
    func_0210b1b8(2);
    func_0210b04c(16);
    GX_SetBankForTexPltt(32);
    func_0210af38(64);
    func_0210ab74(4);
    func_0210aaf8(8);
    func_0210aa6c(128);
    func_0210a9e0(256);
}
