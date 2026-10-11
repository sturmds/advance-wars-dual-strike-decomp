#ifndef GAME_SCREEN_EFFECT_H
#define GAME_SCREEN_EFFECT_H

#include "types.h"

/* Which screens the window wipes and blend fades act on: func_020076f8
 * returns the low nibble of data_0216e2b0 byte 1. */
#define EFFECT_SCREEN_CURRENT 0
#define EFFECT_SCREEN_BOTH    1
#define EFFECT_SCREEN_MAIN    2
#define EFFECT_SCREEN_SUB     3

int func_020076f8(void);

#endif
