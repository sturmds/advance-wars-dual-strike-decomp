#include "sdk/wl.h"

void Wifi_ArmOneShotMs(u32 milliseconds, OSAlarmHandler handler);
void WifiMlme_ReassociationTimedOut(void *arg);
void *WifiFrame_NewReassocAsk(u16 *peer);
void WifiTx_EnqueueMgmt(void *frame);

/*
 * Reassociate request, run as WL task 4. State 0x60 sends a reassociation
 * request frame to request->peer and waits in 0x61 for the answer, at most
 * request->timeoutMs. State 0x63 ends the request and sends the reply.
 */
void WifiMlme_RunReassociation(void)
{
    WlPeerJob *job = &WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->peer;
    void *frame;

    switch (job->state) {
    case 0x60:
        frame = WifiFrame_NewReassocAsk(job->request->peer);
        if (frame == NULL) {
            job->reply->unk_04 = WIFI_ANSWER_NO_MEMORY;
            job->state = 0x63;
            WifiTask_Schedule(2, 4);
        } else {
            job->state = 0x61;
            WifiTx_EnqueueMgmt(frame);
            Wifi_ArmOneShotMs(job->request->timeoutMs, WifiMlme_ReassociationTimedOut);
        }
        break;
    case 0x63:
        WifiTx_ReportSlotResult(1);
        WifiTx_DrainQueue(1, 0);
        job->state = 0;
        WifiHost_ReturnCurrentRequest();
        break;
    }
}
