#include "sdk/wm.h"
#include "sdk/os.h"

void Mem_CopyBytes(const void* src, void* dest, u32 size);
void MIi_CpuCopy16(const void* src, void* dest, int size);
void MIi_CpuClear16(u16 data, void* dest, int size);
void WmClearFifoRecvFlag(void);

u16 data_022ac328;
WMArm9Buf* data_022ac32c;
extern WMPortRecvCallback gWmConnectPortArg;

/* Needed to match: the zero goes through a halfword on the stack. */
static inline void MI_CpuClear16(void* dest, u32 size) {
  volatile u16 zero = 0;

  MIi_CpuClear16(zero, dest, size);
}

/* Runs when the ARM7 sends a WM reply or indication through the PXI FIFO:
 * calls the callback registered for it, tells the port callbacks about
 * connects and disconnects, then hands the buffer back to the ARM7. */
void WmReceiveFifo(u32 tag, u32 data, BOOL err) {
  WMCallback* cb = (WMCallback*)data;
  WMCallbackFunc callback;

  if (err) {
    return;
  }

  DC_InvalidateRange(data_022ac32c->replyBuf, WM_FIFO_BUF_SIZE);
  if (data_022ac32c->scanOnlyFlag == 0) {
    DC_InvalidateRange(data_022ac32c->status, WM_STATUS_BUF_SIZE);
  }
  if (cb != data_022ac32c->replyBuf) {
    DC_InvalidateRange(cb, WM_FIFO_BUF_SIZE);
  }

  if (cb->apiid >= WM_NUM_OF_CALLBACK) {
    if (cb->apiid == WM_APIID_INDICATION) {
      if (cb->errcode == WM_ERRCODE_FLASH_ERROR) {
        OS_Terminate();
      }
      if (data_022ac32c->indCallback != NULL) {
        data_022ac32c->indCallback(cb);
      }
    } else if (cb->apiid == WM_APIID_PORT_RECV) {
      WMPortRecvCallback* recv = (WMPortRecvCallback*)cb;

      if (data_022ac32c->portCallback[recv->port] != NULL) {
        recv->arg = data_022ac32c->portCallbackArg[recv->port];
        DC_InvalidateRange(recv->recvBuf, data_022ac32c->status->mpRecvBufSize);
        data_022ac32c->portCallback[recv->port](recv);
      }
    } else if (cb->apiid == WM_APIID_PORT_SEND) {
      WMPortSendCallback* send = (WMPortSendCallback*)cb;

      cb->apiid = WM_APIID_SET_MP_DATA;
      if (send->callback != NULL) {
        send->callback(send);
      }
    }
  } else {
    if (cb->apiid == WM_APIID_START_MP) {
      WMStartMPCallback* mp = (WMStartMPCallback*)cb;

      if ((mp->state == WM_STATECODE_MPEND_IND ||
           mp->state == WM_STATECODE_MP_IND) &&
          mp->errcode == WM_ERRCODE_SUCCESS) {
        DC_InvalidateRange(mp->recvBuf, data_022ac32c->status->mpRecvBufSize);
      }
    }

    if (cb->apiid == WM_APIID_END && cb->errcode == WM_ERRCODE_SUCCESS) {
      callback = data_022ac32c->callbackTable[cb->apiid];
      WM_Finish();
      if (callback != NULL) {
        callback(cb);
      }
      return;
    }

    callback = data_022ac32c->callbackTable[cb->apiid];
    if (callback != NULL) {
      callback(cb);
      if (data_022ac328 == 0) {
        return;
      }
    }

    if (cb->apiid == WM_APIID_START_PARENT ||
        cb->apiid == WM_APIID_START_CONNECT) {
      /* A connect reply is read through the parent layout here:
             * macAddress at 0x0a, aid at 0x10. */
      WMStartParentCallback* conn = (WMStartParentCallback*)cb;
      u16 state;
      u16 aid;
      u16 myAid;
      u8* macAddress;
      u8* ssid;
      u16 port;

      if (cb->apiid == WM_APIID_START_PARENT) {
        state = conn->state;
        aid = conn->aid;
        myAid = 0;
        macAddress = conn->macAddress;
        ssid = conn->ssid;
      } else if (cb->apiid == WM_APIID_START_CONNECT) {
        state = conn->state;
        aid = 0;
        myAid = conn->aid;
        macAddress = conn->macAddress;
        ssid = NULL;
      }

      if (state == WM_STATECODE_CONNECTED ||
          state == WM_STATECODE_DISCONNECTED) {
        gWmConnectPortArg.apiid = WM_APIID_PORT_RECV;
        gWmConnectPortArg.errcode = WM_ERRCODE_SUCCESS;
        gWmConnectPortArg.state = state;
        gWmConnectPortArg.recvBuf = NULL;
        gWmConnectPortArg.data = NULL;
        gWmConnectPortArg.length = 0;
        gWmConnectPortArg.aid = aid;
        gWmConnectPortArg.myAid = myAid;
        gWmConnectPortArg.seqNo = 0xffff;
        Mem_CopyBytes(macAddress, gWmConnectPortArg.macAddress,
                      sizeof(gWmConnectPortArg.macAddress));
        if (ssid != NULL) {
          MIi_CpuCopy16(ssid, gWmConnectPortArg.ssid,
                        sizeof(gWmConnectPortArg.ssid));
        } else {
          MI_CpuClear16(gWmConnectPortArg.ssid, sizeof(gWmConnectPortArg.ssid));
        }
        for (port = 0; port < WM_NUM_OF_PORT; port++) {
          gWmConnectPortArg.port = port;
          if (data_022ac32c->portCallback[port] != NULL) {
            gWmConnectPortArg.arg = data_022ac32c->portCallbackArg[port];
            data_022ac32c->portCallback[port](&gWmConnectPortArg);
          }
        }
      }
    }
  }

  DC_InvalidateRange(data_022ac32c->replyBuf, WM_FIFO_BUF_SIZE);
  WmClearFifoRecvFlag();
  if (cb != data_022ac32c->replyBuf) {
    cb->apiid |= WM_API_REQUEST_ACCEPTED;
    DC_StoreRange(cb, WM_FIFO_BUF_SIZE);
  }
}
