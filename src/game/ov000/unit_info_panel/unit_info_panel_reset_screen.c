#include "types.h"
#include "game/unit_info_panel.h"

void func_01ff8bc8(int layer, int scrollX, int scrollY);
void func_0201e6e8(int x, int y, int width, int height);
void Screen_ResetWindows(void);

/* Sets the panel's position, scrolls layers 0 and 1 to the origin, draws a
 * 16 by 10 tile frame on layer 1 at that position and resets the windows. */
void UnitInfoPanel_InitScreen(void)
{
    data_ov000_0234ec8c->tileX = 8;
    data_ov000_0234ec8c->tileY = 7;
    func_01ff8bc8(0, 0, 0);
    func_01ff8bc8(1, 0, 0);
    func_0201e6e8(8, 7, 16, 10);
    Screen_ResetWindows();
}
