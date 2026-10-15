#include "types.h"
#include "game/Unk_02088d04.h"

/* Zeroes the first tile (32 bytes, or 64 in 256-colour mode) at the layer's
 * character base, so map entries that use tile 0 show nothing. The base is
 * a VRAM address by the time this runs. */
void Gfx_ClearLayerBlankTile(int layer) {
  u16* tiles;
  u32 size;
  int count;
  int i;

  if (GFX_LAYERS(data_027c0280)[layer].mode == GFX_LAYER_NO_COPY) {
    return;
  }
  tiles = (u16*)GFX_LAYERS(data_027c0280)[layer].charBase;
  if (data_027c0280->bgCnt[GFX_LAYERS(data_027c0280)[layer].bg].colorMode ==
      0) {
    size = 32;
  } else {
    size = 64;
  }
  count = size >> 1;
  for (i = 0; i < count; i++) {
    *tiles++ = 0;
  }
}
