#include "types.h"
#include "sdk/gx.h"

/* Sets screen size, color mode, screen base and char base in BG2CNT. The
 * priority and mosaic bits (mask 0x43) stay. */
void Bg2_SetControl(int screenSize, int colorMode, int screenBase,
                    int charBase) {
  *REG_BG2CNT = (*REG_BG2CNT & 0x43) | (screenSize << 14) | (colorMode << 7) |
                (screenBase << 8) | (charBase << 2);
}
