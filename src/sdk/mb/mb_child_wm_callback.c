#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"
#include "sdk/wm.h"

extern WMScanParam data_022acee0;

void MI_DmaCopy16(u32 dmaNo, const void* src, void* dest, u32 size);
void MIi_CpuCopy16(const void* src, void* dst, u32 size);
void MbParent_ReportError(u16 apiid, int errcode);
int MbParent_CanSendMpData(void);
void MbParent_PortCallback(RadioNotice* ind);
BOOL DownloadPlay_AdvanceToAllowedChannel(WMScanParam* scan);
int DownloadPlay_RequestWmReset(void);
int WM_End(WMCallbackFunc callback);
int WM_SetLifeTime(WMCallbackFunc callback, u16 tableNumber, u16 camLifeTime,
                   u16 frameLifeTime, u16 mpLifeTime);
int Wireless_RequestJoinParent(WMCallbackFunc callback, WMBssDesc* bssDesc,
                               const u8* ssid, int powerSave, int authMode);
int WM_StartScan(WMCallbackFunc callback, const WMScanParam* param);

void MbChild_WmCallback(void* arg);

/* After a scan result: scan again, on the next channel when asked to;
 * Download Play ends when no channel is allowed. */
static inline void MbChild_ScanAgain(DlPlayStationArea* work) {
  if (work->unk_5e4 == 0) {
    return;
  }
  if (work->scanAllChannels &&
      !DownloadPlay_AdvanceToAllowedChannel(&data_022acee0)) {
    DownloadPlay_RequestWmReset();
  }
  MbParent_ReportError(WM_APIID_START_SCAN,
                       WM_StartScan(MbChild_WmCallback, &data_022acee0));
}

/* WM callback of the Download Play child side: after WM_Initialize it sets
 * the life times, scans for parents and keeps each one found, connects to
 * connectBssDesc, then starts MP. Each step is reported to work->callback. */
