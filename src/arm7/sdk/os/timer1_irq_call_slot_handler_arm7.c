#include "types.h"
#include "sdk/os_arm7.h"

void Timer1Irq_CallSlotHandler_Arm7(void) { IrqSlot_CallAndClear_Arm7(5); }
