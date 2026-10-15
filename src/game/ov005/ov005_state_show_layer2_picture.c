#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0236bba4.h"
#include "game/screen_other_map.h"
#include "sdk/fs.h"

void Screen_SetCurrentBoth(void* state);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
int Screen_IsAffineBg(int bg);
void Gfx_RequestLayerCopy(int layer);

extern char data_ov005_0236aa80[]; /* "/rikiishi" */
/* "037": map */
static char data_ov005_0236ab2c[] = {
#include "data/ov005/data_ov005_0236ab2c.inc.c"
};
/* "036": tiles, picture 4 */
static char data_ov005_0236ab28[] = {
#include "data/ov005/data_ov005_0236ab28.inc.c"
};
/* "035": palette */
static char data_ov005_0236ab24[] = {
#include "data/ov005/data_ov005_0236ab24.inc.c"
};
/* "034": map */
static char data_ov005_0236ab20[] = {
#include "data/ov005/data_ov005_0236ab20.inc.c"
};
/* "033": tiles, picture 3 */
static char data_ov005_0236ab1c[] = {
#include "data/ov005/data_ov005_0236ab1c.inc.c"
};
/* "032": palette */
static char data_ov005_0236ab18[] = {
#include "data/ov005/data_ov005_0236ab18.inc.c"
};
/* "031": map */
static char data_ov005_0236ab14[] = {
#include "data/ov005/data_ov005_0236ab14.inc.c"
};
/* "030": tiles, picture 2 */
static char data_ov005_0236ab10[] = {
#include "data/ov005/data_ov005_0236ab10.inc.c"
};
/* "02f": palette */
static char data_ov005_0236ab0c[] = {
#include "data/ov005/data_ov005_0236ab0c.inc.c"
};
/* "02e": map */
static char data_ov005_0236ab08[] = {
#include "data/ov005/data_ov005_0236ab08.inc.c"
};
/* "02d": tiles, picture 1 */
static char data_ov005_0236ab04[] = {
#include "data/ov005/data_ov005_0236ab04.inc.c"
};
/* "02c": palette */
static char data_ov005_0236ab00[] = {
#include "data/ov005/data_ov005_0236ab00.inc.c"
};
/* "02b": map */
static char data_ov005_0236aafc[] = {
#include "data/ov005/data_ov005_0236aafc.inc.c"
};
/* "02a": tiles, picture 0 */
static char data_ov005_0236aaf8[] = {
#include "data/ov005/data_ov005_0236aaf8.inc.c"
};
/* "054": map */
char data_ov005_0236aaf4[] = {
#include "data/ov005/data_ov005_0236aaf4.inc.c"
};
/* "038": palette */
static char data_ov005_0236ab30[] = {
#include "data/ov005/data_ov005_0236ab30.inc.c"
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

/* States 33-37 of the state table (data_ov005_0236a74c), arg 0-4 picks the
 * picture (/rikiishi 02a-038). Loads it on both screens, scrolls layers 2
 * and 3 to y = -28, then shows BG2 and OBJ on the main screen and only BG2
 * on the sub screen. */
void Ov005State_ShowLayer2Picture(Unk_ov005_0236bba4* work, int picture) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u16* map;

  switch (picture) {
    case 0:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      LOAD_LAYER2_PICTURE(data_ov005_0236aaf8, data_ov005_0236aafc,
                          data_ov005_0236ab00);
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      LOAD_LAYER2_PICTURE(data_ov005_0236aaf8, data_ov005_0236aafc,
                          data_ov005_0236ab00);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
    case 1:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab04, data_ov005_0236ab08,
                          data_ov005_0236ab0c);
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab04, data_ov005_0236ab08,
                          data_ov005_0236ab0c);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
    case 2:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab10, data_ov005_0236ab14,
                          data_ov005_0236ab18);
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab10, data_ov005_0236ab14,
                          data_ov005_0236ab18);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
    case 3:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab1c, data_ov005_0236ab20,
                          data_ov005_0236ab24);
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab1c, data_ov005_0236ab20,
                          data_ov005_0236ab24);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      break;
    case 4:
      saved0280 = data_027c0280;
      saved027c = data_027c027c;
      Screen_SetCurrentBoth(data_027c0288.unk_00);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab28, data_ov005_0236ab2c,
                          data_ov005_0236ab30);
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      LOAD_LAYER2_PICTURE(data_ov005_0236ab28, data_ov005_0236ab2c,
                          data_ov005_0236ab30);
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
