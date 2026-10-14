#include "types.h"
#include "sdk/gx.h"

void func_0210ac20(int mask);
void func_0210a814(void);
void MIi_CpuClearFast(u32 value, void* dst, u32 size); /* SDK MI_CpuFill32 */

/* Maps every VRAM bank as LCDC memory, zeroes the LCDC_VRAM_SIZE bytes at LCDC_VRAM,
 * restores the bank setup, then clears both screens' OAM (0xc0 words, which
 * should park the sprites below the screen) and palettes. The volatile
 * locals are needed to match. */
void Gfx_ClearVideoMemory(void) {
  volatile u32 oamFillMain;
  volatile u32 oamFillSub;
  volatile u32 vramFill;
  volatile u32 paletteFillMain;
  volatile u32 paletteFillSub;

  func_0210ac20(0x1ff);
  vramFill = 0;
  MIi_CpuClearFast(vramFill, (void*)LCDC_VRAM, LCDC_VRAM_SIZE);
  func_0210a814();
  oamFillMain = 0xc0;
  MIi_CpuClearFast(oamFillMain, (void*)OAM_RAM, OAM_RAM_SIZE);
  paletteFillMain = 0;
  MIi_CpuClearFast(paletteFillMain, (void*)PALETTE_RAM, PALETTE_RAM_SIZE);
  oamFillSub = 0xc0;
  MIi_CpuClearFast(oamFillSub, (void*)(OAM_RAM + OAM_RAM_SIZE), OAM_RAM_SIZE);
  paletteFillSub = 0;
  MIi_CpuClearFast(paletteFillSub, (void*)(PALETTE_RAM + PALETTE_RAM_SIZE),
                   PALETTE_RAM_SIZE);
}
