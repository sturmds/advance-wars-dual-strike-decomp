#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"

/* Removes menu button 241 from screen 0 (the first entry of data_027c0288). */
void Ov006_RemoveScreen0Button241(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MenuButton_Remove(241);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
