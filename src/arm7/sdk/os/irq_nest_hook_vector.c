#include "sdk/os_arm7.h"

/* Installs the nesting IRQ handler in the IRQ vector, keeping the old one for IrqNest_UnhookVector. */
void IrqNest_HookVector(void)
{
    IrqEntryPoint old = BIOS_IRQ_HANDLER_PTR;
    int irqState;

    if (old != IrqNest_DispatchByPriority) {
        gOsNestedIrqWork.unk_00 = 0;
        gOsNestedIrqWork.stack = IRQ_NEST_STACK_BASE;
        gOsNestedIrqWork.stackSize = IRQ_NEST_STACK_BYTES;
        gOsNestedIrqWork.savedVector = old;
        irqState = Cpu_MaskIrq_Arm7();
        BIOS_IRQ_HANDLER_PTR = IrqNest_DispatchByPriority;
        Cpu_RestoreIrqBit_Arm7(irqState);
    }
}
