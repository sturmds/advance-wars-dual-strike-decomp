#include "sdk/wl.h"

/* Alarm handler of func_arm7_027cc5ac: the wait ran out. */
void func_arm7_027cc574(void *arg)
{
    WlTask1Job *job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->task1;

    job->result = WIFI_ANSWER_NO_RESPONSE;
    job->state = 0x25;
    WifiTask_Schedule(2, 1);
}
