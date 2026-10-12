#include "types.h"

extern u16 gObjRowPalette[16];

void Palette_Load(const void* src, int offset, u32 size);

/* Loads a 16-colour palette into row `row` of the OBJ palettes (the buffer
 * offset 0x200 starts them, 32 bytes per row). */
void Palette_LoadObjRow(int row) {
  Palette_Load(gObjRowPalette, ((u32)(row + 16) << 21) >> 16, 32);
}
