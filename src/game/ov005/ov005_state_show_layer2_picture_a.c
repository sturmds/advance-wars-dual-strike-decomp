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
extern char data_ov005_0236aa80[]; /* "/rikiishi" */
/* "028": map */
static char data_ov005_0236ab58[4] = {
#include "data/ov005/data_ov005_0236ab58.inc.c"
};
/* "027": tiles, picture 3 */
static char data_ov005_0236ab54[4] = {
#include "data/ov005/data_ov005_0236ab54.inc.c"
};
/* "025": map */
static char data_ov005_0236ab50[4] = {
#include "data/ov005/data_ov005_0236ab50.inc.c"
};
/* "024": tiles, picture 2 */
static char data_ov005_0236ab4c[4] = {
#include "data/ov005/data_ov005_0236ab4c.inc.c"
};
/* "026": palette, pictures 1 and 2 */
static char data_ov005_0236ab48[4] = {
#include "data/ov005/data_ov005_0236ab48.inc.c"
};
/* "023": map */
static char data_ov005_0236ab44[4] = {
#include "data/ov005/data_ov005_0236ab44.inc.c"
};
/* "022": tiles, picture 1 */
static char data_ov005_0236ab40[4] = {
#include "data/ov005/data_ov005_0236ab40.inc.c"
};
/* "021": palette */
static char data_ov005_0236ab3c[4] = {
#include "data/ov005/data_ov005_0236ab3c.inc.c"
};
/* "020": map */
static char data_ov005_0236ab38[4] = {
#include "data/ov005/data_ov005_0236ab38.inc.c"
};
/* "01f": tiles, picture 0 */
static char data_ov005_0236ab34[4] = {
#include "data/ov005/data_ov005_0236ab34.inc.c"
};
/* "029": palette */
static char data_ov005_0236ab5c[4] = {
#include "data/ov005/data_ov005_0236ab5c.inc.c"
};
extern u32 data_ov005_02367fac[]; /* map bytes of a text BG by screen size */
extern u32 data_ov005_02367fbc
    []; /* map bytes of a rotation/scaling BG by screen size */

/* Loads one /rikiishi picture into BG2 of the current screen: tiles, map
 * (palette 6) and palettes 6-15. */
#define LOAD_LAYER2_PICTURE(tiles, mapFile, palette)                          \
  FS_ChangeDir(data_ov005_0236aa80);                                          \
  File_UnpackTo(                                                              \
      data_027c0280->bgVram +                                                 \
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),     \
      tiles);                                                                 \
  File_UnpackTo(data_027c0280->layer2Map, mapFile);                           \
  map = data_027c0280->layer2Map;                                             \
  TileMap_AddToEntries(                                                       \
      map,                                                                    \
      Screen_IsTextBg(2)                                                      \
          ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]      \
      : Screen_IsAffineBg(2)                                                  \
          ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]      \
          : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2, \
      0x6000);                                                                \
  Palette_LoadFromFile(palette, 0, 0xc0, 0x140);                              \
  Gfx_RequestLayerCopy(2)

/* States 29-32 of the state table (data_ov005_0236a74c), arg 0-3 picks the
 * picture (/rikiishi 01f-029). Same as Ov005State_ShowLayer2Picture with
 * another set of pictures: loads it on both screens, scrolls layers 2 and 3
 * to y = -28, then shows BG2 and OBJ on the main screen and only BG2 on the
 * sub screen. */
void Ov005State_ShowLayer2PictureA(Unk_ov005_0236bba4* work, int picture) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u16* map;

  switch (picture) {
    case 0:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab34, data_ov005_0236ab38,
                          data_ov005_0236ab3c);
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab34, data_ov005_0236ab38,
                          data_ov005_0236ab3c);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
    case 1:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab40, data_ov005_0236ab44,
                          data_ov005_0236ab48);
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab40, data_ov005_0236ab44,
                          data_ov005_0236ab48);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
    case 2:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab4c, data_ov005_0236ab50,
                          data_ov005_0236ab48);
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab4c, data_ov005_0236ab50,
                          data_ov005_0236ab48);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
    case 3:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab54, data_ov005_0236ab58,
                          data_ov005_0236ab5c);
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab54, data_ov005_0236ab58,
                          data_ov005_0236ab5c);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
  }
  work->scrollY = -0x1c000;

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
