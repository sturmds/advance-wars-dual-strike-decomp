#ifndef GAME_BREAK_TEXT_H
#define GAME_BREAK_TEXT_H

#include "types.h"

/* A line of large letters drawn as sprites, such as the CO power name in
 * the power banner. gBreakText points to one in bss. */
typedef struct BreakText {
  s16 letterX[32]; /* x of each letter, centred in its cell */
  u16 tile;        /* first OBJ tile of the letters */
  u8 palette;      /* OBJ palette of the letters */
  u8 width;        /* width of the whole line in pixels */
  u32 tagFont;     /* 1: tag power font, 32-pixel cells; else 16-pixel cells */
} BreakText;

extern BreakText* gBreakText;

extern BreakText data_0229c6c4; /* what gBreakText points to */

#endif
