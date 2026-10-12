#include "types.h"

void GX_SetBankForTex(int mask);
void GX_SetBankForBG(int mask);
void GX_SetBankForOBJ(int mask);
void GX_SetBankForTexPltt(int mask);
void GX_SetBankForBGExtPltt(int mask);
void func_0210ab74(int mask);
void func_0210aaf8(int mask);
void func_0210aa6c(int mask);
void GX_SetBankForSubOBJExtPltt(int mask);

/* Calls the nine per-bank setup functions (guess), each with one VRAM bank
 * bit: A=1, B=2, C=4, D=8, E=0x10, F=0x20, G=0x40, H=0x80, I=0x100. */
void Vram_AssignAllBanks(void)
{
    GX_SetBankForTex(1);
    GX_SetBankForBG(2);
    GX_SetBankForOBJ(0x10);
    GX_SetBankForTexPltt(0x20);
    GX_SetBankForBGExtPltt(0x40);
    func_0210ab74(4);
    func_0210aaf8(8);
    func_0210aa6c(0x80);
    GX_SetBankForSubOBJExtPltt(0x100);
}
