#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_02160ac0.h"
#include "game/combat_mode_menu.h"

void Gfx_InitBothScreens(void);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void* Mem_Alloc(u32 size);
void Mem_Free(void* file);
void Mem_Fill32(u32 value, void* dst, u32 size);
void* File_Load(const char* path);
const char* File_LocalizePath(const char* path);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, u32 offset, u32 size);
void Palette_LoadObjRow(int row);
void Gfx_LoadBmap09f(void* target, int a, int b);
void Font_LoadDigits(int a);
void Gfx_RequestLayerCopy(int layer);
void Icon_LoadTo(int iconId, u8* vram, int tile);
void Icon_LoadPalette(int iconId, int paletteSlot);
void Co_LoadSmallPicture(int coId, void* dest, int paletteSlot);
BOOL CombatModeMenu_IsSlotAvailable(
    u8 slot); /* slots 0-2 always TRUE, 3-6 by game flag */
void ModeFont_Load(int a, int b);
const char* Text_Get(u32 textId);
void ModeFont_ShowTextOnScreen(const char* text, int tile, int paletteSlot,
                               int unk_56, u8 unk_5c, int unk_58);
void Menu_Reset(void);
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom);
void MenuButton_Select(int button);
void Wallpaper_LoadBoth(void);
void CombatModeCoPortrait_Start(void);
void HelpBand_Start(int tile, int a);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8
    gCombatModeCos[]; /* CO ids: Andy, Max, Sami, Flak, Lash, Adder, Hawke */
extern const char gCombatMenuResPath[]; /* "/combat_st/res_cmmenu" */
extern const char
    gCombatMenuLangResPath[]; /* "/combat_st/res_cmmenu_la..." (localized) */

/* Mode title shown at the top of the screen. */
#define TEXT_COMBAT_MENU_TITLE 0x7d000013

#define MAIN_MAP (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

/* Sets up the Combat mode menu: blending on the second screen, the
 * backgrounds, icons and palettes from /combat_st/res_cmmenu, small pictures
 * of the seven COs in gCombatModeCos (a black palette for each slot where
 * CombatModeMenu_IsSlotAvailable returns FALSE), the menu cursor on the button of
 * data_02160ac0->co, and the mode title. data_0229e144 gets a one-byte
 * copy of co. */
void CombatModeMenu_Init(void) {
  int i;
  u8* file;
  u8* langFile;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  Gfx_InitBothScreens();

  SWITCH_TO(MAIN_MAP);
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 1;
  SWITCH_BACK();

  SWITCH_TO(SECOND_MAP);
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 12;
  data_027c0280->blendEvb = 4;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendBottomObj = 1;
  SWITCH_BACK();

  data_0229e144 = Mem_Alloc(sizeof(*data_0229e144));
  *data_0229e144 = data_02160ac0->co;
  file = File_Load(gCombatMenuResPath);
  langFile = File_Load(File_LocalizePath(gCombatMenuLangResPath));

  SWITCH_TO(MAIN_MAP);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Data_Unpack(file + 0xdc, data_027c0280->layer1Map);
  Gfx_RequestLayerCopy(1);
  Data_Unpack(
      langFile,
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
  Data_Unpack(file + 0x1774, data_027c0280->layer2Map);
  Palette_Load(file + 0x189c, 0x40, 0x20);
  for (i = 0; i < 0x300; i++) {
    data_027c0280->layer2Map[i] |= 0x2000;
  }
  Gfx_RequestLayerCopy(2);
  Icon_LoadTo(61, data_027c0280->objVram, 0x60);
  Icon_LoadPalette(61, 24);
  for (i = 0; i < 7; i++) {
    /* 12 tiles of 32 bytes per picture */
    Co_LoadSmallPicture(gCombatModeCos[i],
                        data_027c0280->objVram + ((i * 12) << 5), i + 16);
  }
  for (i = 0; i < 7; i++) {
    if (!CombatModeMenu_IsSlotAvailable(i)) {
      Mem_Fill32(0, data_027c0280->paletteBuffer + (i + 16) * 32, 32);
    }
  }
  Data_Unpack(file + 0x1cc, data_027c0280->objVram + 0x8000);
  Data_Unpack(file + 0x12cc, data_027c0280->objVram + 0xa200);
  Palette_Load(file + 0x14f4, 0x320, 0x60);
  Palette_Load(file + 0x16d4, 0x380, 0x20);
  Palette_LoadObjRow(7);
  SWITCH_BACK();

  SWITCH_TO(SECOND_MAP);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Font_LoadDigits(0);
  Data_Unpack(file, data_027c0280->layer1Map);
  Gfx_RequestLayerCopy(1);
  Icon_LoadTo(19, data_027c0280->objVram, 0x1bc);
  Icon_LoadPalette(19, 18);
  SWITCH_BACK();

  Mem_Free(file);
  Mem_Free(langFile);
  Menu_Reset();
  MenuCursor_SetRect(0, -16, 0x100, -16);
  MenuButton_Select(data_02160ac0->co);
  Wallpaper_LoadBoth();
  ModeFont_Load(0x1c0, 3);
  ModeFont_ShowTextOnScreen(Text_Get(TEXT_COMBAT_MENU_TITLE), 0x1c0, 3, 0x800,
                            4, 0);
  CombatModeCoPortrait_Start();
  HelpBand_Start(0x3f8, 0);
}
