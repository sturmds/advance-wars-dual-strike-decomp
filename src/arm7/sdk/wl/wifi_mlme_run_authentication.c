#include "sdk/wl.h"

void Wifi_ArmOneShotMs(u32 milliseconds, OSAlarmHandler handler);
void WifiMlme_AuthenticationTimedOut(void *arg);
WlAuthFrame *WifiFrame_NewAuthentication(u16 *peer, int arg);
void WifiTx_EnqueueMgmt(void *frame);

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
        frame = WifiFrame_NewAuthentication(job->request->peer, 0);
        if (frame == NULL) {
            job->reply->unk_04 = WIFI_ANSWER_NO_MEMORY;
            job->state = 0x35;
            WifiTask_Schedule(2, 2);
        } else {
            frame->algorithm = job->request->authAlgorithm;
            frame->sequence = 1;
            frame->status = 0;
            job->state = 0x31;
            WifiTx_EnqueueMgmt(frame);
            Wifi_ArmOneShotMs(job->request->timeoutMs, WifiMlme_AuthenticationTimedOut);
        }
        break;
    case 0x35:
        WifiTx_HaltSlot(1);
        WifiTx_ReportSlotResult(1);
        WifiTx_DrainQueue(1, 0);
        job->state = 0;
        WifiHost_ReturnCurrentRequest();
        break;
    }
}
