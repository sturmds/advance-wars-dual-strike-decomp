#include "types.h"
#include "sdk/os.h"
#include "sdk/wm.h"

/* Port callback of a data-sharing parent: collects the children's blocks
 * and removes a child that disconnects from the sets still being filled. */
void DataShare_OnParentPortRecv(void *arg)
{
    WMPortRecvCallback *cb = arg;
    WMDataSharingInfo *dsInfo = cb->arg;

    if (dsInfo == NULL) {
        return;
    }
    if (cb->errcode == WM_ERRCODE_SUCCESS) {
        switch (cb->state) {
        case WM_STATECODE_PORT_RECV:
            DataShare_StoreStationBlock(dsInfo, cb->aid, cb->data);
            DataShare_PushCurrentSet(dsInfo, FALSE);
            break;
        case WM_STATECODE_CONNECTED:
            DataShare_PushCurrentSet(dsInfo, FALSE);
            break;
        case WM_STATECODE_DISCONNECTED: {
            u32 aidBit = 1 << cb->aid;
            int enabled = OS_DisableInterrupts();
            u16 writeIndex = dsInfo->writeIndex;

            dsInfo->ds[writeIndex].aidBitmap &= ~aidBit;
            if (dsInfo->doubleMode == TRUE) {
                /* needed to match: the u16 cast */
                dsInfo->ds[(u16)((writeIndex + 1) & (SHARE_RING_SLOTS - 1))].aidBitmap &= ~aidBit;
            }
            OS_RestoreInterrupts(enabled);
            DataShare_PushCurrentSet(dsInfo, FALSE);
            if (dsInfo->doubleMode == TRUE) {
                DataShare_PushCurrentSet(dsInfo, FALSE);
            }
            break;
        }
        }
    } else {
        dsInfo->state = SHARE_PHASE_FAILED;
    }
}
