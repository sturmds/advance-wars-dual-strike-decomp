#include "types.h"
#include "sdk/wm.h"

void DC_InvalidateRange(void *start, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, int size);

/* Port callback of a data-sharing child: stores each set the parent sends
 * into the next slot of the ring buffer, when the set holds a block for
 * this child. */
void DataShare_OnChildPortRecv(void *arg)
{
    /* needed to match: the declaration orders in this function */
    WMPortRecvCallback *cb;
    WMStatus *status;
    WMDataSharingInfo *dsInfo;
    WMArm9Buf *buf;

    cb = arg;
    buf = func_02116e10();
    dsInfo = cb->arg;
    status = buf->status;

    if (dsInfo == NULL) {
        return;
    }
    if (cb->errcode == WM_ERRCODE_SUCCESS) {
        switch (cb->state) {
        case WM_STATECODE_PORT_RECV: {
            u16 length;
            u16 aidBitmap;
            u16 aid;
            WMDataSet *dataSet = cb->data;

            aidBitmap = dataSet->aidBitmap;
            length = cb->length;

            /* unk_184 holds this station's aid */
            DC_InvalidateRange(&status->unk_184, 2);
            aid = status->unk_184;
            if (length != dsInfo->dataSetLength) {
                if (length > sizeof(WMDataSet)) {
                    length = sizeof(WMDataSet);
                }
            }
            if (length < 4) {          /* not even the set's two bitmaps */
                break;
            }
            if (!(aidBitmap & (1 << aid))) {
                break;
            }
            MIi_CpuCopy16(dataSet, &dsInfo->ds[dsInfo->writeIndex], length);
            dsInfo->seqNum[dsInfo->writeIndex] = cb->seqNo >> 1;
            dsInfo->writeIndex = (dsInfo->writeIndex + 1) & (SHARE_RING_SLOTS - 1);
            break;
        }
        case WM_STATECODE_CONNECTED:
        case WM_STATECODE_DISCONNECTED:
            break;
        }
    } else {
        dsInfo->state = SHARE_PHASE_FAILED;
    }
}
