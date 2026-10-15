#include "types.h"
#include "game/page_strip.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"

void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd);
void Co_LoadSmallPicture(int co, void* dest, int palette);
void Data_Unpack(const void* src, void* dest);
const char* File_LocalizePath(const char* path);
u8* File_Load(const char* path);
void Font_LoadDigits(int a);
void Gfx_LoadBmap09f(void* a, int b, int c);
void Gfx_RequestLayerCopy(int bg);
void MapPreview_Draw(void* dst, int mapId);
void Mem_Copy32(const void* src, void* dest, u32 size);
void Mem_Free(void* res);
void PaletteCycle_Start(int a);
void Palette_Load(const void* src, int offset, u32 size);
void Screen_SetCurrentBoth(void* state);
void Gfx_ClearLayerMap(int layer);
void func_0201e6e8(int x, int y, int width, int height);
void func_020458c0(void);
void SurvivalSelect_LoadCoPictures(void);
void SurvivalSelect_DrawMapRecord(void);
void SurvivalSelect_DrawModeRecord(void);
int SurvivalSelect_GetList(void);
int SurvivalRecord_GetRankIcon(int mode);
int SurvivalRecord_GetCo(int mode, int a1);
int SurvivalRecord_Exists(int mode);
int Survival_GetMapId(int mode, int index);

extern Unk_022a7b80 data_022a7b80;
extern char data_0215ae5c[]; /* "/ohashi/res_survival" */
extern char data_0215ae74[]; /* "/ohashi/res_survival_lang" */

/* Sets up both screens of the Survival map select from the res_survival
 * files. Top: page strip graphics, one palette (two when the list has 2 or
 * more pages, picked by the page count), the selected map's preview.
 * Bottom: a panel that, when SurvivalRecord_Exists is 1 for the mode (probably a
 * saved record), shows two COs from SurvivalRecord_GetCo and an icon picked by
 * SurvivalRecord_GetRankIcon. Also copies the strip's cycling colours into
 * data_0229c744 and resets the menu cursor and buttons. */
void SurvivalSelect_Init(void) {
  u8* res;
  int mode = data_022a7b80.mode;
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
  SurvivalSelect_DrawMapRecord();
  func_0201e6e8(12, 18, 16, 6);
  Data_Unpack(res + 0x234, data_027c0280->objVram + 0xc00);
  Data_Unpack(res + 0x2dc, data_027c0280->objVram + 0x1000);
  if (data_0229c744->lists[SurvivalSelect_GetList()].unk_03 <= 1) {
    Palette_Load(res + 0xe58, 0x300, 32);
  } else {
    Palette_Load(
        res + 0xe58 +
            ((data_0229c744->lists[SurvivalSelect_GetList()].unk_03 - 2) & 7) *
                32,
        0x300, 32);
    Palette_Load(
        res + 0xe58 +
            ((data_0229c744->lists[SurvivalSelect_GetList()].unk_03 - 1) & 7) *
                32,
        0x320, 32);
  }
  Icon_LoadTo(203, data_027c0280->objVram, 112);
  IconBank_LoadPalette(4, 26);
  SurvivalSelect_LoadCoPictures();
  MapPreview_Draw(
      data_027c0280->objVram + 0x8000,
      Survival_GetMapId(data_022a7b80.mode,
                        data_0229c744->lists[SurvivalSelect_GetList()].cursor));
  Screen_SetCurrentGfx(savedState);

  savedState = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Bg_PutTileBlock(data_027c0280->layer1Map, 2, 10, res + 0xf98, 0);
  SurvivalSelect_DrawModeRecord();
  Data_Unpack(res, data_027c0280->objVram + 0x300);
  Palette_Load(res + 0xe58, 0x280, 32);
  if (SurvivalRecord_Exists(mode) == 1) {
    Co_LoadSmallPicture(SurvivalRecord_GetCo(mode, 0), data_027c0280->objVram,
                        18);
    if (SurvivalRecord_GetCo(mode, 1) != 0) {
      Co_LoadSmallPicture(SurvivalRecord_GetCo(mode, 1),
                          data_027c0280->objVram + 0x180, 19);
    }
    Icon_LoadTo(SurvivalRecord_GetRankIcon(mode) + 85, data_027c0280->objVram,
                88);
  }
  IconBank_LoadPalette(1, 21);
  Screen_SetCurrentGfx(savedState);
  Mem_Copy32(res + 0xf78, data_0229c744->colours,
             sizeof(data_0229c744->colours));
  Mem_Free(res);

  res = File_Load(File_LocalizePath(data_0215ae74));
  savedState = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Data_Unpack(res, data_027c0280->objVram + 0x3840);
  Screen_SetCurrentGfx(savedState);
  Mem_Free(res);

  PaletteCycle_Start(27);
  Menu_Reset();
  MenuCursor_SetRect(0, -16, 224, -16);
  func_020458c0();
  MenuButtons_DisableTouch();
}
