#include "types.h"
#include "game/owner_info.h"
#include "game/Unk_02088d04.h"

void Data_Unpack(const void* src, void* dest);
void Palette_Load(const void* src, u32 offset, u32 size);

extern Unk_027c0280* data_027c0280;
extern u8* data_ov000_022f6570[]; /* per language: packed OBJ tiles */
extern u8*
    data_ov000_022f65d4[]; /* per language: a second set of packed OBJ tiles */
extern u8 data_02139208[];

/* Unpacks the language's two OBJ tile sets to tiles 0x2be and 0x31d of OBJ
 * VRAM and loads a palette to OBJ palette 1. The address cast needed to match:
 * the original adds 0x40 at run time. */
void ObjGfx_LoadLanguageTiles(void) {
  Data_Unpack(data_ov000_022f6570[data_ov000_023503a0],
              data_027c0280->objVram + 0x57c0);
  Palette_Load((u8*)((u32)data_02139208 + 0x40), 0x220, 0x20);
  Data_Unpack(data_ov000_022f65d4[data_ov000_023503a0],
              data_027c0280->objVram + 0x63a0);
}
