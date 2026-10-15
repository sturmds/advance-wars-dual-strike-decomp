#include "types.h"
#include "game/Unk_02088d04.h"

extern u8* data_02297790;

void BgControl_SetCharBase(BgControl* bg, u32 charOffset);

/* Sets where layer 0-3's tile data starts in BG VRAM (a byte offset) and
 * writes it to the layer's BG control register. As in EventScript_Init,
 * layer 0's data address is kept in data_02297790 when it is in main-engine
 * BG VRAM. */
void Screen_SetLayerCharBase(int layer, u32 charOffset) {
  GfxLayer* l = GFX_LAYER(data_027c0280, layer);
  u8* vram;

  l->charBase = charOffset;
  BgControl_SetCharBase(
      &data_027c0280->bgCnt[GFX_LAYER(data_027c0280, layer)->bg], l->charBase);
  vram = data_027c0280->bgVram +
         (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14);
  if ((u32)vram < 0x6200000 && layer == 0) {
    data_02297790 = vram;
  }
}
