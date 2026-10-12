#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/pxi.h"

void func_arm7_03806770(void);                                   /* Cartridge_ZeroStateTakeLockId */
void func_arm7_037fe0a0(void);                                   /* PXI_Init */
void func_arm7_037fe298(u32 tag, PXIFifoCallback callback);      /* PXI_SetFifoRecvCallback */
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
    func_arm7_037fe0a0();
    func_arm7_037fe298(FIFO_TAG_GBA_SLOT, Cartridge_StoreInitDoneWord);
    Cartridge_CheckLogoFromArm9Once();
    func_arm7_037fe298(FIFO_TAG_GBA_SLOT, Cartridge_OnShutdownMessage);
}
