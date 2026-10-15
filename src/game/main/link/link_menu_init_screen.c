#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/link_menu_state.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229fb30.h"
#include "game/screen_other_map.h"

void Gfx_InitBothScreens(void);
void Wallpaper_LoadBoth(void);
void Screen_SetCurrentBoth(void* state);
void Gfx_LoadBmap09f(void* target, int a, int b);
void Font_LoadDigits(int a);
void LargeFont_Load(void);
void* File_Load(const char* path);
const char* File_LocalizePath(const char* path);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, u32 offset, u32 size);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Mem_Free(void* file);
void Gfx_RequestLayerCopy(int layer);
void NumberFont_LoadLow(void);
int func_0207a324(void);
void Link_DrawPlayerRows(void);
void LinkMenu_DrawSecondView(void);
void ModeFont_Load(int a, int b);
void ModeFont_ShowText(const void* text, int a, int b, int c, int d);
const void* Text_Get(u32 textId);
void SpriteTask_Create(int a, int b, int c, int d);
void LinkMenu_AddAllButtons(void);
void LinkMenu_AddBackButtons(void);

extern u16 data_0229f330
    []; /* layer 2 map of the second view, beside the list's map in gMenuState */
extern const char gLinkLoginResPath[];
extern const char gLinkMenuCommonLangResPath[];
extern const char gLinkMenuCommonResPath[];

/* Script command of the link player list (LinkMenu_Start's script). Loads
 * /commo/res_linklogin, /commo/res_linkcommon_lang and /commo/res_linkcommon:
 * two layer 2 maps (the player list into gMenuState, and a second view picked
 * by func_0207a324, 2, 4 or 8, into data_0229f330), both moved to palette 1.
 * LinkMenuState.view picks the view: 0 shows the player list
 * (Link_DrawPlayerRows), 1 the second view (LinkMenu_DrawSecondView). MAP_STATE->unk_8b
 * picks the title text. */

void LinkMenu_InitScreen(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u8* file;
  u16* map;
  int i;
  u32 textId;

  *(u16*)&task->unk_4c = 0; /* the original stores a halfword */
  Gfx_InitBothScreens();
  Wallpaper_LoadBoth();
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  LargeFont_Load();

  file = File_Load(gLinkLoginResPath);
  Data_Unpack(file, &gMenuState);
  switch (func_0207a324()) {
    case 8:
      Data_Unpack(file + 0x3c0, data_0229f330);
      break;
    case 4:
    default:
      Data_Unpack(file + 0x2a8, data_0229f330);
      break;
    case 2:
      Data_Unpack(file + 0x190, data_0229f330);
      break;
  }
  Palette_Load(file + 0x6cc, 0x20, 0x20);
  Mem_Free(file);
  map = (u16*)&gMenuState;
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    map[i] += 0x1000; /* palette 1 */
  }
  map = data_0229f330;
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    map[i] += 0x1000;
  }
  if (data_0215fed0->view == 0) {
    Mem_Copy32(&gMenuState, data_027c0280->layer2Map, BG_VISIBLE_SIZE);
  } else {
    Mem_Copy32(data_0229f330, data_027c0280->layer2Map, BG_VISIBLE_SIZE);
  }
  Gfx_RequestLayerCopy(2);

  file = File_Load(File_LocalizePath(gLinkMenuCommonLangResPath));
  if (MAP_STATE->unk_8b == 2) {
    Data_Unpack(
        file + 0x5e4,
        data_027c0280->bgVram +
            (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
  } else {
    Data_Unpack(
        file + 0x2d8,
        data_027c0280->bgVram +
            (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
  }
  Data_Unpack(file, data_027c0280->objVram);
  Mem_Free(file);

  file = File_Load(gLinkMenuCommonResPath);
  Palette_Load(file, 0x200, 0x20);
  Palette_Load(file, 0x220, 0x20);
  Mem_Copy32(file + PALETTE_16_SIZE, &data_0229fb30, PALETTE_16_SIZE);
  Mem_Free(file);
  NumberFont_LoadLow();

  if (data_0215fed0->view == 0) {
    Link_DrawPlayerRows();
  } else {
    LinkMenu_DrawSecondView();
  }
  ModeFont_Load(0, 0);
  switch (MAP_STATE->unk_8b) {
    default:
    case 0:
      textId = 0x7d000017;
      break;
    case 1:
      textId = 0x7d000018;
      break;
    case 2:
      textId = 0x7d000019;
      break;
    case 3:
      textId = 0x7d000013;
      break;
    case 4:
      textId = 0x7d00001a;
      break;
  }
  ModeFont_ShowText(Text_Get(textId), 0, 0, 0, 0);
  SpriteTask_Create(0xe8, 8, 0x3f0, 15);
  Menu_Reset();
  if (data_0215fed0->view == 0) {
    LinkMenu_AddAllButtons();
  } else {
    LinkMenu_AddBackButtons();
  }
  MenuButtons_DisableTouch();
}
