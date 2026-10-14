#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

#define STATE (data_027c027c->unk_48[data_027c0284])
/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Tile blocks of the Vs setup screen that VsSetup_DrawSlotPanels and
 * others read from gMenuState. */
#define PANEL_ONE_MAP ((u8*)MENU + 0x20)
#define PANEL_TWO_MAPS ((u8*)MENU + 0xe4)
/* Two more blocks from res_versus; what reads them is not known yet. */
#define VS_BLOCK_1A8 ((u8*)MENU + 0x1a8)
#define VS_BLOCK_21C ((u8*)MENU + 0x21c)

/* Start of the tile data of a BG layer in BG VRAM. */
#define LAYER_CHAR_BASE(gfx, layerBg) \
  ((gfx)->bgVram + (gfx)->bgCnt[layerBg].charBase * 0x4000)

void Data_Unpack(const void* src, void* dest);
u8* File_Load(const char* path);
const char* File_LocalizePath(const char* path);
void ForceRank_LoadSets(int mode);
void Gfx_InitBothScreens(void);
void Gfx_LoadBmap09f(void* a, int b, int c);
void HelpBand_Start(int textId, int arg);
void IconBank_LoadPalette(int bank, int paletteSlot);
void Icon_LoadTo(int iconId, u8* vram, int tile);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Mem_Free(void* res);
void ModeFont_Load(int x, int y);
void ModeFont_ShowTextOnScreen(const char* text, int tile, int paletteSlot,
                               int unk_56, u8 unk_5c, int unk_58);
