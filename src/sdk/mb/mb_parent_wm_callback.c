#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"
#include "sdk/wm.h"

extern DlPlayHostLinkArea *data_022aced4;
extern DlPlayStationArea *data_022aced8;

void MbParent_ReportError(u16 apiid, int errcode);
void MbParent_SetIndAndLifeTime(void);
int MbParent_CanSendMpData(void);
int WM_SetParentParameter(WMCallbackFunc callback, WMParentParam *param);
int WM_SetBeaconIndication(WMCallbackFunc callback, int flag);
int WM_StartParent(WMCallbackFunc callback);
int WM_End(WMCallbackFunc callback);

/* The parent's WM callback. It steps WM through its start-up (initialize, life
 * times, parent parameters, beacon indications, start parent), starts MP when
 * the first child connects, and passes WM events to errorCallback. */
void MbParent_WmCallback(void *arg)
{
    WMCallback *cb = arg;

    switch (cb->apiid) {
    case WM_APIID_INITIALIZE:
        if (cb->errcode != WM_ERRCODE_SUCCESS) {
            data_022aced8->errorCallback(DLPLAY_EVENT_CALL_REFUSED, arg);
            return;
        }
        MbParent_SetIndAndLifeTime();
        break;
    case WM_APIID_SET_LIFETIME:
        if (cb->errcode != WM_ERRCODE_SUCCESS) {
            data_022aced8->errorCallback(DLPLAY_EVENT_CALL_REFUSED, arg);
            return;
        }
        MbParent_ReportError(WM_APIID_SET_P_PARAM,
                             WM_SetParentParameter(MbParent_WmCallback, (WMParentParam *)data_022aced8));
        break;
    case WM_APIID_SET_P_PARAM:
        data_022aced8->errorCallback(DLPLAY_EVENT_HOST_SETTINGS_DONE, arg);
        MbParent_ReportError(WM_APIID_SET_BEACON_IND, WM_SetBeaconIndication(MbParent_WmCallback, 1));
        break;
    case WM_APIID_SET_BEACON_IND:
        if (cb->errcode != WM_ERRCODE_SUCCESS) {
            data_022aced8->errorCallback(DLPLAY_EVENT_CALL_REFUSED, arg);
            return;
        }
        MbParent_ReportError(WM_APIID_START_PARENT, WM_StartParent(MbParent_WmCallback));
        break;
    case WM_APIID_START_PARENT: {
        WMStartParentCallback *parent = arg;

        if (cb->errcode != WM_ERRCODE_SUCCESS) {
            data_022aced8->errorCallback(DLPLAY_EVENT_CALL_REFUSED, arg);
            return;
        }
        switch (parent->state) {
        case WM_STATECODE_PARENT_START:
            data_022aced8->unk_52a = 0;
            data_022aced8->unk_528 = 0;
            break;
        case WM_STATECODE_CONNECTED:
            if (data_022aced8->unk_526 == 1) {
                return;
            }
            data_022aced8->unk_52a |= 1 << parent->aid;
            data_022aced8->errorCallback(DLPLAY_EVENT_CHILD_JOINED, arg);
            if (data_022aced8->unk_528 == 0 && data_022aced4->mpStarting == 0) {
                data_022aced4->mpStarting = 1;
                MbParent_ReportError(WM_APIID_START_MP,
                                     WM_StartMPEx(MbParent_WmCallback, data_022aced8->mpRecvBuffer,
                                                  data_022aced8->mpRecvSize, data_022aced8->mpSendBuffer,
                                                  data_022aced8->mpSendSize,
                                                  data_022aced8->unk_52c ? 0 : 1, 0, FALSE, FALSE, TRUE,
                                                  TRUE));
            } else if (MbParent_CanSendMpData()) {
                data_022aced8->errorCallback(DLPLAY_EVENT_MAY_TRANSMIT, NULL);
            }
            break;
        case WM_STATECODE_DISCONNECTED:
            data_022aced8->unk_52a &= ~(1 << parent->aid);
            data_022aced8->errorCallback(DLPLAY_EVENT_CHILD_LEFT, arg);
            break;
        case WM_STATECODE_BEACON_SENT:
            if (data_022aced8->unk_526 == 1) {
                return;
            }
            data_022aced8->errorCallback(DLPLAY_EVENT_BEACON_OUT, arg);
            break;
        default:
            data_022aced8->errorCallback(DLPLAY_EVENT_CALL_REFUSED, arg);
            break;
        }
        break;
    }
    case WM_APIID_START_MP: {
        WMStartMPCallback *mp = arg;

        data_022aced4->mpStarting = 0;
        switch (mp->state) {
        case WM_STATECODE_MP_START:
            data_022aced8->unk_528 = 1;
            data_022aced8->errorCallback(DLPLAY_EVENT_MAY_TRANSMIT, NULL);
            break;
        case WM_STATECODE_MPEND_IND:
            data_022aced8->errorCallback(DLPLAY_EVENT_HOST_FRAME_IN, mp->recvBuf);
            break;
        default:
            data_022aced8->errorCallback(DLPLAY_EVENT_CALL_REFUSED, arg);
            break;
        }
        break;
    }
    case WM_APIID_SET_MP_DATA:
        if (data_022aced4->unk_74c8 != 0) {
            u32 busy = 0;
            u32 i;

            for (i = 0; i < DLPLAY_CHILD_SLOTS; i++) {
                if (data_022aced4->commState[i] != 0) {
                    busy++;
                    if (busy >= 2) {
                        break;
                    }
                }
            }
            if (busy == 1) {
                OS_SpinWait(13000);
            }
        }
        data_022aced8->sendQueued = 0;
        if (cb->errcode == WM_ERRCODE_SUCCESS) {
            data_022aced8->errorCallback(DLPLAY_EVENT_HOST_FRAME_OUT, arg);
            data_022aced8->errorCallback(DLPLAY_EVENT_MAY_TRANSMIT, NULL);
        } else if (cb->errcode == WM_ERRCODE_SEND_QUEUE_FULL) {
            data_022aced8->errorCallback(DLPLAY_EVENT_OUTBOX_FULL, arg);
        } else {
            data_022aced8->errorCallback(DLPLAY_EVENT_TRANSMIT_FAILED, arg);
            data_022aced8->errorCallback(DLPLAY_EVENT_MAY_TRANSMIT, NULL);
        }
        break;
    case WM_APIID_RESET:
        if (data_022aced4->unk_1320 == 0) {
            if (cb->errcode != WM_ERRCODE_SUCCESS) {
                data_022aced8->unk_526 = 0;
                data_022aced8->errorCallback(DLPLAY_EVENT_CALL_REFUSED, arg);
                return;
            }
            data_022aced8->unk_52a = 0;
            data_022aced8->unk_528 = 0;
            MbParent_ReportError(WM_APIID_END, WM_End(MbParent_WmCallback));
            break;
        }
        WM_SetPortCallback(1, NULL, NULL);
        WM_SetIndCallback(NULL);
        /* fall through */
    case WM_APIID_END:
        if (cb->errcode != WM_ERRCODE_SUCCESS) {
            data_022aced8->unk_526 = 0;
            data_022aced8->errorCallback(DLPLAY_EVENT_CALL_REFUSED, arg);
            return;
        }
        data_022aced8->unk_50d = 0;
        data_022aced4->unk_1316 = 0;
        data_022aced8->errorCallback(DLPLAY_EVENT_SHUTDOWN_DONE, arg);
        break;
    case WM_APIID_DISCONNECT:
        if (cb->errcode != WM_ERRCODE_SUCCESS) {
            return;
        }
        data_022aced8->unk_52a &= ~((RadioDropReply *)arg)->disconnectedBitmap;
        break;
    case WM_APIID_INDICATION:
        switch (((RadioNotice *)arg)->state) {
        case WIRELESS_NOTICE_HEARD_BEACON:
            data_022aced8->errorCallback(DLPLAY_EVENT_HEARD_BEACON, arg);
            break;
        case WIRELESS_NOTICE_STATION_DROPPED:
            data_022aced8->errorCallback(DLPLAY_EVENT_STATION_DROPPED, arg);
            break;
        case WIRELESS_NOTICE_STATION_BACK:
            data_022aced8->errorCallback(DLPLAY_EVENT_STATION_REJOINED, arg);
            break;
        case WIRELESS_NOTICE_AUTHED:
            data_022aced8->errorCallback(DLPLAY_EVENT_STATION_AUTHED, arg);
            break;
        case WIRELESS_NOTICE_QUEUE_REFUSED:
            OS_Terminate();
            break;
        }
        break;
    default:
        data_022aced8->errorCallback(DLPLAY_EVENT_CALL_REFUSED, arg);
        break;
    }
}
