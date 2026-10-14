#include "types.h"

u16 gWlTestPatterns[3] = {
#include "data/arm7/gWlTestPatterns.inc.c"
};
u16 gWlBbSkipRegsRf5[26] = {
#include "data/arm7/gWlBbSkipRegsRf5.inc.c"
};
u16 gWlBbSkipRegs[28] = {
#include "data/arm7/gWlBbSkipRegs.inc.c"
};
/* declared `const WlRegMask gWlTestRegs[WIFI_SELFTEST_MAC_TARGETS]` in wifi_self_test_mac_registers.c: declared in src/arm7/sdk/wl/wifi_self_test_mac_registers.c, with a type no header defines */
u32 gWlTestRegs[27] = {
#include "data/arm7/gWlTestRegs.inc.c"
};
