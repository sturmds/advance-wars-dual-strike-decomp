#include "types.h"
#include "game/screen_other_map.h"

void* Ov004_GetResourceData(const char* name);
void Data_Unpack(const void* src, void* dst);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);

extern void* data_ov004_022bf150;
extern u32 data_ov004_022bdc94[]; /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84
    []; /* map bytes of a rotation/scaling BG by screen size */

/* Unpacks BG3's tiles into the overlay's tile buffer and its map into the
 * layer's map buffer, then sets the palette number (bits 12-15) of every map
 * entry. */
void Ov004_LoadBg3FromResource(const char* name, int tilesOffset, int mapOffset,
                               int palette) {
  u16* map;
  int mapBytes;

  Data_Unpack((u8*)Ov004_GetResourceData(name) + tilesOffset,
              data_ov004_022bf150);
  Data_Unpack((u8*)Ov004_GetResourceData(name) + mapOffset,
              GFX_LAYER(data_027c0280, 3)->map);
  map = GFX_LAYER(data_027c0280, 3)->map;
  if (Screen_IsTextBg(3)) {
    mapBytes = data_ov004_022bdc94[GFX_LAYER(data_027c0280, 3)->screenSize];
  } else if (Screen_IsAffineBg(3)) {
    mapBytes = data_ov004_022bdc84[GFX_LAYER(data_027c0280, 3)->screenSize];
  } else {
    mapBytes = data_ov004_022bdc84[GFX_LAYER(data_027c0280, 3)->screenSize] * 2;
  }
  TileMap_AddToEntries(map, mapBytes, ((u32)palette << 28) >> 16);
}
