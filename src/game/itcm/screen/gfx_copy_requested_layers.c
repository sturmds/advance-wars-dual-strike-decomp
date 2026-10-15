#include "types.h"
#include "game/Unk_02088d04.h"

void Gfx_CopyLayerMapToVram(int layer);

/* Copies the tile maps of the layers marked by Gfx_RequestLayerCopy to VRAM,
 * then clears the requests and the palette upload flag. */
void Gfx_CopyRequestedLayers(void) {
  if (data_027c0280->layerCopyMask & 1) {
    Gfx_CopyLayerMapToVram(0);
  }
  if (data_027c0280->layerCopyMask & 2) {
    Gfx_CopyLayerMapToVram(1);
  }
  if (data_027c0280->layerCopyMask & 4) {
    Gfx_CopyLayerMapToVram(2);
  }
  if (data_027c0280->layerCopyMask & 8) {
    Gfx_CopyLayerMapToVram(3);
  }
  data_027c0280->layerCopyMask = 0;
  data_027c0280->paletteUploadQueued = 0;
}
