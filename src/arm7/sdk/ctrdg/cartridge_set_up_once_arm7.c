#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/pxi.h"

extern u32 data_arm7_0380afd0;

/* ARM7 copy of CTRDG_Init. */
void Cartridge_SetUpOnce_Arm7(void)
{
    if (data_arm7_0380afd0 != 0) {
        return;
    }
    data_arm7_0380afd0 = 1;
    Cartridge_ZeroStateTakeLockId_Arm7();
    Fifo_SetUp_Arm7();
    Fifo_RegisterTagHandler_Arm7(FIFO_TAG_GBA_SLOT, Cartridge_StoreInitDoneWord);
    Cartridge_CheckLogoFromArm9Once();
    Fifo_RegisterTagHandler_Arm7(FIFO_TAG_GBA_SLOT, Cartridge_OnShutdownMessage);
}
