#include "types.h"
#include "game/icon.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
/* Needed to match: data_022a7a9c is read with its rows (as in
 * battle_results_fill.c), so hide the header's one-entry declaration. */
#define data_022a7a9c data_022a7a9c_entry
#include "game/Unk_022a7a9c.h"
#include "game/co_data.h"
#undef data_022a7a9c

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
u8 Map_GetLoadedPlayerCount(void);
const char* Map_GetCurrentName(void);
void Screen_SetLayerScreenSize(int layer, int screenSize);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);
void Gfx_LoadBmap09f(void* target, int a, int b);
void Font_LoadDigits(int a);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const char* text, u16 a,
                               u16 b);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Palette_CopyBlock(
    int block,
    u8* dest); /* copies 0x100 bytes of palette block `block` to dest */
void func_0200208c(int coId, void* dest, int paletteSlot, int player);
void Co_UnpackPicture(int coId, int firstTile);
const void* Terrain_GetPalette(int id, int army);
void Palette_Load(
    const void* src, u16 offset,
    u32 size); /* copies into the current screen's palette buffer */
void Palette_LoadObjRow(int row);
const void* Player_GetPalette(int player);
u8* UnitIcons_GetTiles(void);
int UnitIcons_GetTileCount(void);
void ResultStats_DrawPlayerTotals(int player);
void ResultStats_PrintFirstPageLabel(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern BattleResults data_022a7a9c;
extern u8 gMenuState[];
extern u8 data_0229eb70[]; /* gMenuState + 0x40 */

#define GAME (data_027c027c->unk_48[data_027c0284])
#define ROW(player) (data_022a7a9c.rows[player])

#define MAIN_MAP (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

/* SWITCH_TO opens a block with its own saved pointers, which SWITCH_BACK
 * restores and closes. Needed to match: every switch keeps its saved
 * pointers in its own stack slots. */
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

/* First step of the battle stats screen. Starts on player 1
 * (task->unk_66), page 0 (task->unk_68), with task->unk_6a players. On
 * both screens: sets up layers 0 and 1, copies a layer 1 tile map from
 * gMenuState, prints the map name and the day, and loads the army icon of
 * each player and icons 140-149 and 193. On the sub screen it loads both
 * CO pictures and the army palette of each player, on the main screen the
 * player palettes. Then draws player 1's page. */
void ResultStats_Init(Task* task) {
  const char* mapName;
  int day;
  int i;

  SWITCH_TO(MAIN_MAP);
  task->unk_66 = 1;
  task->unk_68 = 0;
  task->unk_6a = Map_GetLoadedPlayerCount();
  mapName = Map_GetCurrentName();
  day = GAME->day;
  SWITCH_BACK();

  {
    Unk_027c0280* saved0280 = data_027c0280;
    int screen; /* declared here to match its stack slot */
    Unk_02080440* saved027c = data_027c027c;

    for (screen = 0;
         screen < (int)(sizeof(data_027c0288) / sizeof(data_027c0288.unk_00));
         screen++) {
      Screen_SetCurrentBoth(((void**)&data_027c0288)[screen]);
      Screen_SetLayerScreenSize(0, 1);
      Screen_SetLayerScreenSize(1, 1);
      data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
      data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
      data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0xff00;
      data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0xff00;
      Gfx_ClearLayerMap(0);
      Gfx_ClearLayerMap(1);
      Gfx_RequestLayerCopy(0);
      Gfx_RequestLayerCopy(1);
      Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
      Font_LoadDigits(0);
      /* Text 0xc00000ad goes left of the day number, one tile
             * further left for each extra digit. */
      if (data_027c027c == MAIN_MAP) {
        Mem_Copy32(data_0229eb70, data_027c0280->layer1Map, 0x5c0);
        Text_PrintStringImmediate(8, 2, data_027c0280->layer0Map, mapName, 0,
                                  0);
        Number_Draw(28, 2, data_027c0280->layer0Map, day, 0, 0);
        if (day >= 100) {
          Text_PrintImmediate(23, 2, data_027c0280->layer0Map, 0xc00000ad, 0,
                              0);
        } else if (day >= 10) {
          Text_PrintImmediate(24, 2, data_027c0280->layer0Map, 0xc00000ad, 0,
                              0);
        } else {
          Text_PrintImmediate(25, 2, data_027c0280->layer0Map, 0xc00000ad, 0,
                              0);
        }
      } else {
        Mem_Copy32(gMenuState, data_027c0280->layer1Map, BG_VISIBLE_SIZE);
        Text_PrintStringImmediate(8, 3, data_027c0280->layer0Map, mapName, 0,
                                  0);
        Number_Draw(28, 3, data_027c0280->layer0Map, day, 0, 0);
        if (day >= 100) {
          Text_PrintImmediate(23, 3, data_027c0280->layer0Map, 0xc00000ad, 0,
                              0);
        } else if (day >= 10) {
          Text_PrintImmediate(24, 3, data_027c0280->layer0Map, 0xc00000ad, 0,
                              0);
        } else {
          Text_PrintImmediate(25, 3, data_027c0280->layer0Map, 0xc00000ad, 0,
                              0);
        }
      }
      for (i = 0; i < task->unk_6a; i++) {
        Icon_LoadTo(ROW(i + 1).armyColour + 74, data_027c0280->objVram, i * 8);
      }
      IconBank_LoadPalette(1, 16);
      for (i = 0; i < ICON_DIGIT_COUNT; i++) {
        Icon_LoadTo(ICON_DIGITS + i, data_027c0280->objVram, i * 10 + 34);
      }
      Icon_LoadTo(193, data_027c0280->objVram, 32);
      IconBank_LoadPalette(4, 17);
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }

  /* CO pictures: front CO in picture 2 * i, tag partner in 2 * i + 1,
     * 12 tiles each from tile 174 (palette 18 + picture). */
  SWITCH_TO(SECOND_MAP);
  Palette_CopyBlock(6, data_027c0280->objVram + 0x10c0);
  Palette_CopyBlock(14, data_027c0280->objVram + 0x11c0);
  Palette_CopyBlock(10, data_027c0280->objVram + 0x12c0);
  Palette_CopyBlock(11, data_027c0280->objVram + 0x13c0);
  Palette_CopyBlock(22, data_027c0280->objVram + 0x14c0);
  for (i = 0; i < task->unk_6a; i++) {
    func_0200208c(ROW(i + 1).unk_00,
                  data_027c0280->objVram + ((i * 2 * 12 + 174) << 5),
                  i * 2 + 18, i + 1);
    Co_UnpackPicture(ROW(i + 1).unk_00 & CO_ID_MASK, i * 2 * 12 + 0x10e);
    if (ROW(i + 1).unk_02 & 0x7f) {
      func_0200208c(ROW(i + 1).unk_02,
                    data_027c0280->objVram + (((i * 2 + 1) * 12 + 174) << 5),
                    i * 2 + 19, i + 1);
      Co_UnpackPicture(ROW(i + 1).unk_02 & 0x7f, (i * 2 + 1) * 12 + 0x10e);
    }
    Palette_Load(Terrain_GetPalette(6, ROW(i + 1).armyColour), (i + 27) << 5,
                 32);
  }
  Palette_LoadObjRow(10);
  SWITCH_BACK();

  SWITCH_TO(MAIN_MAP);
  Mem_Copy32(UnitIcons_GetTiles(), data_027c0280->objVram + 0x10c0,
             UnitIcons_GetTileCount() << 5);
  for (i = 0; i < task->unk_6a; i++) {
    Palette_Load(Player_GetPalette(i + 1), (i + 18) << 5, 32);
  }
  SWITCH_BACK();
  ResultStats_DrawPlayerTotals(1);
  ResultStats_PrintFirstPageLabel();
}
