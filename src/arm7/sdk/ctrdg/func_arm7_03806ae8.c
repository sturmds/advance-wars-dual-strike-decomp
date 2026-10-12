#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/pxi.h"

void func_arm7_03806770(void);                                   /* Cartridge_ZeroStateTakeLockId */
void Cartridge_CheckLogoFromArm9Once(void);
void Cartridge_StoreInitDoneWord(u32 tag, u32 data, int err);
void Cartridge_OnShutdownMessage(u32 tag, u32 data, int err);

extern u32 data_arm7_0380afd0;

/* ARM7 copy of CTRDG_Init. */
void func_arm7_03806ae8(void)
{
    if (data_arm7_0380afd0 != 0) {
        return;
    }
    data_arm7_0380afd0 = 1;
    func_arm7_03806770();
    Fifo_SetUp_Arm7();
    Fifo_RegisterTagHandler_Arm7(FIFO_TAG_GBA_SLOT, Cartridge_StoreInitDoneWord);
    Cartridge_CheckLogoFromArm9Once();
    Fifo_RegisterTagHandler_Arm7(FIFO_TAG_GBA_SLOT, Cartridge_OnShutdownMessage);
}
