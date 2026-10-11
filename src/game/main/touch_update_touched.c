#include "types.h"
#include "game/touch.h"

/* Updates gTouch for a frame where the screen is touched. A new touch
 * becomes TOUCH_PRESSED; after unk_03 frames of pressing it becomes
 * TOUCH_HELD. unk_01 keeps the phase before each change. */
void Touch_UpdateTouched(TouchSample *sample)
{
    if (sample->validity != 0) {
        return;
    }
    switch (gTouch.phase) {
    case TOUCH_PRESSED:
        if (gTouch.active == 1) {
            gTouch.active = 0;
        }
        if (gTouch.unk_02 < gTouch.unk_03) {
            gTouch.unk_02++;
            break;
        }
        gTouch.holdX = sample->x;
        gTouch.holdY = sample->y;
        gTouch.unk_01 = gTouch.phase;
        gTouch.phase = TOUCH_HELD;
        gTouch.active = 1;
        break;
    case TOUCH_HELD:
        if (gTouch.active == 1) {
            gTouch.active = 0;
        }
        break;
    case TOUCH_RELEASED:
    default:
        gTouch.startX = sample->x;
        gTouch.startY = sample->y;
        gTouch.unk_01 = gTouch.phase;
        gTouch.phase = TOUCH_PRESSED;
        gTouch.active = 1;
        gTouch.unk_02 = 0;
        break;
    }
    gTouch.x = sample->x;
    gTouch.y = sample->y;
}
