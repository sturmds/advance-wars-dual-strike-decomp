#include "types.h"
#include "game/screen_other_map.h"

void func_020075c0(void);
void Screen_ApplyAll(void);

/* Turns the current screen fully white: brighten blend at level 16 / 16. */
void Screen_FillWhite(void)
{
    func_020075c0();
    data_027c0280->blendMode = BLEND_MODE_BRIGHTEN;
    data_027c0280->blendEva = 0;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 16;
    Screen_ApplyAll();
}
