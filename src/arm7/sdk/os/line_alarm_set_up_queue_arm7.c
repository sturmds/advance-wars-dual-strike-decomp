#include "types.h"
#include "sdk/os_arm7.h"

extern u16 data_arm7_03807a98;          /* V-alarms set up */
extern OSVAlarmQueue gOsiVAlarmQueue_Arm7;
extern s32 data_arm7_03807aa0;
extern s32 data_arm7_03807a9c;
OSIrqMask func_arm7_037fb654(OSIrqMask mask);

/* Once only: empties the V-count alarm queue, turns the V-count interrupt off and resets the frame counter. */
void LineAlarm_SetUpQueue_Arm7(void)
{
    if (data_arm7_03807a98 != 0) {
        return;
    }
    data_arm7_03807a98 = 1;
    gOsiVAlarmQueue_Arm7.head = NULL;
    gOsiVAlarmQueue_Arm7.tail = NULL;
    func_arm7_037fb654(IRQ_BIT_LINE_MATCH);
    data_arm7_03807aa0 = 0;
    data_arm7_03807a9c = 0;
}
