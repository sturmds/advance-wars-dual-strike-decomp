#include "types.h"

int Text_GetCenteredX(const char* text);
void Oam_PutTextString(u16 x, u16 y, const char* text);

/* Draws text on the current screen, centered across its 256 pixels. */
void Text_DrawCentered(int y, const char* text) {
  Oam_PutTextString(Text_GetCenteredX(text), y, text);
}
