#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/text_print.h"

void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);

/* Clears the 8x2 box at (3, 29) on layer 0 of the second screen and prints the map name (unk_d8) there. */
void Ov006_DrawNameField(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Bg_FillRect(data_027c0280->layer0Map, 3, 29, 8, 2, 0);
  Text_Print(3, 29, data_027c0280->layer0Map, data_ov006_0236d640->unk_d8, 0,
             0x300);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
