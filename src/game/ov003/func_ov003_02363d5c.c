#include "types.h"

int Ov003_GetGlyphWidth(int ch);

int func_ov003_02363d5c(const u8 *str)
{
    int width = 0;

    while (*str != 0) {
        width += Ov003_GetGlyphWidth(*str);
        str++;
    }
    return width;
}
