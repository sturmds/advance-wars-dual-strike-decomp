#include "sdk/os.h"
#include "sdk/wm.h"

void DC_InvalidateRange(void *start, u32 size);
void DC_StoreRange(void *start, u32 size);
void MI_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size);
void OS_InitMessageQueue(OSMessageQueue *queue, OSMessage *messages, int count);
BOOL OS_SendMessage(OSMessageQueue *queue, OSMessage msg, int flags);
void WmClearFifoRecvFlag(void);
void WmReceiveFifo(int tag, u32 data, BOOL err);
int WM_SetPortCallback(int port, WMCallbackFunc callback, void *arg);
void func_02110e58(void);
BOOL PXI_IsCallbackReady(int tag, int proc);
void PXI_SetFifoRecvCallback(int tag, void *callback);

extern u16 data_022ac328;
extern WMArm9Buf *data_022ac32c;
extern OSMessageQueue data_022ac330;
extern OSMessage data_022ac344[WIRELESS_CMD_BUFFERS];
extern u16 data_022ac3c0[WIRELESS_CMD_BUFFERS][WM_FIFO_BUF_SIZE / 2];

/* Sets up the WM work area, the queue of command buffers and the PXI
 * handler. Fails if WM is already initialised or the ARM7 side is not
 * running. workArea must be 32-byte aligned. */
int Wireless_SetUpWorkArea(void *workArea, u16 dmaNo, u32 size)
{
    int port;
    int i;

    if (data_022ac328) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    data_022ac328 = TRUE;

    if (workArea == NULL) {
        data_022ac328 = FALSE;
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (dmaNo > 3) {
        data_022ac328 = FALSE;
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((u32)workArea & 0x1f) {
        data_022ac328 = FALSE;
        return WM_ERRCODE_INVALID_PARAM;
    }

    func_02110e58();
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WM, PXI_PROC_ARM7)) {
        data_022ac328 = FALSE;
        return WM_ERRCODE_WM_DISABLE;
    }

    DC_InvalidateRange(workArea, size);
    MI_DmaFill32(dmaNo, workArea, 0, size);

    data_022ac32c = workArea;
    data_022ac32c->arm7Buf = (u8 *)data_022ac32c + WM_ARM9WM_BUF_SIZE;
    data_022ac32c->status = (WMStatus *)((u8 *)data_022ac32c->arm7Buf + WM_ARM7WM_BUF_SIZE);
    data_022ac32c->requestBuf = (u8 *)data_022ac32c->status + WM_STATUS_BUF_SIZE;
    data_022ac32c->replyBuf = (u8 *)data_022ac32c->requestBuf + WM_FIFO_BUF_SIZE;
    WmClearFifoRecvFlag();
    data_022ac32c->dmaNo = dmaNo;
    data_022ac32c->unk_15c = 0;

    for (port = 0; port < WM_NUM_OF_PORT; port++) {
        /* Needed to match: the SDK's prototype takes a u16 port. */
        WM_SetPortCallback((u16)port, NULL, NULL);
    }

    OS_InitMessageQueue(&data_022ac330, data_022ac344, WIRELESS_CMD_BUFFERS);
    for (i = 0; i < WIRELESS_CMD_BUFFERS; i++) {
        u16 *buf = data_022ac3c0[i];

        *buf = WM_API_REQUEST_ACCEPTED;
        DC_StoreRange(buf, 2);
        OS_SendMessage(&data_022ac330, buf, OS_MESSAGE_BLOCK);
    }

    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WM, WmReceiveFifo);
    return WM_ERRCODE_SUCCESS;
}
