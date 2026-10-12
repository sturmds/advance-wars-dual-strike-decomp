#include "types.h"
#include "sdk/gx.h"

void Screen_ResetAndShowMessage(void);

void func_0201fb3c(void) {
  *REG_POWCNT1 &= ~POWCNT1_DISPLAY_SWAP;
  Screen_ResetAndShowMessage();
}
