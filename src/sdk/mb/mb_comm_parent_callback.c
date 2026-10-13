#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

void MI_CpuFill8(void* dst, int value, u32 size);
void MIi_CpuClearFast(u32 value, void* dst, u32 size);
void MbGame_SetPlayers(DlPlayGameEntry* game, const void* users,
                       u32 playerFlags, u32 unk);
void MbBeacon_Step(u32 ggid, u16 tgid, u32 attribute);
u8 MbParent_GetNonZeroMask(void);
int MbParent_SendCommData(void);
int func_0211be50(void);
u16 func_0211be3c(void);
void MbComm_ReportError(u32 aid, u16 errorCode);
void MbParent_ReceiveMpData(const WMMpRecvBuf* recvBuf);
void MbComm_SetState(u32 aid, u32 state, void* arg);
void MbPieces_Clear(u16 aid);

/* The comm layer's callback for WM events on the parent: children joining
 * and leaving, MP data in and out, beacons and errors. At the end it frees
 * the work area and tells the application. */
void MbParent_CommCallback(u16 type, void* arg) {
  switch (type) {
    case DLPLAY_EVENT_STARTUP_DONE:
      MbComm_SetState(0, DLPLAY_CHILD_LINK_READY, arg);
      break;

    case DLPLAY_EVENT_CHILD_JOINED: {
      WMStartParentCallback* cb = arg;

      if (cb->aid == 0) {
        break;
      }
      if (cb->aid >= DLPLAY_CHILD_SLOTS + 1) {
        break;
      }
      MbComm_SetState(cb->aid, DLPLAY_CHILD_JOINED, arg);
      break;
    }

    case DLPLAY_EVENT_CHILD_LEFT: {
      WMStartParentCallback* cb = arg;

      if (cb->aid == 0) {
        break;
      }
      if (cb->aid >= DLPLAY_CHILD_SLOTS + 1) {
        break;
      }
      data_022aced4->childVersions[cb->aid - 1] = 0;
      MI_CpuFill8(&data_022aced4->childGgids[cb->aid - 1], 0, sizeof(u32));
      MI_CpuFill8(&data_022aced4->childUsers[cb->aid - 1], 0,
                  sizeof(DlPlayPlayerBadge));
      MbPieces_Clear(cb->aid);
      data_022aced4->childRequests[cb->aid - 1] = DLPLAY_APP_WANTS_NOTHING;
      {
        u16 aid = cb->aid;
        s8 fileId = data_022aced4->childFiles[aid - 1];

        if (fileId != DLPLAY_NO_FILE_CHOSEN) {
          MbComm_GetFile(data_022aced4, (u8)fileId)->childBitmap &= ~(1 << aid);
          MbComm_GetFile(data_022aced4, (u8)fileId)->changedBitmap |= 1 << aid;
          data_022aced4->childFiles[aid - 1] = DLPLAY_NO_FILE_CHOSEN;
          MbComm_GetFile(data_022aced4, (u8)fileId)->pollBitmap &= ~(1 << aid);
        }
      }
      if (data_022aced4->childEntryBitmap & (1 << cb->aid)) {
        data_022aced4->childCount--;
        data_022aced4->childEntryBitmap &= ~(1 << cb->aid);
      }
      if (data_022aced4->commState[cb->aid - 1] == DLPLAY_CHILD_TOLD_TO_BOOT) {
        MbComm_SetState(cb->aid, DLPLAY_CHILD_CAN_BOOT, NULL);
      }
      MbComm_SetState(cb->aid, DLPLAY_CHILD_GONE, arg);
      data_022aced4->commState[cb->aid - 1] = DLPLAY_CHILD_UNUSED;
      break;
    }

    case DLPLAY_EVENT_SHUTDOWN_DONE:
      /* handled after the switch */
      break;

    case DLPLAY_EVENT_HOST_FRAME_IN:
      MbParent_ReceiveMpData(arg);
      break;

    case DLPLAY_EVENT_MAY_TRANSMIT:
      MbParent_SendCommData();
      break;

    case DLPLAY_EVENT_BEACON_OUT: {
      u8 i;

      for (i = 0; i < DLPLAY_OFFER_SLOTS; i++) {
        if (data_022aced4->files[i].active &&
            data_022aced4->files[i].changedBitmap != 0) {
          MbGame_SetPlayers(&data_022aced4->files[i].gameInfo,
                            data_022aced4->childUsers,
                            data_022aced4->files[i].childBitmap,
                            data_022aced4->files[i].changedBitmap);
          data_022aced4->files[i].changedBitmap = 0;
        }
      }
      MbBeacon_Step(func_0211be50(), func_0211be3c(),
                    MbParent_GetNonZeroMask());
      break;
    }

    case DLPLAY_EVENT_CALL_REFUSED:
      switch (((DlPlayFailedCall*)arg)->errcode) {
        case WM_ERRCODE_FAILED:
        case WM_ERRCODE_WM_DISABLE:
        case WM_ERRCODE_NO_DATASET:
        case WM_ERRCODE_INVALID_PARAM:
        case WM_ERRCODE_FIFO_ERROR:
        case WM_ERRCODE_TIMEOUT:
          MbComm_ReportError(0, DLPLAY_FAULT_UNRECOVERABLE);
          break;
        case WM_ERRCODE_SUCCESS:
        case WM_ERRCODE_OPERATING:
        case WM_ERRCODE_ILLEGAL_STATE:
        case WM_ERRCODE_NO_CHILD:
        case WM_ERRCODE_SEND_QUEUE_FULL:
        case WM_ERRCODE_NO_ENTRY:
        case WM_ERRCODE_OVER_MAX_ENTRY:
        case WM_ERRCODE_INVALID_POLLBITMAP:
        case WM_ERRCODE_NO_DATA:
        case WM_ERRCODE_SEND_FAILED:
        default:
          MbComm_ReportError(0, DLPLAY_FAULT_RADIO_CALL);
          break;
      }
      break;

    case DLPLAY_EVENT_RADIO_FAULT:
      /* By WM API id: failures of these calls end Download Play. */
      switch (((WMCallback*)arg)->apiid) {
        case 0:
        case 7:
        case 8:
        case 13:
        case 14:
        case 15:
        case 17:
        case 18:
        case 21:
        case 25:
        case 29:
          MbComm_ReportError(0, DLPLAY_FAULT_UNRECOVERABLE);
          break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 9:
        case 10:
        case 11:
        case 12:
        case 16:
        case 19:
        case 20:
        case 22:
        case 23:
        case 24:
        case 26:
        case 27:
        case 28:
        default:
          MbComm_ReportError(0, DLPLAY_FAULT_RADIO_CALL);
          break;
      }
      break;
  }

  if (type == DLPLAY_EVENT_SHUTDOWN_DONE) {
    DlPlayLinkEventFunc callback = data_022aced4->callback;
    volatile u32 zero = 0;

    MIi_CpuClearFast(zero, data_022aced4, sizeof(DlPlayHostLinkArea));
    data_022aced4 = NULL;
    if (callback != NULL) {
      callback(0, DLPLAY_CHILD_FINISHED, NULL);
    }
  }
}
