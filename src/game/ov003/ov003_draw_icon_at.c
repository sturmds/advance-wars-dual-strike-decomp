#include "types.h"
#include "game/ov003_unit.h"

void func_ov003_02353744(int x, int y, int a, int b, int c, int d, int e, int f, int g);

/* Calls func_ov003_02353744 for the point (x, y) shifted by (-9, -4) from the
 * camera, unless (x, y) is off screen. What it draws is not known. */
void Ov003_DrawIconAt(int x, int y)
{
    if (x < OV003_SCROLL_X - 22 || x > OV003_SCROLL_X + 265) {
        return;
    }
    if (y < OV003_SCROLL_Y - 11 || y > OV003_SCROLL_Y + 196) {
        return;
    }
    func_ov003_02353744(x - OV003_SCROLL_X - 9, y - OV003_SCROLL_Y - 4, 23, 2, 1, 0x4c8, 17, 0, 0);
}
