#include "types.h"
#include "game/Unk_ov005_0235b574.h"

/* Needed to match: propagation off keeps the four shifts as their own instructions, and the
 * sums written in the loop are hoisted after them. The order of the assignments and the
 * declarations (src before row) sets the registers. */
#pragma opt_propagation off
#pragma opt_strength_reduction off
#pragma opt_loop_invariants off

/* Needed to match: the destination row is written out in each store, not kept in a pointer. */
#define ROW_DST (dst + (cellLo + cellHi) + (row << 7))

/* Copies glyph `glyph` of the font in buffer_0010 to cell `cell` of the texture in
 * buffer_6010: 8 rows of 4 bytes from each of the two planes. */
void Ov005_DrawGlyphToTexture(Unk_ov005_0235b574* work, int cell, int glyph) {
  int cellLo;
  int glyphLo;
  int cellHi;
  int glyphHi;
  int col;
  u8* src;
  int row;
  u8* dst;
  u8* s;

  col = 0;
  row = 0;
  src = work->buffer_0010;
  dst = work->buffer_6010;
  cellLo = cell & 31;
  cellLo <<= 2;
  glyphLo = glyph & 31;
  glyphLo <<= 5;
  cellHi = cell & 0xffe0;
  cellHi <<= 6;
  glyphHi = glyph & 0xffe0;
  glyphHi <<= 6;
  do {
    s = src + col + (glyphLo + glyphHi);

    ROW_DST[0] = s[0];
    ROW_DST[0x400] = s[0x400];
    ROW_DST[1] = s[1];
    ROW_DST[0x401] = s[0x401];
    ROW_DST[2] = s[2];
    ROW_DST[0x402] = s[0x402];
    ROW_DST[3] = s[3];
    ROW_DST[0x403] = s[0x403];
    col += 4;
    row++;
  } while (col < 32);
}
