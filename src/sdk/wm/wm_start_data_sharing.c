#include "types.h"
#include "sdk/math.h"
#include "sdk/wm.h"

void DC_InvalidateRange(void *start, u32 size);
void MIi_CpuClearFast(u32 value, void *dst, u32 size);
void DataShare_OnParentPortRecv(void *arg);
void DataShare_OnChildPortRecv(void *arg);
void DataShare_OnSetSent(void *arg);
int WM_SetMPDataToPortEx(WMCallbackFunc callback, void *arg, const void *sendData, u16 sendDataSize,
                         u16 destBitmap, u16 port, u16 prio);

/* Starts data sharing on port among the stations in aidBitmap, each giving
 * dataLength bytes a frame. The parent sends the first empty set (two with
 * doubleMode) to get the exchange going. */
int WM_StartDataSharing(WMDataSharingInfo *dsInfo, u16 port, u16 aidBitmap, u16 dataLength, BOOL doubleMode)
{
    /* needed to match: aid declared first */
    u16 aid;
    u16 connectedBitmap = 1;           /* on a child: only the parent, aid 0 */
    WMStatus *status = func_02116e10()->status;
    int result;
    int stationNumber;
    volatile u32 zero;

    result = WMi_CheckStateEx(2, WM_STATE_MP_PARENT, WM_STATE_MP_CHILD);
    if (result != 0) {
        return result;
    }
    DC_InvalidateRange(&status->unk_0c, 4);
    if (status->unk_0c == 0) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    if (dsInfo == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (port >= WM_NUM_OF_PORT) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (aidBitmap == 0) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    DC_InvalidateRange(&status->unk_184, 2);
    aid = status->unk_184;
    if (aid == 0) {
        /* unk_86: the children that take part in MP */
        DC_InvalidateRange(&status->unk_86, 2);
        connectedBitmap = status->unk_86;
    }

    zero = 0;
    MIi_CpuClearFast(zero, dsInfo, sizeof(WMDataSharingInfo));
    dsInfo->writeIndex = 0;
    dsInfo->sendIndex = 0;
    dsInfo->readIndex = 0;
    dsInfo->dataLength = dataLength;
    dsInfo->port = port;
    dsInfo->aidBitmap = 0;
    dsInfo->doubleMode = doubleMode ? TRUE : FALSE;
    /* this station takes part too; needed to match: the expression written
     * twice, a shared local costs the match */
    dsInfo->aidBitmap = aidBitmap | (1 << aid);
    stationNumber = MATH_CountPopulation((u16)(aidBitmap | (1 << aid)));
    dsInfo->stationNumber = stationNumber;
    dsInfo->dataSetLength = dataLength * stationNumber;
    if (dsInfo->dataSetLength > SHARE_SET_PAYLOAD_BYTES) {
        dsInfo->aidBitmap = 0;
        return WM_ERRCODE_INVALID_PARAM;
    }
    dsInfo->dataSetLength += 4;        /* the set's two bitmaps */
    dsInfo->state = SHARE_PHASE_RUNNING;

    if (aid == 0) {
        int i;

        for (i = 0; i < SHARE_RING_SLOTS; i++) {
            dsInfo->ds[i].aidBitmap = dsInfo->aidBitmap & (connectedBitmap | 1);
        }
        WM_SetPortCallback(port, DataShare_OnParentPortRecv, dsInfo);
        for (i = 0; i < (dsInfo->doubleMode == TRUE ? 2 : 1); i++) {
            int sendResult;

            dsInfo->writeIndex = (dsInfo->writeIndex + 1) & (SHARE_RING_SLOTS - 1);
            sendResult = WM_SetMPDataToPortEx(DataShare_OnSetSent, dsInfo, &dsInfo->ds[i],
                                       dsInfo->dataSetLength, dsInfo->aidBitmap & connectedBitmap,
                                       dsInfo->port, WIRELESS_URGENCY_RAISED);
            if (sendResult == WM_ERRCODE_NO_CHILD) {
                dsInfo->seqNum[i] = 0xffff;
                dsInfo->sendIndex = (dsInfo->sendIndex + 1) & (SHARE_RING_SLOTS - 1);
            } else if (sendResult != WM_ERRCODE_SUCCESS && sendResult != WM_ERRCODE_OPERATING) {
                dsInfo->state = SHARE_PHASE_FAILED;
                return WM_ERRCODE_FAILED;
            }
        }
    } else {
        /* a child stages its first block in the last set */
        dsInfo->sendIndex = SHARE_RING_SLOTS - 1;
        WM_SetPortCallback(port, DataShare_OnChildPortRecv, dsInfo);
    }
    return WM_ERRCODE_SUCCESS;
}
