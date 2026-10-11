#include "types.h"

int func_020149c4(const u8 *text);

/* Width of text in 8-pixel tiles, rounded to the nearest tile. */
int Text_GetWidthInTiles(const u8 *text)
{
    return (func_020149c4(text) + 6) / 8;
}
