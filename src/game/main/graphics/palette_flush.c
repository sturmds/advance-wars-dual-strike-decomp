#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void Mem_CopyAuto(const void* src, void* dst, u32 size);
void TransferQueue_Add(const void* src, void* dst, u32 size); /* queued copy */

/* Uploads the palette buffer to palette RAM. With DISPCNT bit 7 (forced blank)
 * set the copy is immediate; otherwise it is queued once until it has run.
 * Colour 0 is set to black first. */
void Palette_Flush(void) {
  *(u16*)data_027c0280->paletteBuffer = 0;
  if (((u32)data_027c0280->dispCnt << 24) >> 31 == 1) {
    Mem_CopyAuto(data_027c0280->paletteBuffer, data_027c0280->paletteCopy,
                 0x400);
    return;
  }
  if (data_027c0280->paletteUploadQueued == 0) {
    TransferQueue_Add(data_027c0280->paletteBuffer, data_027c0280->paletteCopy,
                      0x400);
    data_027c0280->paletteUploadQueued = 1;
  }
}
