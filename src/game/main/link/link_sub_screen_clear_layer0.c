#include "types.h"
#include "game/screen_other_map.h"

void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);

/* Clears layer 0 of the lower screen. */
void LinkSubScreen_ClearLayer0(void)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Gfx_ClearLayerMap(0);
    Gfx_RequestLayerCopy(0);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
