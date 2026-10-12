#include "types.h"
#include "sdk/os_arm7.h"

void Dma2Irq_CallSlotHandler_Arm7(void) { IrqSlot_CallAndClear_Arm7(2); }
