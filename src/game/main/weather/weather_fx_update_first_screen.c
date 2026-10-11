#include "types.h"
#include "game/screen_other_map.h"

void func_020d1c98(void);

/* Runs func_020d1c98 with the first map screen (data_027c0288.unk_00) made
 * current, then restores the current screen. */
void WeatherFx_UpdateFirstScreen(void)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    func_020d1c98();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
