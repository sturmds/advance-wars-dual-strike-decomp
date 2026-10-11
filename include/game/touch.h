#ifndef GAME_TOUCH_H
#define GAME_TOUCH_H

#include "types.h"

/* Values of TouchState.phase, as menu buttons read them. Guessed from
 * MenuButton_HandleTouch: a button confirms on TOUCH_RELEASED after
 * TOUCH_PRESSED, and TOUCH_HELD on it cancels the press. */
#define TOUCH_RELEASED 0
#define TOUCH_PRESSED  1
#define TOUCH_HELD     2

/* Touch screen state of the current frame, in screen pixels. */
typedef struct TouchState {
    u8 phase;                          /* TOUCH_* */
    u8 unk_01;                         /* 1 or 2 on release: Ov006 reads it as the kind of touch that ended */
    u8 pad_02[0x04 - 0x02];
    u32 active;                        /* menu buttons ignore phase while this is 0 */
    u8 x;
    u8 y;
    u8 startX;                         /* where the touch began */
    u8 startY;
    u8 pad_0c[0x0e - 0x0c];
    u8 endX;                           /* x and y copied here when the touch ends */
    u8 endY;
} TouchState;

extern TouchState gTouch;

#endif
