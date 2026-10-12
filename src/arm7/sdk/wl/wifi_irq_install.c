#include "sdk/wl.h"

void WifiIrq_Dispatch(void);

void WifiIrq_Install(void)
{
    Irq_InstallHandlers_Arm7(IRQ_MASK_WIRELESS, WifiIrq_Dispatch);
    Irq_SetEnableBits_Arm7(IRQ_MASK_WIRELESS);
}
