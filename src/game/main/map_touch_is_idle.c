#include "types.h"
#include "game/touch.h"

extern s16 data_0229777c;

/* TRUE when no script holds input off and the screen is not touched. */
BOOL MapTouch_IsIdle(void)
{
    if (data_0229777c != 0) {
        return FALSE;
    }
    return gTouch.phase == TOUCH_RELEASED;
}
