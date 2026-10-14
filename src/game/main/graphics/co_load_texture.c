#include "types.h"
#include "game/co_graphics.h"
#include "sdk/os.h"
#include "game/co_data.h"

void* Mem_Alloc(u32 size);
void Mem_Free(void* ptr);
int Co_GetPictureVariant(int coId);
int Co_GetSavedPaletteVariant(int coId);
void Gfx_LoadTexPalette(const void* src, u32 offset, u32 size);
void Gfx_LoadTexImage(const void* src, u32 offset, u32 size);
void Image4bpp_UntileStrip(const u8* src, u8* dest, int tiles);

/* Picture tile pointers at gCoGraphics + 0x40, one row per CO. */
extern const u8* const data_02152b78[][sizeof(CoGraphics) / sizeof(void*)];

/* Loads the picture of a CO as a texture. The 12 tiles of the stored
 * variant are rearranged into a row-major image (strips of 4, 4, 2 and 2
 * tiles), copied to texOffset, and the CO's palette goes to paletteOffset. */
void Co_LoadTexture(int coId, u32 texOffset, u32 paletteOffset) {
  u8* image = Mem_Alloc(0x180);
  int co = coId & CO_ID_MASK;
  const u8* tiles = data_02152b78[co][Co_GetPictureVariant(coId)];

  Image4bpp_UntileStrip(tiles, image, 4);
  Image4bpp_UntileStrip(tiles + 0x80, image + 0x80, 4);
  Image4bpp_UntileStrip(tiles + 0x100, image + 0x100, 2);
  Image4bpp_UntileStrip(tiles + 0x140, image + 0x140, 2);
  DC_FlushRange(image, 0x180);
  Gfx_LoadTexImage(image, texOffset, 0x180);
  Gfx_LoadTexPalette(
      gCoGraphics[co].palettes + Co_GetSavedPaletteVariant(co) * 32,
      paletteOffset, 32);
  Mem_Free(image);
}
