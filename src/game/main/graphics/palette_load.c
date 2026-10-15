#include "types.h"
#include "game/Unk_02088d04.h"

void Mem_Copy16(const void* src, void* dst,
                u32 size); /* copies in 16-bit units */
void Palette_Flush(void);  /* sends the palette buffer to palette RAM */

/* Copies size bytes of colours into the current screen's palette buffer,
 * offset bytes in (0x200 starts the OBJ palettes), then uploads it. */
void Palette_Load(const void* src, int offset, u32 size) {
  Mem_Copy16(src, (u16*)data_027c0280->paletteBuffer + (offset >> 1), size);
  Palette_Flush();
}
