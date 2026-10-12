#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/* Puts back the IRQ handler that IrqNest_HookVector replaced, if the nesting handler is still installed. */
void IrqNest_UnhookVector(void)
{
    int irqState;

    if (BIOS_IRQ_HANDLER_PTR == IrqNest_DispatchByPriority) {
        irqState = func_arm7_037fdb80();
        BIOS_IRQ_HANDLER_PTR = gOsNestedIrqWork.savedVector;
        func_arm7_037fdb94(irqState);
    }
}
