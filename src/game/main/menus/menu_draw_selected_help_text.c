#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void Gfx_RequestLayerCopy(int layer);
void Text_PrintById(int x, int y, void* target, u32 text, int a, int b);
void Bg_ClearRect(void* target, int x, int y, int w, int h);

/* Draws the help text of the selected button (ids 8 to 11) on layer 0 of
 * the screen in data_027c0288.unk_00. */
void Menu_DrawSelectedHelpText(void) {
  u32 text;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  switch (Menu_GetSelectedId()) {
    case 8:
      text = 0x4400000a;
      break;
    case 9:
      text = 0x4400000b;
      break;
    case 10:
      text = 0x4400000c;
      break;
    case 11:
      text = 0x4400000d;
      break;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Bg_ClearRect(data_027c0280->layer0Map, 1, 19, 13, 4);
  Text_PrintById(1, 19, data_027c0280->layer0Map, text, 0, 960);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
