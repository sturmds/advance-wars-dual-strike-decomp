#include "types.h"
#include "sdk/os_arm7.h"

void LineAlarm_Clear_Arm7(OSVAlarm *alarm)
{
    alarm->handler = NULL;
    alarm->tag = 0;
    alarm->finish = FALSE;
}
