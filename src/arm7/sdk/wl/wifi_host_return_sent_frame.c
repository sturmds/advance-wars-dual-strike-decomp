#include "sdk/wl.h"

// Fills the reply of a finished TX request with the frame result and hands
// the buffer back to the host.
void WifiHost_ReturnSentFrame(WlBufferList *list, WlTxRequest *request)
{
    // needed to match: counted from the buffer start; 8 halfwords reach frame
    WlReply *reply = (WlReply *)((u16 *)request + request->replyOffset + 8);

    request->command = reply->unk_00;
    reply->status = 2;
    reply->unk_04 = 0;
    reply->value = request->frame.result;
    WifiBuffer_GiveToHost(list, &request->header);
}
