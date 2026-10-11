#include "types.h"

int func_020babb4(const char *text);
void Oam_PutTextString(u16 x, u16 y, const char *text);

/* Draws text on the current screen, centered across its 256 pixels. */
void Text_DrawCentered(int y, const char *text)
{
    Oam_PutTextString(func_020babb4(text), y, text);
}
