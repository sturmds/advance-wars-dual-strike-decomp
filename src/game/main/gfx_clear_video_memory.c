#include "types.h"

void func_0210ac20(int mask);
void func_0210a814(void);
void func_0210f4a0(u32 value, void *dst, u32 size);   /* SDK MI_CpuFill32 */

/* Maps every VRAM bank as LCDC memory, zeroes the 0xa4000 bytes at 0x6800000,
 * restores the bank setup, then clears both screens' OAM (0xc0 words, which
 * should park the sprites below the screen) and palettes. The volatile
 * locals are needed to match. */
void Gfx_ClearVideoMemory(void)
{
    volatile u32 oamFillMain;
    volatile u32 oamFillSub;
    volatile u32 vramFill;
    volatile u32 paletteFillMain;
    volatile u32 paletteFillSub;

    func_0210ac20(0x1ff);
    vramFill = 0;
    func_0210f4a0(vramFill, (void *)0x6800000, 0xa4000);
    func_0210a814();
    oamFillMain = 0xc0;
    func_0210f4a0(oamFillMain, (void *)0x7000000, 0x400);
    paletteFillMain = 0;
    func_0210f4a0(paletteFillMain, (void *)0x5000000, 0x400);
    oamFillSub = 0xc0;
    func_0210f4a0(oamFillSub, (void *)0x7000400, 0x400);
    paletteFillSub = 0;
    func_0210f4a0(paletteFillSub, (void *)0x5000400, 0x400);
}