void MbChild_WmCallback(void* arg) {
  WMCallback* cb = arg;
  DlPlayStationArea* work = data_022aced8;

  switch (cb->apiid) {
    case WM_APIID_INITIALIZE:
      if (cb->errcode != WM_ERRCODE_SUCCESS) {
        work->callback(DLPLAY_EVENT_RADIO_FAULT, arg);
        return;
      }
      work->callback(DLPLAY_EVENT_STARTUP_DONE, arg);
      MbParent_ReportError(
          WM_APIID_SET_LIFETIME,
          WM_SetLifeTime(MbChild_WmCallback, 0xffff, 40, 5, 40));
      return;

    case WM_APIID_SET_LIFETIME:
      if (cb->errcode != WM_ERRCODE_SUCCESS) {
        work->callback(DLPLAY_EVENT_RADIO_FAULT, arg);
        return;
      }
      data_022acee0.scanBuf = &work->scanBuffer;
      if (data_022acee0.channel == 0) {
        data_022acee0.channel = 1;
      }
      if (data_022acee0.maxChannelTime == 0) {
        data_022acee0.maxChannelTime = 200;
      }
      /* any parent */
      data_022acee0.bssid[0] = 0xff;
      data_022acee0.bssid[1] = 0xff;
      data_022acee0.bssid[2] = 0xff;
      data_022acee0.bssid[3] = 0xff;
      data_022acee0.bssid[4] = 0xff;
      data_022acee0.bssid[5] = 0xff;
      work->unk_5e4 = 1;
      work->scanAllChannels = 1;
      MbParent_ReportError(WM_APIID_START_SCAN,
                           WM_StartScan(MbChild_WmCallback, &data_022acee0));
      return;

    case WM_APIID_START_SCAN: {
      WMStartScanCallback* scan = arg;
      DlPlayScannedHost* found;
      int i;

      if (scan->errcode != WM_ERRCODE_SUCCESS) {
        work->callback(DLPLAY_EVENT_RADIO_FAULT, arg);
        return;
      }
      switch (scan->state) {
        case WM_STATECODE_SCAN_START:
          return;
        case WM_STATECODE_PARENT_FOUND:
          found = work->found;
          for (i = 0; i < work->foundCount; i++) {
            if (scan->macAddress[0] == found[i].scan.macAddress[0] &&
                scan->macAddress[1] == found[i].scan.macAddress[1] &&
                scan->macAddress[2] == found[i].scan.macAddress[2] &&
                scan->macAddress[3] == found[i].scan.macAddress[3] &&
                scan->macAddress[4] == found[i].scan.macAddress[4] &&
                scan->macAddress[5] == found[i].scan.macAddress[5]) {
              /* known parent: refresh its game info and beacon */
              found[i].scan.gameInfoLength = scan->gameInfoLength;
              found[i].scan.gameInfo = scan->gameInfo;
              DC_InvalidateRange(&work->found[i].bssDesc, sizeof(WMBssDesc));
              MI_DmaCopy16(data_022aced0, &work->scanBuffer,
                           &work->found[i].bssDesc, sizeof(WMBssDesc));
              work->lastFoundIndex = i;
              goto notify;
            }
          }
          if (i < DLPLAY_SCAN_HIT_SLOTS) {
            work->foundCount = i + 1;
            MIi_CpuCopy16(scan, &found[i].scan, sizeof(WMStartScanCallback));
            DC_InvalidateRange(&work->found[i].bssDesc, sizeof(WMBssDesc));
            MI_DmaCopy16(data_022aced0, &work->scanBuffer,
                         &work->found[i].bssDesc, sizeof(WMBssDesc));
            work->lastFoundIndex = i;
          }
        notify:
          work->callback(DLPLAY_EVENT_SCAN_HIT, arg);
          MbChild_ScanAgain(work);
          return;
        case WM_STATECODE_PARENT_NOT_FOUND:
          work->callback(DLPLAY_EVENT_SCAN_MISS, arg);
          MbChild_ScanAgain(work);
          return;
        default:
          work->callback(DLPLAY_EVENT_RADIO_FAULT, arg);
          return;
      }
    }

    case WM_APIID_END_SCAN:
      if (cb->errcode != WM_ERRCODE_SUCCESS) {
        work->callback(DLPLAY_EVENT_RADIO_FAULT, arg);
        return;
      }
      MbParent_ReportError(
          WM_APIID_START_CONNECT,
          Wireless_RequestJoinParent(MbChild_WmCallback, work->connectBssDesc,
                                     NULL, TRUE, 0));
      return;

    case WM_APIID_START_CONNECT: {
      WMStartConnectCallback* connect = arg;

      if (connect->errcode != WM_ERRCODE_SUCCESS) {
        work->foundCount = 0;
        work->callback(DLPLAY_EVENT_JOIN_REFUSED, arg);
        return;
      }
      switch (connect->state) {
        case WM_STATECODE_CONNECT_START:
          work->unk_52a = 0;
          work->unk_528 = 1;
          return;
        case WM_STATECODE_CONNECTED:
          work->aid = connect->aid;
          work->callback(DLPLAY_EVENT_JOINED_HOST, arg);
          work->unk_52a = 1;
          if (WM_SetPortCallback(DLPLAY_WIRELESS_PORT,
                                 (WMCallbackFunc)MbParent_PortCallback,
                                 NULL) != WM_ERRCODE_SUCCESS) {
            return;
          }
          MbParent_ReportError(
              WM_APIID_START_MP,
              WM_StartMPEx(MbChild_WmCallback, work->mpRecvBuffer,
                           work->mpRecvSize, work->mpSendBuffer,
                           work->mpSendSize, work->unk_52c == 0, 0, FALSE,
                           FALSE, TRUE, TRUE));
          return;
        case WM_STATECODE_DISCONNECTED:
          work->callback(DLPLAY_EVENT_LOST_HOST, arg);
          work->unk_52a = 0;
          work->unk_528 = 0;
          return;
        default:
          work->callback(DLPLAY_EVENT_RADIO_FAULT, arg);
          return;
      }
    }

    case WM_APIID_START_MP: {
      WMStartMPCallback* mp = arg;

      switch (mp->state) {
        case WM_STATECODE_MP_START:
          work->unk_528 = 1;
          if (MbParent_CanSendMpData()) {
            work->callback(DLPLAY_EVENT_MAY_TRANSMIT, NULL);
          }
          return;
        case WM_STATECODE_MP_IND:
        case WM_STATECODE_MPACK_IND:
          return;
        default:
          work->callback(DLPLAY_EVENT_RADIO_FAULT, arg);
          return;
      }
    }

    case WM_APIID_SET_MP_DATA:
      work->sendQueued = 0;
      if (cb->errcode == WM_ERRCODE_SUCCESS) {
        work->callback(DLPLAY_EVENT_CHILD_FRAME_OUT, arg);
      } else if (cb->errcode == WM_ERRCODE_TIMEOUT) {
        work->callback(DLPLAY_EVENT_CHILD_FRAME_LATE, arg);
      } else {
        work->callback(DLPLAY_EVENT_CHILD_FRAME_FAILED, arg);
      }
      work->callback(DLPLAY_EVENT_MAY_TRANSMIT, NULL);
      return;

    case WM_APIID_RESET:
      if (cb->errcode != WM_ERRCODE_SUCCESS) {
        work->unk_526 = 0;
        work->callback(DLPLAY_EVENT_RADIO_FAULT, arg);
        return;
      }
      work->unk_52a = 0;
      data_022aced8->unk_528 =
          0; /* needed to match: read through the global again */
      MbParent_ReportError(WM_APIID_END, WM_End(MbChild_WmCallback));
      return;

    case WM_APIID_END:
      if (cb->errcode != WM_ERRCODE_SUCCESS) {
        work->unk_526 = 0;
        work->callback(DLPLAY_EVENT_RADIO_FAULT, arg);
        return;
      }
      work->started = 0;
      data_022aced4->unk_1316 = 0; /* MB no longer runs */
      work->callback(DLPLAY_EVENT_SHUTDOWN_DONE, arg);
      return;

    case WM_APIID_START_KS:
      if (MbParent_CanSendMpData()) {
        work->callback(DLPLAY_EVENT_MAY_TRANSMIT, NULL);
      }
      return;

    case WM_APIID_INDICATION:
      if (((RadioNotice*)arg)->state == WIRELESS_NOTICE_QUEUE_REFUSED) {
        OS_Terminate();
      }
      return;

    default:
      work->callback(DLPLAY_EVENT_RADIO_FAULT, arg);
      return;
  }
}
