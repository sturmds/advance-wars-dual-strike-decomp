#include "types.h"
#include "sdk/os_arm7.h"

void func_arm7_037fdc10(void);

/* Sets the tag LineAlarm_WithdrawTagged_Arm7 matches on. Stops the system if tag is 0. */
void LineAlarm_AssignTag_Arm7(OSVAlarm *alarm, u32 tag)
{
    if (tag == 0) {
        func_arm7_037fdc10();
    }
    if (alarm != NULL) {
        alarm->tag = tag;
    }
}
