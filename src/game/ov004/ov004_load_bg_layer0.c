#include "types.h"
#include "game/Unk_02088d04.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280* data_027c0280;
extern char data_ov004_022bf0dc[]; /* directory */
extern char data_ov004_022bf11c[]; /* layer 0 tiles */
extern char data_ov004_022bf120[]; /* layer 0 map */
extern char data_ov004_022bf124[]; /* layer 0 palette */
extern u32 data_ov004_022bdca4[];  /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdcb4
    []; /* map bytes of a rotation/scaling BG by screen size */

#define LAYER_MAP_BYTES(layer, i)                                    \
  (Screen_IsTextBg(i)     ? data_ov004_022bdca4[(layer)->screenSize] \
   : Screen_IsAffineBg(i) ? data_ov004_022bdcb4[(layer)->screenSize] \
                          : data_ov004_022bdcb4[(layer)->screenSize] * 2)

/* Layer 0 version of Ov004_LoadBgLayers1And2: tiles, map and palette 0. */
void Ov004_LoadBgLayer0(void) {
  u16* map;

  FS_ChangeDir(data_ov004_022bf0dc);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
      data_ov004_022bf11c);
  File_UnpackTo(GFX_LAYER(data_027c0280, 0)->map, data_ov004_022bf120);
  map = GFX_LAYER(data_027c0280, 0)->map;
  TileMap_AddToEntries(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 0), 0), 0);
  Palette_LoadFromFile(data_ov004_022bf124, 0, 0, 32);
  Gfx_RequestLayerCopy(0);
}
