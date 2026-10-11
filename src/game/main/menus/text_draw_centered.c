#include "types.h"

int func_020babb4(const char *text);
void func_01ff9a3c(u16 x, u16 y, const char *text);

/* Draws text on the current screen, centered across its 256 pixels. */
void Text_DrawCentered(int y, const char *text)
{
    func_01ff9a3c(func_020babb4(text), y, text);
}
