#include "sdk/wl.h"

void Wifi_ArmOneShotMs(u32 milliseconds, OSAlarmHandler handler);
void WifiMlme_AuthenticationTimedOut(void *arg);
WlAuthFrame *func_arm7_027cf55c(u16 *peer, int arg);
void func_arm7_027d01c4(void *frame);
void func_arm7_027d0240(int a, int b);
void func_arm7_027d0500(int a);
void func_arm7_027d0590(int a);

/*
 * Authenticate request, run as WL task 2. State 0x30 sends the first frame
 * of an authentication exchange to request->peer and waits in 0x31 for the
 * answer, at most request->timeoutMs. State 0x35 ends the request and sends
 * the reply.
 */
void WifiMlme_RunAuthentication(void)
{
    WlPeerJob *job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->peer;
    WlAuthFrame *frame;

    switch (job->state) {
    case 0x30:
        frame = func_arm7_027cf55c(job->request->peer, 0);
        if (frame == NULL) {
            job->reply->unk_04 = WIFI_ANSWER_NO_MEMORY;
            job->state = 0x35;
            WifiTask_Schedule(2, 2);
        } else {
            frame->algorithm = job->request->authAlgorithm;
            frame->sequence = 1;
            frame->status = 0;
            job->state = 0x31;
            func_arm7_027d01c4(frame);
            Wifi_ArmOneShotMs(job->request->timeoutMs, WifiMlme_AuthenticationTimedOut);
        }
        break;
    case 0x35:
        func_arm7_027d0500(1);
        func_arm7_027d0590(1);
        func_arm7_027d0240(1, 0);
        job->state = 0;
        WifiHost_ReturnCurrentRequest();
        break;
    }
}
