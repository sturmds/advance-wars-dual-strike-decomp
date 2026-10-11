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
    u8 unk_02;                         /* counts up to unk_03 while a touch is pending */
    u8 unk_03;                         /* limit for unk_02; 20 after Touch_Init */
    u32 active;                        /* menu buttons ignore phase while this is 0 */
    u8 x;
    u8 y;
    u8 startX;                         /* where the touch began */
    u8 startY;
    u8 holdX;                          /* where the touch turned into TOUCH_HELD */
    u8 holdY;
    u8 endX;                           /* x and y copied here when the touch ends */
    u8 endY;
} TouchState;

/* One reading of the touch panel, as the ARM7 touch driver stores it. */
typedef struct TouchSample {
    u16 x;
    u16 y;
    u16 touch;                         /* 1 while the screen is touched */
    u16 validity;                      /* 0 when the sample can be used */
} TouchSample;

extern TouchState gTouch;

/* TRUE when touch input is active and phase (a copy of gTouch.phase) is wanted. */
static inline BOOL Touch_InPhase(int phase, int wanted)
{
    BOOL in;

    if (gTouch.active && phase == wanted) {
        in = TRUE;
    } else {
        in = FALSE;
    }
    return in;
}

#endif
