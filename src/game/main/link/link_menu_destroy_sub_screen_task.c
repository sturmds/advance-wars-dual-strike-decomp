#include "types.h"
#include "game/screen_other_map.h"

void PaletteCycle_Stop(void);
void DesignMap_RefreshAllRows(void);

/* Runs PaletteCycle_Stop on the sub screen (data_027c0288.unk_04), then refreshes the map name list. */
void LinkMenu_DestroySubScreenTask(void)
{
    Unk_027c0280 *saved0280 = data_027c0280;
    Unk_02080440 *saved027c;

    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    PaletteCycle_Stop();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    DesignMap_RefreshAllRows();
}
