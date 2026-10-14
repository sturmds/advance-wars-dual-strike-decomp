#include "types.h"
#include "game/icon.h"
#include "game/menu_state.h"
#include "game/player.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

u8 Map_GetLoadedPlayerCount(void);
int Player_GetCoByte(int player, int slot);
void Gfx_InitBothScreens(void);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
u8* File_Load(const char* path);
const char* File_LocalizePath(const char* path);
void Mem_Free(void* file);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, int offset, u32 size);
void Gfx_LoadBmap09f(void* target, int a, int b);
void Font_LoadDigits(int a);
void func_0201e6e8(int x, int y, int width, int height);
void Gfx_RequestLayerCopy(int layer);
void func_0202b7e0(int a0);
u8* UnitIcons_GetTiles(void);
int UnitIcons_GetTileCount(void);
const void* Player_GetPalette(int player);
void Co_LoadArmyGraphicsAndPalette(int coId, int firstTile, int paletteSlot,
                                   int army);
void Palette_LoadObjRow(int row);
void PlayerInfo_LoadCoPictures(void);
void NumberFont_LoadLow(void);
void TpIcon_Load(void);
void TpIcon2_LoadTo(u8* dest);
void Menu_Reset(void);
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom);
void MenuButtons_DisableTouch(void);
void Wallpaper_LoadBoth(void);
void PlayerInfo_StartRedrawTask(void);
int Link_IsActive(void);
void SpriteTask_Create(int x, int y, int tile, int paletteSlot);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern const char gPlayerInfoPath[];     /* "/ohashi/res_syoguninfo" */
extern const char gPlayerInfoLangPath[]; /* "/ohashi/res_syoguninfo_lang" */

#define GAME STATE_IDX_FIRST
#define MENU (gMenuState.unk_0206d67c)

/* Needed to match: the original adds the field offset to gMenuState's
 * address at run time instead of folding it into the literal. */
#define MENU_OFFSET(field) ((u32) & ((MenuState*)0)->unk_0206d67c.field)
#define MENU_SHOWN_CO \
  (((u8*)&gMenuState + (MENU.player - 1) * 2 + MENU.slot)[MENU_OFFSET(coShown)])

