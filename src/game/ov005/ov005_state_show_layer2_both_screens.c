#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0236bba4.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_ov005_0236aa80[]; /* directory of the files below */
/* layer 2 map */
static char data_ov005_0236ab64[] = {
#include "data/ov005/data_ov005_0236ab64.inc.c"
};
/* layer 2 tiles */
static char data_ov005_0236ab60[] = {
#include "data/ov005/data_ov005_0236ab60.inc.c"
};
/* layer 2 palette */
static char data_ov005_0236ab68[] = {
#include "data/ov005/data_ov005_0236ab68.inc.c"
};
extern u32 data_ov005_02367fac[]; /* map bytes of a text BG by screen size */
extern u32 data_ov005_02367fbc
    []; /* map bytes of a rotation/scaling BG by screen size */

/* State 28 of the state table (data_ov005_0236a74c; work and arg unused):
 * loads the same layer 2 tiles, map (palette 8) and palette on both screens,
 * then shows BG2 and OBJ on the main screen and only BG2 on the sub screen. */
void Ov005State_ShowLayer2BothScreens(Unk_ov005_0236bba4* work, int arg) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;
  u16* map;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  FS_ChangeDir(data_ov005_0236aa80);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
      data_ov005_0236ab60);
  File_UnpackTo(data_027c0280->layer2Map, data_ov005_0236ab64);
  map = data_027c0280->layer2Map;
  TileMap_AddToEntries(
      map,
      Screen_IsTextBg(2)
          ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]
      : Screen_IsAffineBg(2)
          ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]
          : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
      0x8000);
  Palette_LoadFromFile(data_ov005_0236ab68, 0, 0x100, 0x100);
  Gfx_RequestLayerCopy(2);

  Screen_SetCurrentBoth(data_027c0288.unk_04);
  FS_ChangeDir(data_ov005_0236aa80);
  File_UnpackTo(
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
      data_ov005_0236ab60);
  File_UnpackTo(data_027c0280->layer2Map, data_ov005_0236ab64);
  map = data_027c0280->layer2Map;
  TileMap_AddToEntries(
      map,
      Screen_IsTextBg(2)
          ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]
      : Screen_IsAffineBg(2)
          ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]
          : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
      0x8000);
  Palette_LoadFromFile(data_ov005_0236ab68, 0, 0x100, 0x100);
  Gfx_RequestLayerCopy(2);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt |= 0x1000;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt &= ~0x1000;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
