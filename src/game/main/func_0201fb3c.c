#include "types.h"
#include "sdk/gx.h"

void func_0201efd8(void);

void func_0201fb3c(void)
{
    *REG_POWCNT1 &= ~POWCNT1_DISPLAY_SWAP;
    func_0201efd8();
}
