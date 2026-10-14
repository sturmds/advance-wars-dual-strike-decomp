#include "types.h"
#include "game/menu_state.h"
#include "game/icon.h"
#include "game/player.h"
#include "game/tab_list.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Data_Unpack(const void* src, void* dest);
const char* File_LocalizePath(const char* path);
u8* File_Load(const char* path);
void Font_LoadDigits(int a);
void Gfx_LoadBmap09f(void* a, int b, int c);
void Gfx_RequestLayerCopy(int bg);
void HelpBand_Start(int tile, int a);
void Mem_Copy32(const void* src, void* dest, u32 size);
void Mem_Free(void* res);
void MenuButton_Select(int id);
void MenuButtons_DisableTouch(void);
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom);
void Menu_Reset(void);
void ModeFont_Load(int tile, int a);
void ModeFont_ShowText(const void* text, int tile, int paletteSlot, int a,
                       int b);
void Palette_BlendWithColor(void* dst, int a, int b, int c);
void Palette_Load(const void* src, int offset, u32 size);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentBoth(void* state);
void Screen_SetCurrentGfx(void* state);
const void* Text_Get(u32 textId);
void TpIcon3_LoadTo(u8* dest);
void TransferQueue_Add(const void* src, void* dst, u32 size);
void Wallpaper_LoadSelected(void);
void NumberFont_LoadLow(void);
void Screen_SetLayerCharBase(int a, u32 b);
void func_02028a30(void);
int UnitIcons_GetTileCount(void);
u8* UnitIcons_GetTiles(void);
void TabList_AddNavButtons(void);
void TabList_UpdateHelpText(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const u16 gArmyPalettes[][16];
extern char data_0215f7c4[]; /* "/ohashi/res_history_medal" */
extern char data_0215f7e0[]; /* "/ohashi/res_history" */
extern char data_0215f7f4[]; /* "/ohashi/res_history_lang" */

#define BG_CHAR_BASE(bg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[bg].charBase << 14))

/* Sets up both screens of the history (records) screen from the
 * res_history, res_history_lang and res_history_medal files: resets the
 * tab list to tab 0, page 0, loads the background, icons and palettes on
 * each screen, a ModeFont title, the menu cursor, wallpaper and help
 * band, then calls TabList_UpdateHelpText. */
void HistoryScreen_Init(void) {
  u8* res;
  u8* lang;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  Unk_027c0280* gfx;
  u8 bg;
  u8* icons;
  int tileCount;
  int size;
  int i;

  gMenuState.tabList.tab = 0;
  for (i = 0; i < (int)ARRAY_COUNT(gMenuState.tabList.pageOfTab); i++) {
    gMenuState.tabList.pageOfTab[i] = 0;
  }
  gMenuState.tabList.row = 0;
  gMenuState.tabList.unk_06 = 0;
  func_02028a30();

  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->dispCnt |= 0x40; /* DispCntBits.objMap1D */
  data_027c0280->unk_dd = 1;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->dispCnt |= 0x40; /* DispCntBits.objMap1D */
  data_027c0280->unk_dd = 1;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Screen_SetLayerCharBase(1, 0x06204000);
  Screen_SetCurrentGfx(saved0280);

  res = File_Load(data_0215f7c4);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Data_Unpack(res, BG_CHAR_BASE(data_027c0280->layer0Bg) + 0x7c80);
  Data_Unpack(res + 0x6c, BG_CHAR_BASE(data_027c0280->layer0Bg) + 0x7d00);
  Palette_Load(res + 0x2b4, 0x20, 0x40);
  Palette_Load(res + 0x2b4, 0x200, 32);
  Mem_Copy32(data_027c0280->paletteBuffer + 0x200,
             data_027c0280->paletteBuffer + 0x220, PALETTE_16_SIZE);
  Palette_BlendWithColor(data_027c0280->paletteBuffer + 0x220, 0, 8, 8);
  Screen_SetCurrentGfx(saved0280);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Data_Unpack(res, data_027c0280->objVram + 0x4600);
  Palette_Load(res + 0x2b4, 0x240, 32);
  Screen_SetCurrentGfx(saved0280);
  /* Needed to match: the field offset is added to gMenuState at run time. */
  Mem_Copy32(res + 0x2f4,
             (u8*)&gMenuState + (u32) & ((TabListView*)0)->unk_1000,
             sizeof(((TabListView*)0)->unk_1000));
  Mem_Free(res);

  res = File_Load(data_0215f7e0);
  lang = File_Load(File_LocalizePath(data_0215f7f4));
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Data_Unpack(res, data_027c0280->layer1Map);
  Gfx_RequestLayerCopy(1);
  Data_Unpack(lang, data_027c0280->objVram);
  Data_Unpack(lang + 0x2f54, data_027c0280->objVram + 0x8000);
  Palette_Load(res + 0xe0, 0x260, 32);
  for (i = 0; i < ICON_DIGIT_COUNT; i++) {
    Icon_LoadTo(ICON_DIGITS_SMALL + i, data_027c0280->objVram, 128 + i * 2);
  }
  Icon_LoadTo(ICON_SMALL_SLASH, data_027c0280->objVram, 148);
  IconBank_LoadPalette(4, 18);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Data_Unpack(lang + 0x34b4, BG_CHAR_BASE(data_027c0280->layer1Bg));
  Data_Unpack(res + 0x100, data_027c0280->layer1Map);
  Palette_Load(res + 0x3b4, 0x40, 0xa0);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer1Map[i] += 0x2000;
  }
  Gfx_RequestLayerCopy(1);
  Data_Unpack(lang + 0x424, data_027c0280->objVram);
  Palette_Load(lang + 0x3374, 0x260, 0x140);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(lang);
  Mem_Free(res);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  /* Icon tiles go just below BG tile 996, where TabList_PutIconQuad
     * reads them. UnitIcons_GetTileCount is called twice, once for the place and
     * once for the size. */
  gfx = data_027c0280;
  bg = gfx->layer0Bg;
  icons = UnitIcons_GetTiles();
  tileCount = UnitIcons_GetTileCount();
  size = UnitIcons_GetTileCount();
  TransferQueue_Add(
      icons,
      gfx->bgVram + (gfx->bgCnt[bg].charBase << 14) + (996 - tileCount) * 32,
      (u16)(size * 32));
  /* Orange Star and Black Hole. Needed to match: the second as a byte
     * offset; gArmyPalettes[4] folds into the literal. */
  Palette_Load(gArmyPalettes[0], 0x60, sizeof(gArmyPalettes[0]));
  Palette_Load(
      (u8*)gArmyPalettes + (ARMY_BLACK_HOLE - 1) * sizeof(gArmyPalettes[0]),
      0x80, sizeof(gArmyPalettes[0]));
  Screen_SetCurrentGfx(saved0280);

  NumberFont_LoadLow();
  TpIcon3_LoadTo(data_027c0280->objVram + 0x12c0);
  ModeFont_Load(0x234, 0);
  ModeFont_ShowText(Text_Get(0x7d000009), 0x234, 0, 0, 0);
  Menu_Reset();
  MenuCursor_SetRect(0, -16, 256, -16);
  MenuButton_Select(0);
  TabList_AddNavButtons();
  MenuButtons_DisableTouch();
  Wallpaper_LoadSelected();
  HelpBand_Start(0x3f8, 0);
  TabList_UpdateHelpText();
}
