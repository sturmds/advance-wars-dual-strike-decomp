#include "types.h"

char gWlVersionString[8] = {
#include "data/arm7/gWlVersionString.inc.c"
};
/* REG_W_TXBUF_RESET bit of slots LOC1, LOC2, LOC3 */
u16 gWlTxSlotResetBits[4] = {
#include "data/arm7/gWlTxSlotResetBits.inc.c"
};
