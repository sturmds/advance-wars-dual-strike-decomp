#include "types.h"

int Oam_PutTextChar(int x, int y, int ch);

/* Draws a string of sprite font characters from left to right, 8 pixels each.
 * Oam_PutTextChar returns 0 for the marks that sit on the previous character. */
void Oam_PutTextString(u16 x, u16 y, const u8 *str)
{
    while (*str != 0) {
        if (Oam_PutTextChar(x, y, *str++)) {
            x += 8;
        }
    }
}
