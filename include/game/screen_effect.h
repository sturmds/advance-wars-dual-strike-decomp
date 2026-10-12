#ifndef GAME_SCREEN_EFFECT_H
#define GAME_SCREEN_EFFECT_H

#include "types.h"
#include "game/task.h"

/* Which screens the window wipes and blend fades act on: ScreenEffect_GetScreens
 * returns the low nibble of data_0216e2b0 byte 1. */
#define EFFECT_SCREEN_CURRENT 0
#define EFFECT_SCREEN_BOTH 1
#define EFFECT_SCREEN_MAIN 2
#define EFFECT_SCREEN_SUB 3

int ScreenEffect_GetScreens(void);

/* Task steps that start a show (or hide) effect on the screens not yet
 * shown (or hidden): they set ScreenEffect_GetScreens to those screens and
 * return TRUE, or destroy the task and return FALSE when there are none. */
BOOL ScreenEffect_BeginShow(Task* task);
BOOL ScreenEffect_BeginHide(Task* task);

#endif
