#include "types.h"
#include "game/screen_other_map.h"

void WeatherFx_Update(void);

/* Runs WeatherFx_Update with the first map screen (data_027c0288.unk_00) made
 * current, then restores the current screen. */
void WeatherFx_UpdateFirstScreen(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  WeatherFx_Update();
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
