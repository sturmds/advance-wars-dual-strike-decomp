#ifndef GAME_BIG_FONT_H
#define GAME_BIG_FONT_H

#include "types.h"

/* One glyph of the large sprite font in data_0215bf38's file; the table
 * (data_0215bce0) ends with a glyph whose character is 0. */
typedef struct BigFontGlyph {
  u32 offset; /* byte offset of the packed tiles in the file */
  u16 character;
  u16 width; /* in pixels; each glyph sits in a 32-pixel cell */
} BigFontGlyph;

#endif
