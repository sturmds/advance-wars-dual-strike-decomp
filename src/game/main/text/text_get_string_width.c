#include "types.h"

int func_02039e34(int ch);

/* Width in pixels of a string: the sum of each character's width. */
int Text_GetStringWidth(const u8 *str)
{
    int width = 0;

    while (*str != 0) {
        width += func_02039e34(*str);
        str++;
    }
    return width;
}
