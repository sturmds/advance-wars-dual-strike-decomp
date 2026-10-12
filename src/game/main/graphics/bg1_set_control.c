#include "types.h"
#include "sdk/gx.h"

/* BG1CNT version of Bg2_SetControl, with the extended palette bit (13). */
void Bg1_SetControl(int screenSize, int colorMode, int screenBase, int charBase,
                    int extPltt) {
  *REG_BG1CNT = (*REG_BG1CNT & 0x43) | (screenSize << 14) | (colorMode << 7) |
                (screenBase << 8) | (charBase << 2) | (extPltt << 13);
}
