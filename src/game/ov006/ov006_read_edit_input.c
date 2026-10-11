#include "types.h"
#include "game/keypad.h"
#include "game/touch.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690 *data_ov006_0236d640;

#define OV6 data_ov006_0236d640

/* Reads what the player asks of the cursor tile this frame: 1 to use the
 * tool (A, or a touch), 2 for X (likely the pick in Ov006_PickToolAtCursor),
 * 0 for nothing; always 0 while flag 0x10 is set. A newly pressed key always
 * counts. A held key counts only once the cursor has moved off the tile it
 * was on last frame (prevCursorX, prevCursorY), so holding A paints a line without
 * repeating on one tile. A new touch, or a touch on the same tile, counts
 * only below the top 32 pixels (touchY > 32). */
int Ov006_ReadEditInput(void)
{
    u16 flags = OV6->unk_00;
    int pressed;

    if (flags & 0x10) {
        return 0;
    }
    if (gKeypad->pressed & PAD_BUTTON_A) {
        return 1;
    }
    if (gKeypad->pressed & PAD_BUTTON_X) {
        return 2;
    }
    if (flags & 0x40) {
        if (gTouch.active != 0 && gTouch.phase == TOUCH_PRESSED) {
            pressed = TRUE;
        } else {
            pressed = FALSE;
        }
        if (pressed == TRUE) {
            return OV6->touchY > 32;
        }
    }
    if (OV6->cursorX == OV6->prevCursorX && OV6->cursorY == OV6->prevCursorY) {
        if (gTouch.phase != 0 && gTouch.prevPhase == 0) {
            return OV6->touchY > 32;
        }
        return 0;
    }
    if (gKeypad->held & PAD_BUTTON_A) {
        return 1;
    }
    if (gKeypad->held & PAD_BUTTON_X) {
        return gTouch.phase == 0 ? 2 : 0;
    }
    return gTouch.phase != 0;
}
