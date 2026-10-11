#ifndef GAME_BRIGHTNESS_FADE_H
#define GAME_BRIGHTNESS_FADE_H

#include "types.h"
#include "game/task.h"

/* func_020fcdf0 starts a task that fades one screen's brightness over
 * `frames` frames and returns it. `out` nonzero fades from 0 to the target
 * level, zero from the target level back to 0. `screen` picks the screen
 * (0 or 1 in data_027c0288). `white` nonzero makes the level +16 (white, a guess), zero -16.
 * The wrappers below fix the three flags; parentOrList is Task_Create's
 * second argument (a parent task, or TASK_LIST(n)). */
Task *func_020fcdf0(void *parentOrList, int out, int screen, int white, int frames);

Task *func_020fccd8(void *parentOrList, int frames); /* out, screen 0, white */
Task *func_020fcd00(void *parentOrList, int frames); /* in, screen 1, white */
Task *func_020fcd28(void *parentOrList, int frames); /* in, screen 0, white */
Task *func_020fcd50(void *parentOrList, int frames); /* out, screen 1, black */
Task *func_020fcd78(void *parentOrList, int frames); /* out, screen 0, black */
Task *func_020fcda0(void *parentOrList, int frames); /* in, screen 1, black */
Task *func_020fcdc8(void *parentOrList, int frames); /* in, screen 0, black */

#endif
