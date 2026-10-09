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
    u8 pad_01[0x04 - 0x01];
    u32 active;                        /* menu buttons ignore phase while this is 0 */
    u8 x;
    u8 y;
    u8 startX;                         /* where the touch began */
    u8 startY;
} TouchState;

extern TouchState data_0216e4d0;

#endif
