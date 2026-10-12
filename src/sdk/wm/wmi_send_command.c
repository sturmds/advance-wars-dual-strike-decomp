#include "stdarg.h"

#include "sdk/os.h"
#include "sdk/pxi.h"
#include "sdk/wm.h"

void MI_DmaCopy32(int dmaNo, const void* src, void* dst, u32 size);
BOOL OS_ReceiveMessage(OSMessageQueue* queue, OSMessage* msg, int flags);
BOOL OS_SendMessage(OSMessageQueue* queue, OSMessage msg, int flags);
BOOL OS_JamMessage(OSMessageQueue* queue, OSMessage msg, int flags);

extern WMArm9Buf* data_022ac32c;
extern OSMessageQueue data_022ac330;

/* Takes a free buffer from the queue, writes the command into it (or copies
 * requestBuf when there are no arguments) and sends it to the ARM7. */
int WMi_SendCommand(int apiid, u16 argCount, ...) {
  va_list args;
  int i;
  int result;
  u32* buf;

  if (!OS_ReceiveMessage(&data_022ac330, (OSMessage*)&buf,
                         OS_MESSAGE_NOBLOCK)) {
    return WM_ERRCODE_FIFO_ERROR;
  }

  DC_InvalidateRange(buf, 2);
  if ((*(u16*)buf & WM_API_REQUEST_ACCEPTED) == 0) {
    OS_JamMessage(&data_022ac330, buf, OS_MESSAGE_BLOCK);
    return WM_ERRCODE_FIFO_ERROR;
  }

  if (argCount == 0) {
    DC_StoreRange(data_022ac32c->requestBuf, WM_FIFO_BUF_SIZE);
    DC_InvalidateRange(buf, WM_FIFO_BUF_SIZE);
    MI_DmaCopy32(data_022ac32c->dmaNo, data_022ac32c->requestBuf, buf,
                 WM_FIFO_BUF_SIZE);
  }

  /* The command: apiid in the first halfword, then one word per argument. */
  *(u16*)buf = apiid;
  va_start(args, argCount);
  for (i = 0; i < argCount; i++) {
    /* Needed to match: buf[i + 1] stores through an index register. */
    (&buf[i])[1] = va_arg(args, u32);
  }
  va_end(args);

  DC_StoreRange(buf, WM_FIFO_BUF_SIZE);
  result = PXI_SendWordByFifo(PXI_FIFO_TAG_WM, (u32)buf, FALSE);
  OS_SendMessage(&data_022ac330, buf, OS_MESSAGE_BLOCK);
  if (result < 0) {
    return WM_ERRCODE_FIFO_ERROR;
  }
  return WM_ERRCODE_OPERATING;
}
