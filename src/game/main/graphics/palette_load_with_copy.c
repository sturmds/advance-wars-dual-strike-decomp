#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void Mem_Copy16(const void* src, void* dst,
                u32 size); /* copies in 16-bit units */

/* Copies size bytes of colours, offset bytes in, to both the palette buffer
 * and its copy. Unlike Palette_Load it does not upload anything. */
void Palette_LoadWithCopy(const void* src, int offset, u32 size) {
  Mem_Copy16(src, (u16*)data_027c0280->paletteBuffer + (offset >> 1), size);
  Mem_Copy16(src, data_027c0280->paletteCopy + offset, size);
}
