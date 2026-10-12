#include "sdk/wl.h"

void Wifi_ArmOneShotMs(u32 milliseconds, OSAlarmHandler handler);
void WifiMlme_AssociationTimedOut(void *arg);
void *WifiFrame_NewAssocAsk(u16 *peer);
void func_arm7_027d01c4(void *frame);
void func_arm7_027d0240(int a, int b);
void func_arm7_027d0500(int a);
void func_arm7_027d0590(int a);

/*
 * Associate request, run as WL task 3. State 0x50 sends an association
 * request frame to request->peer and waits in 0x51 for the answer, at most
 * request->timeoutMs. State 0x53 ends the request and sends the reply.
 */
void WifiMlme_RunAssociation(void)
{
    WlPeerJob *job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->peer;
    void *frame;

    switch (job->state) {
    case 0x50:
        frame = WifiFrame_NewAssocAsk(job->request->peer);
        if (frame == NULL) {
            job->reply->unk_04 = WIFI_ANSWER_NO_MEMORY;
            job->state = 0x53;
            WifiTask_Schedule(2, 3);
        } else {
            job->state = 0x51;
            func_arm7_027d01c4(frame);
            Wifi_ArmOneShotMs(job->request->timeoutMs, WifiMlme_AssociationTimedOut);
        }
        break;
    case 0x53:
        func_arm7_027d0500(1);
        func_arm7_027d0590(1);
        func_arm7_027d0240(1, 0);
        job->state = 0;
        WifiHost_ReturnCurrentRequest();
        break;
    }
}
