#include "sdk/wl.h"

/* Alarm handler of func_arm7_027cc3a0: no answer came within request->timeoutMs. */
void WifiMlme_AssociationTimedOut(void *arg)
{
    WlPeerJob *job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->peer;

    job->reply->unk_04 = WIFI_ANSWER_NO_RESPONSE;
    job->state = 0x53;
    WifiTask_Schedule(2, 3);
}
