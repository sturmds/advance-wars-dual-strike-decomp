#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os_arm7.h"

void func_arm7_038065c4(GbaSlotWaitStates *saved)
{
    saved->romFirstAccess = (REG_EXMEMCNT & GBA_SLOT_INITIAL_WAIT_BITS) >> GBA_SLOT_INITIAL_WAIT_POS;
    saved->romSecondAccess = (REG_EXMEMCNT & GBA_SLOT_FOLLOWUP_WAIT_BIT) >> GBA_SLOT_FOLLOWUP_WAIT_POS;
    REG_EXMEMCNT = (REG_EXMEMCNT & ~GBA_SLOT_INITIAL_WAIT_BITS) | GBA_SLOT_INITIAL_WAIT_BITS;
    REG_EXMEMCNT = REG_EXMEMCNT & ~GBA_SLOT_FOLLOWUP_WAIT_BIT;
}
