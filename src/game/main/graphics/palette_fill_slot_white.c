#include "types.h"
#include "game/Unk_02088d04.h"

void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits);

extern const u16 gWhitePalette[];

/* Sets the 16 colours of palette slot slot (0-31, BG then OBJ) in the palette
 * buffer to white. The shift form of the offset is needed to match. */
void Palette_FillSlotWhite(int slot) {
  Mem_CopyUnits(gWhitePalette,
                (u16*)data_027c0280->paletteBuffer + ((slot << 5) >> 1),
                PALETTE_16_SIZE, 16);
}
