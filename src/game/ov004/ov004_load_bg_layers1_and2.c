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
extern char data_ov004_022bf104[]; /* layer 1 tiles */
extern char data_ov004_022bf108[]; /* layer 1 map */
extern char data_ov004_022bf10c[]; /* layer 1 palette */
extern char data_ov004_022bf110[]; /* layer 2 tiles */
extern char data_ov004_022bf114[]; /* layer 2 map */
extern char data_ov004_022bf118[]; /* layer 2 palette */
/* map bytes of a text BG by screen size */
const u32 data_ov004_022bdca4[] = {
#include "data/ov004/data_ov004_022bdca4.inc.c"
};
/* map bytes of a rotation/scaling BG by screen size */
const u32 data_ov004_022bdcb4[] = {
#include "data/ov004/data_ov004_022bdcb4.inc.c"
};

/* Size in bytes of the tile map of a layer (as in Gfx_LoadBmap083). */
#define LAYER_MAP_BYTES(layer, i)                                    \
  (Screen_IsTextBg(i)     ? data_ov004_022bdca4[(layer)->screenSize] \
   : Screen_IsAffineBg(i) ? data_ov004_022bdcb4[(layer)->screenSize] \
                          : data_ov004_022bdcb4[(layer)->screenSize] * 2)

void Ov004_LoadBgLayers1And2(void) {
  u16* map;

  FS_ChangeDir(data_ov004_022bf0dc);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14),
      data_ov004_022bf104);
  File_UnpackTo(GFX_LAYER(data_027c0280, 1)->map, data_ov004_022bf108);
  map = GFX_LAYER(data_027c0280, 1)->map;
  TileMap_AddToEntries(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 1), 1),
                       0x1000);
  Palette_LoadFromFile(data_ov004_022bf10c, 0, 32, 32);
  Gfx_RequestLayerCopy(1);

  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
      data_ov004_022bf110);
  File_UnpackTo(GFX_LAYER(data_027c0280, 2)->map, data_ov004_022bf114);
  map = GFX_LAYER(data_027c0280, 2)->map;
  TileMap_AddToEntries(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 2), 2),
                       0x2000);
  Palette_LoadFromFile(data_ov004_022bf118, 0, 64, 256);
  Gfx_RequestLayerCopy(2);
}
