#include "sdk/wl.h"

void WifiTx_AbortReplySlots(void);
void WifiTx_AbortCmdSlot(void);
void WifiTx_AbortDataSlot(void);

// Request unk_10 selects what to reset, one bit per part of the transmitter.
int WifiCmd_ResetTransmitParts(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (request->unk_10 & 1) {
        WifiTx_AbortReplySlots();
    }
    if (request->unk_10 & 2) {
        WifiTx_AbortCmdSlot();
    }
    if (request->unk_10 & 4) {
        WifiTx_AbortDataSlot();
    }
    return 0;
}
