#include "types.h"
#include "sdk/os.h"
#include "sdk/wm.h"

void DC_InvalidateRange(void *start, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
int WM_SetMPDataToPortEx(WMCallbackFunc callback, void *arg, const void *sendData, u16 sendDataSize,
                         u16 destBitmap, u16 port, u16 prio);
void DataShare_OnSetSent(void *arg);         /* runs when the set has been sent */

/* Sends the data set at writeIndex, once it holds data, to the connected
 * stations and moves writeIndex on, clearing the set that will be written
 * next. delayed TRUE leaves the sender's own bit out of the sent set. */
void DataShare_PushCurrentSet(WMDataSharingInfo *dsInfo, BOOL delayed)
{
    /* needed to match: this declaration order */
    u16 resetWI;
    WMStatus *status = func_02116e10()->status;
    int enabled = OS_DisableInterrupts();
    u16 oldWI;
    u16 currentChildBitmap;
    u16 newWI;
    int result;
    volatile u16 zero; /* needed to match: the 0 goes through the stack */

    if (dsInfo->ds[dsInfo->writeIndex].aidBitmap == 0) {
        DC_InvalidateRange(&status->mpReadyBitmap, 2);
        oldWI = dsInfo->writeIndex;
        currentChildBitmap = status->mpReadyBitmap;
        newWI = (oldWI + 1) & (SHARE_RING_SLOTS - 1);
        if (dsInfo->doubleMode == TRUE) {
            resetWI = (newWI + 1) & (SHARE_RING_SLOTS - 1);
        } else {
            resetWI = newWI;
        }
        zero = 0;
        MIi_CpuClear16(zero, &dsInfo->ds[resetWI], sizeof(WMDataSet));
        /* bit 0: the parent, which is always there */
        dsInfo->ds[resetWI].aidBitmap = dsInfo->aidBitmap & (currentChildBitmap | 1);
        dsInfo->writeIndex = newWI;
        dsInfo->ds[oldWI].aidBitmap = dsInfo->aidBitmap;
        if (delayed == TRUE) {
            dsInfo->ds[oldWI].aidBitmap &= ~1;
        }
        OS_RestoreInterrupts(enabled);
        result = WM_SetMPDataToPortEx(DataShare_OnSetSent, dsInfo, &dsInfo->ds[oldWI], dsInfo->dataSetLength,
                                      dsInfo->aidBitmap & currentChildBitmap, dsInfo->port, WIRELESS_URGENCY_RAISED);
        if (result == WM_ERRCODE_NO_CHILD) {
            /* nobody to send to: mark the set as skipped */
            dsInfo->seqNum[oldWI] = 0xffff;
            dsInfo->sendIndex = (dsInfo->sendIndex + 1) & (SHARE_RING_SLOTS - 1);
        } else if (result != WM_ERRCODE_SUCCESS && result != WM_ERRCODE_OPERATING) {
            dsInfo->state = SHARE_PHASE_FAILED;
        }
    } else {
        OS_RestoreInterrupts(enabled);
    }
}
