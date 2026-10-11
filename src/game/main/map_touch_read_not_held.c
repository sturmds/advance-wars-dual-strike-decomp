#include "types.h"
#include "game/touch.h"

extern s16 data_0229777c;

/* Nonzero (32) unless a script holds input off or the touch is held down. */
u32 MapTouch_ReadNotHeld(void)
{
    if (data_0229777c != 0) {
        return 0;
    }
    switch (gTouch.phase) {
    case TOUCH_RELEASED:
    case TOUCH_PRESSED:
    default:
        return 0x20;
    case TOUCH_HELD:
        return 0;
    }
}