void Palette_Load(const void* src, u32 offset, u32 size);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentBoth(void* state);
void Screen_SetCurrentGfx(void* state);
void SpriteTask_Create(int x, int y, int tile, int paletteSlot);
const void* Text_Get(u32 textId);
void Wallpaper_LoadBoth(void);
void NumberFont_LoadLow(void);
void TpIcon2_LoadTo(u8* dest);
void TpIcon_Load(void);
void func_0202b7e0(int a0);
void func_0202b804(void* dest);
int Link_IsActive(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern char data_0215d278[]; /* "/ohashi/res_versus" */
extern char data_0215d28c[]; /* "/commo/res_linkcommon_lang" */
extern char data_0215d2a8[]; /* "/commo/res_linkcommon" */

/* Sets up both screens of the Vs setup screen. Both get file data from
 * func_0202b804 in layer 0's tiles; res_versus gives the layer 2 tiles and palette of both screens and
 * four tile blocks kept in gMenuState. The main screen loads the icons of
 * the setup sprites and the sub screen gets a copy of them. When
 * Link_IsActive() returns 1 (a link mode) the res_linkcommon sprite tiles and palette are
 * loaded too, and a sprite is made at the end. Then the Force Rank sets,
 * the title text (one text for one map, another for two maps), the
 * wallpaper and help band 0x7f8. */
void VsSetup_Init(void) {
  u8* res;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  Unk_027c0280* saved;
  Unk_027c0280* savedIcons; /* needed to match: not shared with saved */
  int i;

  Gfx_InitBothScreens();
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  GFX_DISPCNT(data_027c0280)->objMap1D = 1;
  data_027c0280->unk_dd = 1;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  GFX_DISPCNT(data_027c0280)->objMap1D = 1;
  data_027c0280->unk_dd = 1;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  func_0202b804(LAYER_CHAR_BASE(data_027c0280, data_027c0280->layer0Bg) +
                0x2000);
  func_0202b7e0(2);
  Screen_SetCurrentGfx(saved);

  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  func_0202b804(LAYER_CHAR_BASE(data_027c0280, data_027c0280->layer0Bg) +
                0x2000);
  func_0202b7e0(2);
  Screen_SetCurrentGfx(saved);

  res = File_Load(data_0215d278);
  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Data_Unpack(res, LAYER_CHAR_BASE(data_027c0280, data_027c0280->layer2Bg));
  Palette_Load(res + 0x4fc, 0x60, 0xa0);
  Screen_SetCurrentGfx(saved);

  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
  Mem_Copy32(((Unk_027c0280*)data_027c0288.unk_00)->unk_10c,
             LAYER_CHAR_BASE(data_027c0280, data_027c0280->layer2Bg), 0x1000);
  Palette_Load(res + 0x4fc, 0x60, 0xa0);
  Screen_SetCurrentGfx(saved);

  Mem_Copy32(res + 0x59c, PANEL_ONE_MAP, 0xc2);
  Mem_Copy32(res + 0x660, PANEL_TWO_MAPS, 0xc2);
  Mem_Copy32(res + 0x724, VS_BLOCK_1A8, 0x72);
  Mem_Copy32(res + 0x798, VS_BLOCK_21C, 0x92);
  Mem_Free(res);

  savedIcons = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  for (i = 0; i < 4; i++) {
    Icon_LoadTo(i + 0xcc, data_027c0280->objVram, i * 4);
  }
  Icon_LoadTo(0xd0, data_027c0280->objVram, 16);
  for (i = 0; i < 4; i++) {
    Icon_LoadTo(i + 0x147, data_027c0280->objVram, i * 4 + 20);
    Icon_LoadTo(i + 0x14b, data_027c0280->objVram, i * 4 + 36);
  }
  for (i = 0; i < 4; i++) {
    Icon_LoadTo(i + 0x114, data_027c0280->objVram, i * 32 + 52);
  }
  for (i = 0; i < 9; i++) {
    Icon_LoadTo(i + 0x118, data_027c0280->objVram, i * 8 + 180);
  }
  Icon_LoadTo(0x121, data_027c0280->objVram, 252);
  Icon_LoadTo(0x122, data_027c0280->objVram, 260);
  for (i = 0; i < 4; i++) {
    Icon_LoadTo(i + 0x123, data_027c0280->objVram, i * 8 + 268);
  }
  for (i = 0; i < 9; i++) {
    Icon_LoadTo(i + 0x127, data_027c0280->objVram, i * 16 + 300);
  }
  Icon_LoadTo(0x130, data_027c0280->objVram, 444);
  Icon_LoadTo(0x131, data_027c0280->objVram, 448);
  Icon_LoadTo(0x133, data_027c0280->objVram, 452);
  Icon_LoadTo(0x132, data_027c0280->objVram, 456);
  Icon_LoadTo(0x134, data_027c0280->objVram, 460);
  for (i = 0; i < 4; i++) {
    Icon_LoadTo(i + 0x135, data_027c0280->objVram, i * 4 + 464);
  }
  for (i = 0; i < 4; i++) {
    Icon_LoadTo(i + 0x139, data_027c0280->objVram, i * 4 + 480);
  }
  for (i = 0; i < 10; i++) {
    Icon_LoadTo(i + 0x13d, data_027c0280->objVram, i * 2 + 496);
  }
  Icon_LoadTo(0x14f, data_027c0280->objVram, 516);
  IconBank_LoadPalette(4, 16);
  IconBank_LoadPalette(5, 17);
  Screen_SetCurrentGfx(savedIcons);

  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Mem_Copy32(((Unk_027c0280*)data_027c0288.unk_00)->objVram,
             data_027c0280->objVram, 0x4500);
  IconBank_LoadPalette(4, 16);
  IconBank_LoadPalette(5, 17);
  Screen_SetCurrentGfx(saved);

  if (Link_IsActive() == 1) {
    res = File_Load(File_LocalizePath(data_0215d28c));
    Data_Unpack(res, data_027c0280->objVram + 0xc000);
    Mem_Free(res);
    res = File_Load(data_0215d2a8);
    Palette_Load(res, 0x3c0, 0x20);
    Mem_Copy32(res + 0x20, MENU->colours, sizeof(MENU->colours));
    Mem_Free(res);
  }
  ForceRank_LoadSets(STATE->gameMode);
  NumberFont_LoadLow();
  TpIcon_Load();
  TpIcon2_LoadTo(data_027c0280->objVram + 0x8000);
  ModeFont_Load(0x2c8, 10);
  if (STATE->twoMaps == 0) {
    ModeFont_ShowTextOnScreen(Text_Get(0x7d000017), 0x2c8, 10, 0x800, 0, 0);
  } else {
    ModeFont_ShowTextOnScreen(Text_Get(0x7d000018), 0x2c8, 10, 0x800, 0, 0);
  }
  Wallpaper_LoadBoth();
  HelpBand_Start(0x7f8, 0);
  if (Link_IsActive() == 1) {
    SpriteTask_Create(232, 8, 696, 15);
  }
}
