#include "sdk/os.h"

/* The timer entries start at gOsIrqSlots[4]; see IrqSlot_CallAndClear. */
extern IrqSlotEntry data_022a98ec[];
extern IrqSlotKeepAlias data_022a98f0[];
extern IrqSlotArgAlias data_022a98f4[];

/* Sets the handler and its argument for a timer interrupt, then enables it. */
void Timer_InstallIrqSlot(int timer, void (*handler)(void *), void *arg)
{
    OSIrqMask mask = 1UL << (timer + 3);

    data_022a98ec[timer].handler = handler;
    /* Needed to match: the doubled store makes mwcc load the table address
     * after the mask. It compiles to one store. */
    data_022a98f4[timer].arg = (data_022a98f4[timer].arg = arg);
    OS_EnableIrqMask(mask);
    data_022a98f0[timer].keepEnabled = TRUE;
}
