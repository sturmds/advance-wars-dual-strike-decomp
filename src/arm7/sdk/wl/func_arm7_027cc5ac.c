#include "sdk/wl.h"

void Wifi_ArmOneShotMs(u32 milliseconds, OSAlarmHandler handler);
void func_arm7_027cc574(void *arg);
void Wifi_OpenSession(void);
void Wifi_CloseSession(void);

/*
 * WL task 1. State 0x20 calls Wifi_OpenSession and waits in 0x21, at most
 * request->timeoutMs, for the receive path to set result. State 0x25 sends
 * the reply; on failure it also calls Wifi_CloseSession, which undoes
 * Wifi_OpenSession in the other steps too.
 */
void func_arm7_027cc5ac(void)
{
    WlTask1Job *job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->task1;

    switch (job->state) {
    case 0x20:
        Wifi_OpenSession();
        job->result = WIFI_ANSWER_OK;
        job->unk_06 = 0;
        job->state = 0x21;
        Wifi_ArmOneShotMs(job->request->timeoutMs, func_arm7_027cc574);
        break;
    case 0x25:
        job->reply->unk_04 = job->result;
        job->reply->value = job->unk_06;
        if (job->result != WIFI_ANSWER_OK) {
            Wifi_CloseSession();
        }
        job->state = 0;
        WifiHost_ReturnCurrentRequest();
        break;
    }
}
