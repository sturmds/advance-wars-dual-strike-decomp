#include "sdk/os_arm7.h"

/* Puts back the IRQ handler that IrqNest_HookVector replaced, if the nesting handler is still installed. */
void IrqNest_UnhookVector(void) {
  int irqState;

  if (BIOS_IRQ_HANDLER_PTR == IrqNest_DispatchByPriority) {
    irqState = Cpu_MaskIrq_Arm7();
    BIOS_IRQ_HANDLER_PTR = gOsNestedIrqWork.savedVector;
    Cpu_RestoreIrqBit_Arm7(irqState);
  }
}
