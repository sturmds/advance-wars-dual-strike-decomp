#include "types.h"
#include "sdk/os_arm7.h"

void func_arm7_037fdd34(u32 dmaNo);
void SndHw_Reset(void);
void func_arm7_037fdc78(u32 command);
void func_arm7_03807408(void);

/* The ARM7 half of a system reset: stop all four DMA channels, leave only
 * the PXI receive interrupt enabled, clear every pending interrupt, tell the
 * ARM9 the reset can go ahead and jump to the reset code. */
void func_arm7_037fdc24(void)
{
    func_arm7_037fdd34(0);
    func_arm7_037fdd34(1);
    func_arm7_037fdd34(2);
    func_arm7_037fdd34(3);
    Irq_ReplaceEnableBits_Arm7(IRQ_FIFO_RECV);
    Irq_AckPending_Arm7(~0);
    SndHw_Reset();
    func_arm7_037fdc78(OS_PXI_COMMAND_RESET);
    func_arm7_03807408();
}
