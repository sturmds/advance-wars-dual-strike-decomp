#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/Unk_0229fb30.h"

void Bg_ClearRect(u16* map, int x, int y, int w, int h);
void Text_PrintById(int x, int y, void* target, u32 text, int a, int b);
void Gfx_RequestLayerCopy(int layer);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Prints the text of the shop button (22-25) the local player's row has
 * picked, texts 0x4400000a-0x4400000d, in a 13 by 4 tile box at (1, 19) of
 * the main screen's layer 0. */
void CombatSetup_DrawShopHelp(void) {
  u32 text;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  switch (MENU->combat.rows[MENU->combat.players[data_0229fb30.unk_45a].row - 1]
              .buttonId) {
    case 22:
      text = 0x4400000a;
      break;
    case 23:
      text = 0x4400000b;
      break;
    case 24:
      text = 0x4400000c;
      break;
    case 25:
      text = 0x4400000d;
      break;
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Bg_ClearRect(data_027c0280->layer0Map, 1, 19, 13, 4);
  Text_PrintById(1, 19, data_027c0280->layer0Map, text, 0, 960);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
