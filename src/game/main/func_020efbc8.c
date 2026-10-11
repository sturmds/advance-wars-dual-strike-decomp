#include "types.h"

/* TRUE when the touch (x, y) is on the 32x16 button at x 144, y 0. */
BOOL func_020efbc8(int x, int y)
{
    if (x >= 144 && x <= 176 && y >= 0 && y <= 16) {
        return TRUE;
    }
    return FALSE;
}
