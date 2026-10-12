#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/gx.h"

OSIrqMask func_arm7_037fb690(OSIrqMask mask);
void func_arm7_037fb748(OSIrqMask mask, OSIrqFunction function);
void LineAlarm_OnVcountIrq_Arm7(void);

/* Sets the V-count interrupt to fire on the alarm's line and turns it on. */
void LineAlarm_ProgramVcountIrq_Arm7(OSVAlarm *alarm)
{
    func_arm7_037fb748(IRQ_BIT_LINE_MATCH, LineAlarm_OnVcountIrq_Arm7);
    /* line bits 0-7 go to DISPSTAT bits 8-15, line bit 8 to DISPSTAT bit 7 */
    *REG_DISPSTAT = (*REG_DISPSTAT & 0x3f) | ((alarm->fire & 0xff) << 8) | ((alarm->fire & 0x100) >> 1);
    *REG_DISPSTAT |= DISPSTAT_VCOUNT_IRQ;
    func_arm7_037fb690(IRQ_BIT_LINE_MATCH);
}
