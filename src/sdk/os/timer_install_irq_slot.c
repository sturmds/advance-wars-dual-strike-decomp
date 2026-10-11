#include "sdk/os.h"

/* The timer entries start at gOsIrqSlots[4]; see IrqSlot_CallAndClear. */
extern IrqSlotEntry data_022a98ec[];
extern IrqSlotKeepAlias data_022a98f0[];
extern IrqSlotArgAlias data_022a98f4[];

void func_0210cda8(int timer, void (*handler)(void *), void *arg)
{
    data_022a98ec[timer].handler = handler;
    data_022a98f4[timer].arg = arg;
    OS_EnableIrqMask(1 << (timer + 3));
    data_022a98f0[timer].keepEnabled = TRUE;
}
