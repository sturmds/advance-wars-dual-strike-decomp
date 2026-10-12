#include "types.h"

void Vram_ClearAllBankControl(void);
void GX_SetBankForTex(int mask);
void GX_SetBankForBG(int mask);
void GX_SetBankForOBJ(int mask);
void GX_SetBankForTexPltt(int mask);
void GX_SetBankForBGExtPltt(int mask);
void GX_SetBankForSubBG(int mask);
void func_0210aaf8(int mask);
void GX_SetBankForSubBGExtPltt(int mask);
void GX_SetBankForSubOBJExtPltt(int mask);

/* Sets up the VRAM banks: one call per bank, each with
 * its bank bit (1 to 0x100). The calls look like the SDK's per-bank VRAM
 * setup, not matched to their SDK names yet. */
void Vram_InitBanks(void)
{
    Vram_ClearAllBankControl();
    GX_SetBankForTex(1);
    GX_SetBankForBG(2);
    GX_SetBankForOBJ(16);
    GX_SetBankForTexPltt(32);
    GX_SetBankForBGExtPltt(64);
    GX_SetBankForSubBG(4);
    func_0210aaf8(8);
    GX_SetBankForSubBGExtPltt(128);
    GX_SetBankForSubOBJExtPltt(256);
}
