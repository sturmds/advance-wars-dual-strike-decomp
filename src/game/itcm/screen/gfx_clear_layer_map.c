#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"

/* Map size in bytes by screen size (BGxCNT bits 14-15). */
extern u32 gBgTextMapBytes[];
extern u32 gBgAffineMapBytes[];

BOOL Screen_IsTextBg(int layer);

/* Zeroes the layer's tile map buffer. Affine maps use one byte per entry;
 * the other modes use two, so their size is double the affine table's. */
void Gfx_ClearLayerMap(int layer) {
  u16* map;
  int size;
  int count;
  int i;

  if (GFX_LAYERS(data_027c0280)[layer].mode == GFX_LAYER_NO_COPY) {
    return;
  }
  map = GFX_LAYERS(data_027c0280)[layer].map;
  if (Screen_IsTextBg(layer)) {
    size = gBgTextMapBytes[GFX_LAYERS(data_027c0280)[layer].screenSize];
  } else if (Screen_IsAffineBg(layer)) {
    size = gBgAffineMapBytes[GFX_LAYERS(data_027c0280)[layer].screenSize];
  } else {
    size = gBgAffineMapBytes[GFX_LAYERS(data_027c0280)[layer].screenSize] << 1;
  }
  count = size >> 1;
  for (i = 0; i < count; i++) {
    *map++ = 0;
  }
}
