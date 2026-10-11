#include "types.h"

extern u8 Ov000_gGlyphWidths[];       /* glyph width in pixels, per character */
extern u8 *Ov000_gGlyphPixels[];      /* glyph pixels, 4 bits each, (width + 1) / 2 bytes per row */

/* One 8-pixel row of a 4-bit tile. */
typedef union GlyphRow {
    u32 word;
    u16 half[2];
    u8 byte[4];
} GlyphRow;

/* Draws character c, 16 pixels high, into the 4-bit tiles at dst, starting
 * at pixel x & 7 of the tile column. The glyph spills into the next tile
 * column (32 halfwords on) when it does not fit. Pixels 0xa take colour
 * 0xa + colour. A tile column is cleared before the first pixel is drawn
 * into it. Returns the glyph width. */
int Font_DrawGlyph(int c, u16 *dst, int x, int colour)
{
    int row;
    u8 width;
    GlyphRow rows[2];
    int col;
    int i;
    int b;
    u8 *src;
    int shift;

    shift = x & 7;
    width = Ov000_gGlyphWidths[c];
    if (shift <= 1) {
        for (row = 0; row < 32; row++) {
            dst[row] = 0;
        }
    }
    if (shift + width > 8) {
        int offset = ((shift + width) & 8) * 4;

        for (i = 0; i < 32; i++) {
            dst[i + offset] = 0;
        }
    }
    if (width != 0) {
        src = Ov000_gGlyphPixels[c];
        /* needed to match: x is reused for the bytes per glyph row */
        x = (width + 1) >> 1;
        for (row = 0; row < 16; row++) {
            rows[1].word = 0;
            rows[0].word = 0;
            for (col = 0; col < x; col++) {
                b = *src;
                if ((b & 0xf0) == 0xa0) {
                    b += colour << 4;
                }
                if ((b & 0xf) == 10) {
                    b += colour;
                }
                rows[1].byte[col] = b;
                rows[0].byte[col] = rows[1].byte[col];
                src++;
            }
            rows[0].word <<= shift * 4;
            dst[0] |= rows[0].half[0];
            dst[1] |= rows[0].half[1];
            rows[1].word >>= (8 - shift) * 4;
            if (rows[1].word != 0) {
                dst[32] |= rows[1].half[0];
                dst[33] |= rows[1].half[1];
            }
            dst += 2;
        }
    }
    return width;
}
