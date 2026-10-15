#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"
#include "game/screen_other_map.h"

/* Sets up icon bank 4 on the first screen (tile 576, palette slot 23) and loads
 * icons 150 to 159 and 162 into it. 162 is loaded twice. */
void Ov005SoundRoom_LoadIcons(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  IconBank_Init(4, data_027c0280->objVram, 576, 23);
  Icon_Load(150);
  Icon_Load(151);
  Icon_Load(152);
  Icon_Load(153);
  Icon_Load(154);
  Icon_Load(155);
  Icon_Load(156);
  Icon_Load(157);
  Icon_Load(158);
  Icon_Load(159);
  Icon_Load(162);
  Icon_Load(162);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
