#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/survival_record.h"
#include "game/icon.h"
#include "game/screen_other_map.h"

void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd);
void Co_LoadSmallPicture(int co, void* dest, int palette);
void Data_Unpack(const void* src, void* dest);
u8* File_Load(const char* path);
void Font_LoadDigits(int a);
void Gfx_LoadBmap09f(void* a, int b, int c);
void Gfx_RequestLayerCopy(int bg);
void Mem_Free(void* res);
void MenuButton_Select(int id);
void MenuButtons_DisableTouch(void);
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom);
void Menu_Reset(void);
void Palette_Load(const void* src, int offset, u32 size);
void Screen_SetCurrentBoth(void* state);
void Gfx_ClearLayerMap(int layer);
void NumberFont_LoadLow(void);
void TpIcon_Load(void);
void func_02046d68(void);
void SurvivalModeMenu_DrawRecords(void);
int SurvivalSelect_GetList(void);
int SurvivalModeMenu_GetRowMode(int row);
int SurvivalRecord_GetRankIcon(int mode);
int SurvivalRecord_GetCo(int mode, int a1);
int SurvivalRecord_Exists(int mode);
int SurvivalMode_HasSecondRow(void);

extern char data_0215ae5c[]; /* "/ohashi/res_survival" */

/* Clears both screens, then sets up the top screen of the Survival mode menu
 * from res_survival: a panel per row (the second only when SurvivalMode_HasSecondRow
 * allows it) and, for each row whose mode has a saved record, the record's
 * two COs (palettes 16 + row * 2 and 17 + row * 2) and an icon. Then resets
 * the menu cursor and buttons and selects the row SurvivalSelect_GetList returns.
 * Needed to match: the second picture's offset from a local, its palette
 * from row. */
void SurvivalModeMenu_Init(void) {
  u8* res;
  int row;
  int mode;
  int picture;
  Unk_027c0280* savedState = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  Gfx_ClearLayerMap(0);
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  Gfx_ClearLayerMap(0);
  Gfx_ClearLayerMap(1);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(savedState);

  res = File_Load(data_0215ae5c);
  savedState = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Bg_PutTileBlock(data_027c0280->layer1Map, 0, 4, res + 0xf98, 0);
  if (SurvivalMode_HasSecondRow() == 1) {
    Bg_PutTileBlock(data_027c0280->layer1Map, 0, 12, res + 0xf98, 0);
  }
  SurvivalModeMenu_DrawRecords();
  Data_Unpack(res, data_027c0280->objVram + 0x600);
  Palette_Load(res + 0xe58, 0x280, 32);
  for (row = 0; row < SURVIVAL_MENU_ROW_COUNT; row++) {
    mode = SurvivalModeMenu_GetRowMode(row);
    if (SurvivalRecord_Exists(mode) != 0) {
      Co_LoadSmallPicture(SurvivalRecord_GetCo(mode, 0),
                          data_027c0280->objVram + ((row * 24) << 5),
                          row * 2 + 16);
      if (SurvivalRecord_GetCo(mode, 1) != 0) {
        picture = row * 2 + 1;
        Co_LoadSmallPicture(SurvivalRecord_GetCo(mode, 1),
                            data_027c0280->objVram + ((picture * 12) << 5),
                            row * 2 + 17);
      }
      Icon_LoadTo(SurvivalRecord_GetRankIcon(mode) + 85, data_027c0280->objVram,
                  112 + row * 4);
    }
  }
  IconBank_LoadPalette(1, 21);
  Screen_SetCurrentGfx(savedState);
  Mem_Free(res);

  NumberFont_LoadLow();
  TpIcon_Load();
  Menu_Reset();
  MenuCursor_SetRect(0, -16, 224, -16);
  MenuButton_Select(SurvivalSelect_GetList());
  func_02046d68();
  MenuButtons_DisableTouch();
}
