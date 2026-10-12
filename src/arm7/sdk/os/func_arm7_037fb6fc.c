#include "sdk/os_arm7.h"

/* The timer entries of the ARM7 IRQ slot table, from data_arm7_0380788c[4], reached through three symbols. */
extern IrqSlotEntry data_arm7_038078bc[];
extern IrqSlotKeepAlias data_arm7_038078c0[];
extern IrqSlotArgAlias data_arm7_038078c4[];
OSIrqMask func_arm7_037fb690(OSIrqMask mask);

/* ARM7 copy of Timer_InstallIrqSlot: sets the handler and its argument for a timer interrupt, then enables it. */
void func_arm7_037fb6fc(int timer, void (*handler)(void *), void *arg)
{
    data_arm7_038078bc[timer].handler = handler;
    data_arm7_038078c4[timer].arg = arg;
    func_arm7_037fb690(1 << (timer + 3));
    data_arm7_038078c0[timer].keepEnabled = TRUE;
}
