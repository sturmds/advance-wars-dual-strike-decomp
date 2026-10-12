#ifndef GAME_SCREEN_WIPE_H
#define GAME_SCREEN_WIPE_H

#include "types.h"
#include "game/screen_effect.h"

/* Screen wipes: task updates that move the display windows a step each frame
 * until the progress reaches its end, then resume the task's script. */

/* ScreenEffect_GetScreens return values: the screens a wipe runs on. */
#define WIPE_CURRENT 0 /* the current screen only */
#define WIPE_BOTH 1
#define WIPE_SCREEN0 2 /* data_027c0288.unk_00 */
#define WIPE_SCREEN1 3 /* data_027c0288.unk_04 */

extern s16
    data_0216e29c; /* eased value; ScreenWipe_EaseDownUpdate takes it from 352 to 0 */
extern s16 data_0216e2a8; /* wipe progress, from 0 up to the wipe's end */

#endif
