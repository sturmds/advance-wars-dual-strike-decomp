#include "types.h"
#include "game/Unk_02088d04.h"

void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);

extern Unk_027c0280* data_027c0280;
extern GfxLayer data_02169ef8[]; /* layer config for Screen_InitLayers */
extern char data_0216a1a0[];     /* "/bmap" */
extern char data_0216a1e0[];     /* "09c": layer 3 tiles */
extern char data_0216a1e4[];     /* "09b": layer 3 map */
extern char data_0216a1e8[];     /* "09d": layer 3 palette */
extern char data_0216a1ec[];     /* "098": layer 0 tiles */
extern char data_0216a1f0[];     /* "099": layer 0 map */
extern char data_0216a1f4[];     /* "09a": layer 0 palette */
extern char data_0216a1f8[];     /* "092": layer 2 tiles */
extern char data_0216a1fc[];     /* "094": layer 2 map */
extern char data_0216a200[];     /* "093": layer 2 palette */
extern u32 data_0214d3b8[];      /* map bytes of a text BG by screen size */
extern u32
    data_0214d3a8[]; /* map bytes of a rotation/scaling BG by screen size */

/* Map bytes of layer i, by its BG kind and screen size. */
#define LAYER_MAP_BYTES(i)                                                     \
  (Screen_IsTextBg(i) ? data_0214d3b8[GFX_LAYER(data_027c0280, i)->screenSize] \
   : Screen_IsAffineBg(i)                                                      \
       ? data_0214d3a8[GFX_LAYER(data_027c0280, i)->screenSize]                \
       : data_0214d3a8[GFX_LAYER(data_027c0280, i)->screenSize] * 2)

/* Sets up BG mode 0 and the layers for the map screen, then loads tiles,
 * tile map and palette of layers 3, 0 and 2 from /bmap. Layer 3 gets its
 * map twice (both screen blocks), with palette 3 and characters from
 * 0x100; layer 0 palette 2 from character 0x108; layer 2 palette 4. */
void MapBg_LoadGraphics(void) {
  u16* map;

  Screen_SetBgMode(0);
  Screen_InitLayers(data_02169ef8);
  FS_ChangeDir(data_0216a1a0);

  File_UnpackTo(data_027c0280->unk_120 + 0x2000, data_0216a1e0);
  File_UnpackTo(data_027c0280->layer3Map, data_0216a1e4);
  File_UnpackTo(data_027c0280->layer3Map + 0x800, data_0216a1e4);
  map = data_027c0280->layer3Map;
  TileMap_AddToEntries(map, LAYER_MAP_BYTES(3), 0x3100);
  Palette_LoadFromFile(data_0216a1e8, 0, 0x60, 0x20);

  File_UnpackTo(data_027c0280->unk_e4 + 0x2100, data_0216a1ec);
  File_UnpackTo(data_027c0280->layer0Map, data_0216a1f0);
  map = data_027c0280->layer0Map;
  TileMap_AddToEntries(map, LAYER_MAP_BYTES(0), 0x2108);
  Palette_LoadFromFile(data_0216a1f4, 0, 0x40, 0x20);

  File_UnpackTo(data_027c0280->unk_10c, data_0216a1f8);
  File_UnpackTo(data_027c0280->layer2Map, data_0216a1fc);
  map = data_027c0280->layer2Map;
  TileMap_AddToEntries(map, LAYER_MAP_BYTES(2), 0x4000);
  Palette_LoadFromFile(data_0216a200, 0, 0x80, 0xc0);

  Gfx_RequestLayerCopy(3);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(2);
}