/* BG tile data of layer n of the current screen. */
#define LAYER_CHARS(n)     \
  (data_027c0280->bgVram + \
   (data_027c0280->bgCnt[data_027c0280->layer##n##Bg].charBase << 14))

/* First step of the player info screen, opened on task->unk_64 (player 1-4)
 * and task->unk_66 (0 front CO, 1 back CO); task->unk_68 nonzero sets
 * unk_0a8. In Survival with unk_0a8 set only player 1 is listed, with army
 * colour and army style 1. Stores each player's army colour and front and back
 * CO, sets an alpha blend (12/16 and 4/16) of layer 2 over layer 3 on the main
 * screen, then loads /ohashi/res_syoguninfo and its language file: the main
 * screen gets the layer 2 picture, icons and player palettes, the sub screen
 * the layer 1 picture, the shown CO's picture in its army palette, and two
 * runs of ten icons. */
void PlayerInfo_Init(Task* task) {
  int i;
  u8* file;
  u8* langFile;

  /* Needed to match: the original reads unk_64 signed (ldrsh). */
  MENU.player = task->unk_64;
  MENU.slot = task->unk_66;
  MENU.page = 0;
  MENU.lastPageView = 0;
  if (task->unk_68 != 0) {
    MENU.unk_0a8 = 1;
  } else {
    MENU.unk_0a8 = 0;
  }

  if (GAME->gameMode == GAME_MODE_SURVIVAL && MENU.unk_0a8 == 1) {
    MENU.playerCount = 1;
    GAME->players[1].armyColour = 1;
    GAME->players[1].armyStyle = 1;
  } else {
    MENU.playerCount = Map_GetLoadedPlayerCount();
  }
  for (i = 0; i < MENU.playerCount; i++) {
    MENU.playerArmy[i] = GAME->players[i + 1].armyColour;
    MENU.coShown[i][0] = Player_GetCoByte(i + 1, 0);
    MENU.coShown[i][1] = Player_GetCoByte(i + 1, 1);
  }

  Gfx_InitBothScreens();
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = 12;
    data_027c0280->blendEvb = 4;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendBottomObj = 0;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Unk_02080440* saved027c;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }

  file = File_Load(gPlayerInfoPath);
  langFile = File_Load(File_LocalizePath(gPlayerInfoLangPath));

  {
    Unk_027c0280* saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Font_LoadDigits(0);
    func_0201e6e8(2, 0, 28, 4);
    Data_Unpack(file + 0x20, LAYER_CHARS(2));
    Data_Unpack(file + 0x638, data_027c0280->layer2Map);
    Palette_Load(file + 0x7e8, 0x20, 0x20);
    for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
      data_027c0280->layer2Map[i] += 0x1000;
    }
    Gfx_RequestLayerCopy(2);
    Icon_LoadTo(19, LAYER_CHARS(0), 704);
    Icon_LoadTo(20, LAYER_CHARS(0), 708);
    Icon_LoadTo(66, LAYER_CHARS(0), 712);
    IconBank_LoadPalette(0, 2);
    for (i = 0; i < 41; i++) {
      Icon_LoadTo(i + 225, LAYER_CHARS(0), 716 + i * 3);
    }
    Icon_LoadTo(174, LAYER_CHARS(0), 0x347);
    Icon_LoadTo(175, LAYER_CHARS(0), 0x349);
    Icon_LoadTo(176, LAYER_CHARS(0), 0x34b);
    Icon_LoadTo(177, LAYER_CHARS(0), 0x34d);
    Icon_LoadTo(178, LAYER_CHARS(0), 0x34f);
    Icon_LoadTo(179, LAYER_CHARS(0), 0x351);
    Icon_LoadTo(180, LAYER_CHARS(0), 0x353);
    IconBank_LoadPalette(4, 3);
    func_0202b7e0(4);
    {
      /* Needed to match: the screen and its BG are read before the two calls. */
      Unk_027c0280* gfx = data_027c0280;
      int bg = gfx->layer0Bg;
      Mem_Copy32(UnitIcons_GetTiles(),
                 gfx->bgVram + (gfx->bgCnt[bg].charBase << 14) + 0x6ca0,
                 UnitIcons_GetTileCount() << 5);
    }
    for (i = 0; i < 4; i++) {
      Palette_Load(Player_GetPalette(i + 1), (u16)((i + 5) * 32), 32);
    }
    Data_Unpack(langFile, data_027c0280->objVram + 0x8000);
    Palette_Load(file, 0x280, 32);
    for (i = 0; i < 5; i++) {
      Icon_LoadTo(i + 80, data_027c0280->objVram, 12 + i * 4);
    }
    IconBank_LoadPalette(1, 17);
    for (i = 0; i < 10; i++) {
      Icon_LoadTo(i + 150, data_027c0280->objVram, 32 + i * 2);
    }
    Icon_LoadTo(162, data_027c0280->objVram, 52);
    IconBank_LoadPalette(4, 19);
    Screen_SetCurrentGfx(saved0280);
  }
  {
    Unk_027c0280* saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Data_Unpack(langFile + 0x428, LAYER_CHARS(1));
    Data_Unpack(file + 0x8a8, data_027c0280->layer1Map);
    Palette_Load(file + 0x808, 0, 160);
    Gfx_RequestLayerCopy(1);
    Co_LoadArmyGraphicsAndPalette(MENU_SHOWN_CO, 108, 0, MENU.player);
    Palette_LoadObjRow(1);
    Icon_LoadTo(139, data_027c0280->objVram, 48);
    for (i = 0; i < ICON_DIGIT_COUNT; i++) {
      Icon_LoadTo(ICON_DIGITS + i, data_027c0280->objVram, 56 + i * 2);
    }
    Icon_LoadTo(213, data_027c0280->objVram, 76);
    for (i = 0; i < 10; i++) {
      Icon_LoadTo(i + 150, data_027c0280->objVram, 80 + i * 2);
    }
    Icon_LoadTo(160, data_027c0280->objVram, 100);
    Icon_LoadTo(161, data_027c0280->objVram, 102);
    Icon_LoadTo(197, data_027c0280->objVram, 104);
    IconBank_LoadPalette(4, 18);
    Screen_SetCurrentGfx(saved0280);
  }

  /* Five rows of 26 bytes, 28 apart, over the start of gMenuState. */
  Mem_Copy32(file + 0x99c, (u8*)&gMenuState, 26);
  Mem_Copy32(file + 0x9b8, (u8*)&gMenuState + 28, 26);
  Mem_Copy32(file + 0x9d4, (u8*)&gMenuState + 56, 26);
  Mem_Copy32(file + 0x9f0, (u8*)&gMenuState + 84, 26);
  Mem_Copy32(file + 0xa0c, (u8*)&gMenuState + 112, 26);
  Mem_Free(file);
  Mem_Free(langFile);

  PlayerInfo_LoadCoPictures();
  NumberFont_LoadLow();
  TpIcon_Load();
  TpIcon2_LoadTo(data_027c0280->objVram + 0x6c0);
  Menu_Reset();
  MenuCursor_SetRect(0, -16, 256, -16);
  MenuButtons_DisableTouch();
  Wallpaper_LoadBoth();
  PlayerInfo_StartRedrawTask();
  if (Link_IsActive() == 1) {
    SpriteTask_Create(232, 8, 540, 15);
  }
}
