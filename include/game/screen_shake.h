#ifndef GAME_SCREEN_SHAKE_H
#define GAME_SCREEN_SHAKE_H

#include "types.h"

/* Offset in pixels that the screen shake subtracts from the BG scroll and
 * the sprite positions. */
extern volatile s16 gScreenShakeX;
extern volatile s16 gScreenShakeY;

#endif
