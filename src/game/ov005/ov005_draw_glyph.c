#include "types.h"
#include "game/Unk_ov005_0235b574.h"

/* Copies glyph `glyph` of the font in buffer_0010 to text cell `cell` of the tiles in
 * buffer_2010: 8 rows of 4 bytes from each of the two planes. */
void Ov005_DrawGlyph(Unk_ov005_0235b574 *work, int cell, int glyph)
{
    int cellOffset;
    int glyphOffset;
    u8 *font;
    u8 *tiles;
    int col;

    /* Needed to match: the buffers before the offsets, and the row pointers
     * built inside the loop, so that mwcc hoists them in this order. */
    font = work->buffer_0010;
    tiles = work->buffer_2010;
    cellOffset = ((cell & 3) << 5) + ((cell & 0xfffc) << 6);
    glyphOffset = ((glyph & 31) << 5) + ((glyph & 0xffe0) << 6);
    for (col = 0; col < 32; col += 4) {
        u8 *s = font + glyphOffset + col;
        u8 *d = tiles + cellOffset + col;

        d[0] = s[0];
        d[1] = s[1];
        d[2] = s[2];
        d[3] = s[3];
        d[0x80] = s[0x400];
        d[0x81] = s[0x401];
        d[0x82] = s[0x402];
        d[0x83] = s[0x403];
    }
}
