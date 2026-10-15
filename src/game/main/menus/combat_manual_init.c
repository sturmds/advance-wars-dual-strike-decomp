#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"
#include "game/text_print.h"
#include "game/menu_button.h"

void Gfx_InitBothScreens(void);
void* File_Load(const char* path);
void Mem_Free(void* file);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, u32 offset, u32 size);
void Gfx_LoadBmap09f(void* target, int a, int b);
void Font_LoadDigits(int a);
void Gfx_RequestLayerCopy(int layer);
void func_0201e6e8(int x, int y, int width, int height);
void Bg_PutTileQuad(int x, int y, int firstTile, int palette);
void Wallpaper_LoadBoth(void);
void ModeFont_Load(int tile, int paletteSlot);
const void* Text_Get(u32 textId);
void ModeFont_ShowTextOnScreen(const void* text, int tile, int paletteSlot,
                               int unk_56, u8 unk_5c, int unk_58);
void func_0208b9a0(void);

extern const char gCombatManualResPath[]; /* "/combat_st/res_cmmanual" */

#define MAIN_MAP (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

/* SWITCH_TO opens a block with the saved screen pointers, which
 * SWITCH_BACK restores and closes. */
#define SWITCH_TO(state)                     \
  {                                          \
    Unk_027c0280* saved0280 = data_027c0280; \
    Unk_02080440* saved027c;                 \
    Screen_SetCurrentGfx(state);             \
    saved027c = data_027c027c;               \
    Screen_SetCurrent(state)

#define SWITCH_BACK()              \
  Screen_SetCurrent(saved027c);    \
  Screen_SetCurrentGfx(saved0280); \
  }

/* Sets up the Combat mode manual screen from /combat_st/res_cmmanual: on
 * the main screen a full window with the title (text 0x43000002) and five
 * lines (0x43000003-7), each with a 2x2 icon on its left; on the second
 * screen two windows with texts 0x43000000 and 0x43000001. Then the
 * wallpapers, the mode title (0x7d000013) and one menu button. */
void CombatManual_Init(void) {
  u8* file;

  Gfx_InitBothScreens();
  file = File_Load(gCombatManualResPath);

  SWITCH_TO(MAIN_MAP);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  func_0201e6e8(0, 0, 32, 24);
  Text_PrintId(1, 1, data_027c0280->layer0Map, 0x43000002, 0, 0x100);
  Text_PrintId(3, 13, data_027c0280->layer0Map, 0x43000003, 0, 0x280);
  Text_PrintId(3, 15, data_027c0280->layer0Map, 0x43000004, 0, 0x2c0);
  Text_PrintId(3, 17, data_027c0280->layer0Map, 0x43000005, 0, 0x300);
  Text_PrintId(3, 19, data_027c0280->layer0Map, 0x43000006, 0, 0x340);
  Text_PrintId(3, 21, data_027c0280->layer0Map, 0x43000007, 0, 0x380);
  Data_Unpack(
      file, data_027c0280->bgVram +
                (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14) +
                0x7800);
  Palette_Load(file + 0x1e4, 0x20, 0x20);
  Bg_PutTileQuad(1, 13, 0x3c0, 1);
  Bg_PutTileQuad(1, 15, 0x3c4, 1);
  Bg_PutTileQuad(1, 17, 0x3c8, 1);
  Bg_PutTileQuad(1, 19, 0x3cc, 1);
  Bg_PutTileQuad(1, 21, 0x3d0, 1);
  Gfx_RequestLayerCopy(0);
  SWITCH_BACK();

  SWITCH_TO(SECOND_MAP);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  func_0201e6e8(0, 6, 32, 12);
  func_0201e6e8(0, 18, 32, 6);
  Text_PrintId(1, 7, data_027c0280->layer0Map, 0x43000000, 0, 0x100);
  Text_PrintId(1, 19, data_027c0280->layer0Map, 0x43000001, 0, 0x300);
  Gfx_RequestLayerCopy(0);
  SWITCH_BACK();

  Mem_Free(file);
  Wallpaper_LoadBoth();
  ModeFont_Load(0, 0);
  ModeFont_ShowTextOnScreen(Text_Get(0x7d000013), 0, 0, 0x800, 0, 0);
  Menu_Reset();
  MenuCursor_SetRect(0, -16, 0x100, -16);
  func_0208b9a0();
  MenuButtons_DisableTouch();
}
