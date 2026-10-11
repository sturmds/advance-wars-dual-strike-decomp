#include "types.h"
#include "game/screen_other_map.h"

void func_020077a4(int value);
int func_02007710(int screens);

/* Sets up the screen effect state at start-up: both screens marked hidden,
 * effects aimed at the current screen, master brightness 0. */
void Screens_InitEffectState(void)
{
    func_020077a4(1);
    func_02007710(0);
    data_027c0280->masterBrightness = 0;
}
