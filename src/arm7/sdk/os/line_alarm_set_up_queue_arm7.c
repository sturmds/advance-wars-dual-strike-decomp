#include "types.h"
#include "sdk/os_arm7.h"

/* V-alarms set up; also declared in src/arm7/sdk/os/line_alarm_is_set_up_arm7.c */
u16 data_arm7_03807a98;
/* also declared in src/arm7/sdk/os/func_arm7_037fd530.c and 1 more */
s32 data_arm7_03807aa0;
/* also declared in src/arm7/sdk/os/func_arm7_037fd530.c */
s32 data_arm7_03807a9c;
/* also declared in src/arm7/sdk/os/line_alarm_on_vcount_irq_arm7.c and 4 more */
OSVAlarmQueue gOsiVAlarmQueue_Arm7;

/* Once only: empties the V-count alarm queue, turns the V-count interrupt off and resets the frame counter. */
void LineAlarm_SetUpQueue_Arm7(void) {
  if (data_arm7_03807a98 != 0) {
    return;
  }
  data_arm7_03807a98 = 1;
  gOsiVAlarmQueue_Arm7.head = NULL;
  gOsiVAlarmQueue_Arm7.tail = NULL;
  Irq_ClearEnableBits_Arm7(IRQ_BIT_LINE_MATCH);
  data_arm7_03807aa0 = 0;
  data_arm7_03807a9c = 0;
}
