#include "types.h"
#include "game/ov003_gfx.h"
#include "game/screen_other_map.h"

void Data_Unpack(const void* src, void* dest);
void Palette_Load(const void* src, u32 offset, u32 size);
void* Mem_Alloc(u32 size);
void* data_ov003_02365b10;
void* data_ov003_02365b0c;
u8 data_ov003_02365b14[0x1e0]; /* size: up to the next symbol */

/* Palettes 0x200 and 0x240 are object palettes. Tiles go to object VRAM; pack
 * holds two palettes and two more unpacked blocks. */
void Ov003_LoadObjGraphics(u8* pack, const void* tiles) {
  Data_Unpack(tiles, data_027c0280->objVram + 0xa300);
  Palette_Load(pack, 0x200, 0x40);
  Data_Unpack(pack + 0x40, data_027c0280->objVram + 0xaf00);
  Palette_Load(pack + 0x574, 0x240, 0x20);
  data_ov003_02365b0c = Mem_Alloc(0x2000);
  Data_Unpack(pack + 0xf8, data_ov003_02365b0c);
}
