#include "types.h"

/* TRUE when the touch (x, y) is on the 32x16 button at x 80, y 0. */
BOOL MapTouch_InLeftButton(int x, int y)
{
    if (x >= 80 && x <= 112 && y >= 0 && y <= 16) {
        return TRUE;
    }
    return FALSE;
}
