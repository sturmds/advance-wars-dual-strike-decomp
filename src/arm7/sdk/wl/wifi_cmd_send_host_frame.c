#include "sdk/wl.h"

void WifiStation_CountQueuedFrame(WlTxFrame *frame);
void func_arm7_027d10f4(int queue); /* sends what waits in WlWork.txQueues[queue] */

// Queues the frame of a host TX request on queue 0 for station 0 and starts
// sending; WifiHost_ReturnSentFrame answers the host once it is done.
int WifiCmd_SendHostFrame(WlTxRequest *request, WlReply *reply)
{
    WlTxFrame *frame;

    reply->status = 1;
    request->command = 0xffff;
    frame = &request->frame;
    frame->station = 0;
    frame->unk_12 = frame->unk_06;
    WifiStation_CountQueuedFrame(frame);
    WifiBuffer_Transfer(&WIFI_DRIVER_STATE->inbox, &WIFI_DRIVER_STATE->txQueues[0], &request->header);
    func_arm7_027d10f4(0);
    return 0;
}
