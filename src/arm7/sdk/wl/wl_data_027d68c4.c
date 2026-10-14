#include "types.h"
#include "sdk/wl.h"

u16 gWlTestPatterns[] = {
#include "data/arm7/gWlTestPatterns.inc.c"
};
u16 gWlBbSkipRegsRf5[] = {
#include "data/arm7/gWlBbSkipRegsRf5.inc.c"
};
u16 gWlBbSkipRegs[] = {
#include "data/arm7/gWlBbSkipRegs.inc.c"
};
WlRegMask gWlTestRegs[] = {
#include "data/arm7/gWlTestRegs.inc.c"
};
