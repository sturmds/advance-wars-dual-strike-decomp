#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

extern DlPlayHostLinkArea *data_022aced4;

void MbParent_HandleChildMessage(WMMpRecvData *data, u16 aid);

/* Handles the MP data the children sent this frame: restarts every active
 * file from its first block, then passes each child's message on. Empty
 * messages (length 0 or 0xffff) are skipped. */
void MbParent_ReceiveMpData(const WMMpRecvBuf *recvBuf)
{
    u16 i;
    u16 aid;
    WMMpRecvData *data;

    for (i = 0; i < DLPLAY_OFFER_SLOTS; i++) {
        if (MbComm_GetFile(data_022aced4, i)->active) {
            MbComm_GetFile(data_022aced4, i)->nextBlock = 0;
        }
    }
    for (aid = 1; aid <= DLPLAY_CHILD_SLOTS; aid++) {
        data = WM_ReadMPData(recvBuf, aid);
        if (data != NULL && data->length != 0xffff && data->length != 0) {
            MbParent_HandleChildMessage(data, aid);
        }
    }
}
