#include "types.h"
#include "game/ov003_unit.h"

void func_ov003_02353744(int x, int y, int a, int b, int c, int d, int e, int f, int g);

/* Like Ov003_DrawIconAt, but the shift is (-1, -4) and frame picks the fifth
 * argument of func_ov003_02353744 from a table of 4-byte steps at 0x47c. */
void func_ov003_023576d0(int x, int y, int frame)
{
    if (x < OV003_SCROLL_X - 14 || x > OV003_SCROLL_X + 257) {
        return;
    }
    if (y < OV003_SCROLL_Y - 11 || y > OV003_SCROLL_Y + 196) {
        return;
    }
    func_ov003_02353744(x - OV003_SCROLL_X - 1, y - OV003_SCROLL_Y - 4, 23, 1, 1, 0x47c + (frame - 1) * 4, 17, 0, 0);
}
