#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;
extern u32 gBgTextMapBytes[];
extern u32 gBgAffineMapBytes[];

void Mem_CopyAuto(const void* src, void* dst, u16 size);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);

/* Copies the layer's tile map buffer to its map in VRAM; sizes are as in
 * Gfx_ClearLayerMap. */
void Gfx_CopyLayerMapToVram(int layer) {
  void* vram;
  void* map;
  u32 size;

  map = GFX_LAYERS(data_027c0280)[layer].map;
  vram = (void*)GFX_LAYERS(data_027c0280)[layer].screenBase;

  if (Screen_IsTextBg(layer)) {
    size = gBgTextMapBytes[GFX_LAYERS(data_027c0280)[layer].screenSize];
  } else if (Screen_IsAffineBg(layer)) {
    size = gBgAffineMapBytes[GFX_LAYERS(data_027c0280)[layer].screenSize];
  } else {
    size = gBgAffineMapBytes[GFX_LAYERS(data_027c0280)[layer].screenSize] << 1;
  }
  Mem_CopyAuto(map, vram, size);
}
