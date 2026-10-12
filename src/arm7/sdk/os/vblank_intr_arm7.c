#include "types.h"

extern u32 data_arm7_03809ff8;
void PowerLed_StepPattern(void);

/*
 * V-blank interrupt handler; the ARM7 main installs it for IRQ mask 1.
 * Runs PowerLed_StepPattern only while data_arm7_03809ff8 is set.
 */
void VBlankIntr_Arm7(void)
{
    if (data_arm7_03809ff8 != 0) {
        PowerLed_StepPattern();
    }
}
