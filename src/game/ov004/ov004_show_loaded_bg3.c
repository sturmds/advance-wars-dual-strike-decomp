#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern void* data_ov004_022bf150;

void Mem_Copy32(const void* src, void* dst, u32 size);
void Gfx_RequestLayerCopy(int layer);

/* Copies the 64 KB tile buffer into BG3's character area, alternating between
 * its first and second half (bg3InSecondHalf is 1 after the first half), and shows
 * only BG3. */
void Ov004_ShowLoadedBg3(void) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;

  if (state->bg3InSecondHalf == 0) {
    Mem_Copy32(
        data_ov004_022bf150,
        data_027c0280->bgVram +
            data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase * 0x4000,
        OV004_BG3_BUFFER_SIZE);
    state->bgCharBase = 0;
    state->bg3InSecondHalf = 1;
  } else {
    Mem_Copy32(
        data_ov004_022bf150,
        data_027c0280->bgVram +
            data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase * 0x4000 +
            OV004_BG3_BUFFER_SIZE,
        OV004_BG3_BUFFER_SIZE);
    state->bgCharBase = 1;
    state->bg3InSecondHalf = 0;
  }
  Gfx_RequestLayerCopy(3);
  GFX_DISPCNT(data_027c0280)->bg0 = 0;
  GFX_DISPCNT(data_027c0280)->bg1 = 0;
  GFX_DISPCNT(data_027c0280)->bg2 = 0;
  GFX_DISPCNT(data_027c0280)->bg3 = 1;
  GFX_DISPCNT(data_027c0280)->obj = 0;
}
