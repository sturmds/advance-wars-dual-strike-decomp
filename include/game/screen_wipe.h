#ifndef GAME_SCREEN_WIPE_H
#define GAME_SCREEN_WIPE_H

#include "types.h"

/* Screen wipes: task updates that move the display windows a step each frame
 * until the progress reaches its end, then resume the task's script. */

/* func_020076f8 return values: the screens a wipe runs on. */
#define WIPE_CURRENT 0                     /* the current screen only */
#define WIPE_BOTH 1
#define WIPE_SCREEN0 2                     /* data_027c0288.unk_00 */
#define WIPE_SCREEN1 3                     /* data_027c0288.unk_04 */

int func_020076f8(void);

extern u16 data_0216e29c;
extern s16 data_0216e2a8;                  /* wipe progress, from 0 up to the wipe's end */

#endif
