#include "types.h"

void Screen_ApplyMain(void);
void Screen_PrepareHBlankRegs(void);
void Screen_ApplySub(void);

/* Writes both screen states to the display registers: the main screen
 * (0x04000000), the DTCM copies Screen_PrepareHBlankRegs prepares, then the sub
 * screen (0x04001000). */
void Screen_ApplyAll(void) {
  Screen_ApplyMain();
  Screen_PrepareHBlankRegs();
  Screen_ApplySub();
}
