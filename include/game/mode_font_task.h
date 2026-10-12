#ifndef GAME_MODE_FONT_TASK_H
#define GAME_MODE_FONT_TASK_H

#include "types.h"

#define MODE_FONT_MAX_CHARS 16
#define MODE_FONT_SPACE 127 /* glyph for a character the font lacks */

/* The task ModeFont_StartTextTask makes (a 0x6c-byte Task) to draw one line
 * of text in the mode font. */
typedef struct ModeFontTask {
  u8 pad_00[0x29];
  s8 glyphs[MODE_FONT_MAX_CHARS]; /* 0-25: a-z, 26: '*', 27: '-', 127: none */
  u8 letterStart
      [MODE_FONT_MAX_CHARS]; /* frame each letter starts to slide in (ModeFontTask_InitLetterStarts) */
  u8 pad_49;
  u16 frame; /* frames into the current animation step */
  u16 x;     /* left edge in pixels */
  u16 y;
  u16 length; /* characters in glyphs */
  u16 tile;   /* first tile of the font */
  u16 paletteSlot;
  u16 unk_56;
  u32 unk_58;
  u8 unk_5c;
  u8 pad_5d[0x6c - 0x5d];
} ModeFontTask;

/* Advance of each glyph in pixels, before the 2 pixel overlap. */
extern u8 data_02159814[];

#endif
