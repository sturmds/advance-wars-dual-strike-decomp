#include "types.h"
#include "sdk/os.h"

void MI_StopDma(u32 dmaNo);
u32 func_0210cef0(void);   /* OS_GetLockID */
u32 func_0210cfc0(u16 lockID);   /* takes the card lock; nonzero while another user holds it */
void func_0210eb70(void);   /* halts the CPU until an interrupt */
void func_0210ec44(u32 command);
void OS_ResetSystem(void);

/* Stops DMA and interrupts, tells the ARM7 to reset, and waits for it. */
void Reset_StopAndRequestArm7(u32 parameter)
{
    u16 lockID;

    if (*BOOT_ORIGIN_WORD == BOOT_ORIGIN_WIRELESS) {
        OS_Terminate();
    }
    lockID = func_0210cef0();
    while (func_0210cfc0(lockID) != 0) {
        func_0210eb70();
    }
    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);
    OS_SetIrqMask(IRQ_FIFO_RECV);
    OS_ResetRequestIrqMask(~0);
    *RESET_ARGUMENT_WORD = parameter;
    func_0210ec44(OS_PXI_COMMAND_RESET);
    OS_ResetSystem();
}
