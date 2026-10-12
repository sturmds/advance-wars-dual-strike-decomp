#include "types.h"
#include "sdk/wm.h"
#include "sdk/os.h"

/* The data the child with this aid sent in the last MP exchange, or NULL when
 * the aid is not connected. */
WMMpRecvData* WM_ReadMPData(const WMMpRecvBuf* recvBuf, u16 aid) {
  WMArm9Buf* buf = func_02116e10();
  u32 entries[16]; /* addresses; a pointer array changes the registers */
  int i;

  if (func_02116df4() != 0) {
    return NULL;
  }
  if (aid < 1 || aid > 15) {
    return NULL;
  }
  DC_InvalidateRange(&buf->status->childBitmap, 2);
  if ((buf->status->childBitmap & (1 << aid)) == 0) {
    return NULL;
  }
  if (recvBuf->count == 0) {
    return NULL;
  }
  entries[0] = (u32)recvBuf->firstEntry;
  i = 0;
  do {
    if (((WMMpRecvData*)entries[i])->aid == aid) {
      return (WMMpRecvData*)entries[i];
    }
    i++;
    entries[i] = recvBuf->stride + entries[i - 1];
  } while (i < recvBuf->count);
  return NULL;
}
