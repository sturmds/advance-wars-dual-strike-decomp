#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void File_ReadAt(void* dest, const char* path, u32 offset,
                 u32 size); /* reads size bytes at offset of a file */
void Palette_Flush(void);   /* sends the palette buffer to palette RAM */

/* Reads size bytes of colours at filePos of a file into the current screen's
 * palette buffer, paletteOffset bytes in, then uploads it. */
void Palette_LoadFromFile(const char* path, u32 filePos, int paletteOffset,
                          u32 size) {
  File_ReadAt((u16*)data_027c0280->paletteBuffer + (paletteOffset >> 1), path,
              filePos, size);
  Palette_Flush();
}
