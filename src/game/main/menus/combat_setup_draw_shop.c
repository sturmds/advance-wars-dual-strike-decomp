#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"

void Bg_ClearRect(u16* map, int x, int y, int width, u16 height);
void Gfx_RequestLayerCopy(int layer);
void Number_Draw(int x, int y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet);
void Text_PrintImmediate(int x, int y, u16* map, u32 textId, u16 unk_30,
                         int arg);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Draws the funds of the local player's Combat row and the four units on
 * sale with their prices (1000, 3000, 5000, 6000) on layer 0 of the main
 * screen. A unit is drawn in the second style when the row is full (20
 * units) or its funds are below the price. */
void CombatSetup_DrawShop(void) {
  int row;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int cannotBuy;

  row = MENU->combat.players[data_0229fb30.unk_45a].row;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Bg_ClearRect(data_027c0280->layer0Map, 1, 1, 13, 10);
  Text_PrintImmediate(1, 1, data_027c0280->layer0Map, 0x44000002, 0, 0);
  Number_Draw(13, 1, data_027c0280->layer0Map, LINK_ROWS[row - 1].funds, 0, 0);

  if (LINK_ROWS[row - 1].spawnCount != 20 && LINK_ROWS[row - 1].funds >= 1000) {
    cannotBuy = 0;
  } else {
    cannotBuy = 1;
  }
  /* Needed to match: the original narrows the flag to u16 here too. */
  Text_PrintImmediate(3, 3, data_027c0280->layer0Map, 0xc0000001, 0,
                      (u16)cannotBuy);
  Number_Draw(13, 3, data_027c0280->layer0Map, 1000, 0, cannotBuy);

  if (LINK_ROWS[row - 1].spawnCount != 20 && LINK_ROWS[row - 1].funds >= 3000) {
    cannotBuy = 0;
  } else {
    cannotBuy = 1;
  }
  Text_PrintImmediate(3, 5, data_027c0280->layer0Map, 0xc0000005, 0,
                      (u16)cannotBuy);
  Number_Draw(13, 5, data_027c0280->layer0Map, 3000, 0, cannotBuy);

  if (LINK_ROWS[row - 1].spawnCount != 20 && LINK_ROWS[row - 1].funds >= 5000) {
    cannotBuy = 0;
  } else {
    cannotBuy = 1;
  }
  Text_PrintImmediate(3, 7, data_027c0280->layer0Map, 0xc0000004, 0,
                      (u16)cannotBuy);
  Number_Draw(13, 7, data_027c0280->layer0Map, 5000, 0, cannotBuy);

  if (LINK_ROWS[row - 1].spawnCount != 20 && LINK_ROWS[row - 1].funds >= 6000) {
    cannotBuy = 0;
  } else {
    cannotBuy = 1;
  }
  Text_PrintImmediate(3, 9, data_027c0280->layer0Map, 0xc0000007, 0,
                      (u16)cannotBuy);
  Number_Draw(13, 9, data_027c0280->layer0Map, 6000, 0, cannotBuy);

  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
