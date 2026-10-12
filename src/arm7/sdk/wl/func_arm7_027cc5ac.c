#include "sdk/wl.h"

void Wifi_ArmOneShotMs(u32 milliseconds, OSAlarmHandler handler);
void func_arm7_027cc574(void *arg);
void func_arm7_037f8e04(void);
void func_arm7_037f92c8(void);

/*
 * WL task 1. State 0x20 calls func_arm7_037f8e04 and waits in 0x21, at most
 * request->timeoutMs, for the receive path to set result. State 0x25 sends
 * the reply; on failure it also calls func_arm7_037f92c8, which undoes
 * func_arm7_037f8e04 in the other steps too.
 */
void func_arm7_027cc5ac(void)
{
    WlTask1Job *job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->task1;

    switch (job->state) {
    case 0x20:
        func_arm7_037f8e04();
        job->result = WIFI_ANSWER_OK;
        job->unk_06 = 0;
        job->state = 0x21;
        Wifi_ArmOneShotMs(job->request->timeoutMs, func_arm7_027cc574);
        break;
    case 0x25:
        job->reply->unk_04 = job->result;
        job->reply->value = job->unk_06;
        if (job->result != WIFI_ANSWER_OK) {
            func_arm7_037f92c8();
        }
        job->state = 0;
        WifiHost_ReturnCurrentRequest();
        break;
    }
}
