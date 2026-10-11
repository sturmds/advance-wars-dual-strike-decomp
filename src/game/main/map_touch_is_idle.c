#include "types.h"
#include "game/touch.h"

extern s16 gEventScriptOwnsInput;

/* TRUE when no script holds input off and the screen is not touched. */
BOOL MapTouch_IsIdle(void)
{
    if (gEventScriptOwnsInput != 0) {
        return FALSE;
    }
    return gTouch.phase == TOUCH_RELEASED;
}
