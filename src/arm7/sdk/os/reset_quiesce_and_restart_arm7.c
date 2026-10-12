#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/mi.h"
#include "sdk/snd_hw.h"

void func_arm7_037fdc78(u32 command);
void Reset_MasterOffAndReboot_Arm7(void);

/* The ARM7 half of a system reset: stop all four DMA channels, leave only
 * the PXI receive interrupt enabled, clear every pending interrupt, tell the
 * ARM9 the reset can go ahead and jump to the reset code. */
void Reset_QuiesceAndRestart_Arm7(void)
{
    Dma_HaltChannel_Arm7(0);
    Dma_HaltChannel_Arm7(1);
    Dma_HaltChannel_Arm7(2);
    Dma_HaltChannel_Arm7(3);
    Irq_ReplaceEnableBits_Arm7(IRQ_FIFO_RECV);
    Irq_AckPending_Arm7(~0);
    SndHw_Reset();
    func_arm7_037fdc78(OS_PXI_COMMAND_RESET);
    Reset_MasterOffAndReboot_Arm7();
}
