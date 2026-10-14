#include "types.h"
#include "sdk/os_arm7.h"

/* 0x10 bytes no code names, before data_arm7_03807750 */
u32 data_arm7_03807740[] = {
#include "data/arm7/data_arm7_03807740.inc.c"
};
/* declared `OSIrqFunction data_arm7_03807750[]` in irq_dispatch_lowest_pending_arm7.c: type OSIrqFunction is not one the tool can write */
void* data_arm7_03807750[] = {&VBlankIrq_CountAndCallSlot_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Timer0Irq_CallSlotHandler_Arm7,
                              &Timer1Irq_CallSlotHandler_Arm7,
                              &Timer2Irq_CallSlotHandler_Arm7,
                              &Timer3Irq_CallSlotHandler_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Dma0Irq_CallSlotHandler_Arm7,
                              &Dma1Irq_CallSlotHandler_Arm7,
                              &Dma2Irq_CallSlotHandler_Arm7,
                              &Dma3Irq_CallSlotHandler_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7,
                              &Irq_IgnoreUnused_Arm7};
