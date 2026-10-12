#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

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
        irqState = func_arm7_037fdb80();
        BIOS_IRQ_HANDLER_PTR = IrqNest_DispatchByPriority;
        func_arm7_037fdb94(irqState);
    }
}
