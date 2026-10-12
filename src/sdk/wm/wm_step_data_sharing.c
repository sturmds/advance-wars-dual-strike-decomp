#include "types.h"
#include "sdk/wm.h"

void DC_InvalidateRange(void *start, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, int size);
void DataShare_OnSetSent(void *arg);
int WM_SetMPDataToPortEx(WMCallbackFunc callback, void *arg, const void *sendData, u16 sendDataSize,
                         u16 destBitmap, u16 port, u16 prio);

/* Called once a frame by every station: hands out the next complete set in
 * receiveData and passes this station's block (sendData) on. The parent
 * puts its block into the set it fills; a child sends its block to the
 * parent. Returns WM_ERRCODE_NO_DATASET while no set is ready yet. */
int WM_StepDataSharing(WMDataSharingInfo *dsInfo, u16 *sendData, WMDataSet *receiveData)
{
    /* needed to match: aid declared first, and result reused below */
    u16 aid;
    WMStatus *status = func_02116e10()->status;
    int result;
    u16 readyBitmap;                   /* read on the parent only */

    result = WMi_CheckStateEx(2, WM_STATE_MP_PARENT, WM_STATE_MP_CHILD);
    if (result != 0) {
        return result;
    }
    DC_InvalidateRange(&status->mpFlag, 4);
    if (status->mpFlag == 0) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    if (dsInfo == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (sendData == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (receiveData == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    DC_InvalidateRange(&status->aid, 2);
    aid = status->aid;
    if (aid == 0) {
        DC_InvalidateRange(&status->mpReadyBitmap, 2);
        readyBitmap = status->mpReadyBitmap;
    }
    if (dsInfo->state == SHARE_PHASE_FAILED) {
        return WM_ERRCODE_FAILED;
    }
    if (dsInfo->state != SHARE_PHASE_RUNNING && dsInfo->state != SHARE_PHASE_RESEND) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    result = WM_ERRCODE_NO_DATASET;

    if (aid == 0) {
        BOOL sendFlag = FALSE;
        BOOL syncFlag = FALSE;

        if (dsInfo->state == SHARE_PHASE_RESEND) {
            int sendResult;
            u16 oldWriteIndex;

            dsInfo->state = SHARE_PHASE_RUNNING;
            oldWriteIndex = (dsInfo->writeIndex + SHARE_RING_SLOTS - 1) & (SHARE_RING_SLOTS - 1);
            sendResult = WM_SetMPDataToPortEx(DataShare_OnSetSent, dsInfo,
                                       &dsInfo->ds[oldWriteIndex], dsInfo->dataSetLength,
                                       dsInfo->aidBitmap & readyBitmap, dsInfo->port, WIRELESS_URGENCY_RAISED);
            if (sendResult == WM_ERRCODE_NO_CHILD) {
                dsInfo->seqNum[oldWriteIndex] = 0xffff;
                dsInfo->sendIndex = (dsInfo->sendIndex + 1) & (SHARE_RING_SLOTS - 1);
            } else if (sendResult != WM_ERRCODE_SUCCESS && sendResult != WM_ERRCODE_OPERATING) {
                dsInfo->state = SHARE_PHASE_FAILED;
                return WM_ERRCODE_FAILED;
            }
        }
        if (dsInfo->readIndex != dsInfo->sendIndex) {
            /* bit 0: aid 0, the parent's own block */
            dsInfo->ds[dsInfo->readIndex].aidBitmap |= 1;
            MIi_CpuCopy16(&dsInfo->ds[dsInfo->readIndex], receiveData, sizeof(WMDataSet));
            dsInfo->currentSeqNum = dsInfo->seqNum[dsInfo->readIndex];
            dsInfo->readIndex = (dsInfo->readIndex + 1) & (SHARE_RING_SLOTS - 1);
            sendFlag = TRUE;
            result = WM_ERRCODE_SUCCESS;
            if (!dsInfo->doubleMode && readyBitmap && dsInfo->ds[dsInfo->writeIndex].aidBitmap == 1) {
                syncFlag = TRUE;
            } else {
                syncFlag = FALSE;
            }
        }
        DataShare_PushCurrentSet(dsInfo, FALSE);
        if (sendFlag) {
            DataShare_StoreStationBlock(dsInfo, 0, sendData);
            if (!dsInfo->doubleMode) {
                DataShare_PushCurrentSet(dsInfo, syncFlag);
            }
        }
    } else {
        BOOL sendFlag = FALSE;

        if (dsInfo->state == SHARE_PHASE_RESEND) {
            dsInfo->state = SHARE_PHASE_RUNNING;
            sendFlag = TRUE;
        } else if (dsInfo->readIndex != dsInfo->writeIndex) {
            /* a new set is marked and handed out one step later */
            if (!(dsInfo->ds[dsInfo->readIndex].aidBitmap & 1)) {
                dsInfo->ds[dsInfo->readIndex].aidBitmap |= 1;
            } else {
                MIi_CpuCopy16(&dsInfo->ds[dsInfo->readIndex], receiveData, sizeof(WMDataSet));
                dsInfo->currentSeqNum = dsInfo->seqNum[dsInfo->readIndex];
                dsInfo->readIndex = (dsInfo->readIndex + 1) & (SHARE_RING_SLOTS - 1);
                sendFlag = TRUE;
                result = WM_ERRCODE_SUCCESS;
            }
        }
        if (sendFlag) {
            int sendResult;
            /* the child's block goes 32 bytes into a free set (a cache line in) */
            u16 *buf = (u16 *)((u8 *)&dsInfo->ds[dsInfo->sendIndex] + 32);

            MIi_CpuCopy16(sendData, buf, dsInfo->dataLength);
            sendResult = WM_SetMPDataToPortEx(DataShare_OnSetSent, dsInfo, buf, dsInfo->dataLength,
                                       dsInfo->aidBitmap, dsInfo->port, WIRELESS_URGENCY_RAISED);
            dsInfo->sendIndex = (dsInfo->sendIndex + 1) & (SHARE_RING_SLOTS - 1);
            if (sendResult != WM_ERRCODE_OPERATING && sendResult != WM_ERRCODE_SUCCESS) {
                dsInfo->state = SHARE_PHASE_FAILED;
                result = WM_ERRCODE_FAILED;
            }
        }
    }
    return result;
}
