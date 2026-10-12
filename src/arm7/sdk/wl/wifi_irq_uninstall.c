#include "sdk/wl.h"

void WifiIrq_Uninstall(void) {
  Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);
  Irq_InstallHandlers_Arm7(IRQ_MASK_WIRELESS, NULL);
}
