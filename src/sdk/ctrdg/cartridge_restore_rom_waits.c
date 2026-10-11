#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os.h"

void Cartridge_RestoreRomWaits(GbaSlotWaitStates *timing)
{
    REG_EXMEMCNT = (timing->romFirstAccess << GBA_SLOT_INITIAL_WAIT_POS)
                 | (REG_EXMEMCNT & ~GBA_SLOT_INITIAL_WAIT_BITS);
    REG_EXMEMCNT = (timing->romSecondAccess << GBA_SLOT_FOLLOWUP_WAIT_POS)
                 | (REG_EXMEMCNT & ~GBA_SLOT_FOLLOWUP_WAIT_BIT);
}
