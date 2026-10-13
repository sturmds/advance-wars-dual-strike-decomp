#include "types.h"
#include "sdk/os.h"

void MI_StopDma(u32 dmaNo);
u32 func_0210cfc0(
    u16 lockID); /* takes the card lock; nonzero while another user holds it */
void OS_Halt(void); /* halts the CPU until an interrupt */
void OS_ResetSystem(void);

/* Stops DMA and interrupts, tells the ARM7 to reset, and waits for it. */
void Reset_StopAndRequestArm7(u32 parameter) {
  u16 lockID;

  if (*BOOT_ORIGIN_WORD == BOOT_ORIGIN_WIRELESS) {
    OS_Terminate();
  }
  lockID = OS_GetLockID();
  while (func_0210cfc0(lockID) != 0) {
    OS_Halt();
  }
  MI_StopDma(0);
  MI_StopDma(1);
  MI_StopDma(2);
  MI_StopDma(3);
  OS_SetIrqMask(IRQ_FIFO_RECV);
  OS_ResetRequestIrqMask(~0);
  *RESET_ARGUMENT_WORD = parameter;
  OsTag_SendCommandWord(OS_PXI_COMMAND_RESET);
  OS_ResetSystem();
}
