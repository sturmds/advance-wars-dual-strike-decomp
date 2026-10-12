#include "types.h"
#include "game/Unk_02088d04.h"

void* File_Load(const char* path);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, u32 offset, u32 size);
void Mem_Free(void* file);

extern const char data_021598b0[];
extern Unk_027c0280* data_027c0280;

#define MODE_FONT_TILES_OFFSET 0x954
#define MODE_FONT_PALETTE0_OFFSET 0xff4
#define MODE_FONT_PALETTE1_OFFSET 0x1014

/* Loads /ohashi/res_modefont into the current screen: its tiles to OBJ
 * tiles tile and tile + 0xe0, its two palettes to OBJ palettes
 * paletteSlot and paletteSlot + 1 (OBJ palettes start at 16). */
void ModeFont_LoadHere(int tile, int paletteSlot) {
  u8* file = File_Load(data_021598b0);

  Data_Unpack(file, data_027c0280->objVram + tile * 32);
  Data_Unpack(file + MODE_FONT_TILES_OFFSET,
              data_027c0280->objVram + (tile + 0xe0) * 32);
  Palette_Load(file + MODE_FONT_PALETTE0_OFFSET,
               ((u32)(paletteSlot + 16) << 21) >> 16, 32);
  Palette_Load(file + MODE_FONT_PALETTE1_OFFSET,
               ((u32)(paletteSlot + 17) << 21) >> 16, 32);
  Mem_Free(file);
}
