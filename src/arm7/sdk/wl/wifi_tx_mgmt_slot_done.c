#include "sdk/wl.h"

int func_arm7_027cb100(int station);
void func_arm7_027cb324(u32 station);
u16 func_arm7_027cb140(u32 station); /* the station's state */
void WifiStation_ChangeState(u16 station, u16 state);
void WifiStation_ReturnAssocId(u16 station);
void Wifi_SwitchMode(int mode); /* switches WlLink.mode */
void WifiStation_UncountQueuedFrame(WlTxFrame *frame);
void WifiTx_StartNextOfQueue(int queue);

/*
 * TX done callback of the LOC2 slot, which sends TX queue 1 (management
 * frames). Counts the result, then acts on what was sent: a successful
 * authentication or (re)association response moves the station on and tells
 * the host; a disassociation or deauthentication moves the station (or, for
 * a multicast address, every station) back, and finishes a pending
 * disassociate or deauthenticate request. Frees the buffer and the slot, and
 * when sendNext is set starts the next queued frame.
 */
void WifiTx_MgmtSlotDone(WlTxFrame *frame, int sendNext)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlMlme *mlme = WIFI_ACTIVE_JOB_OF(work);
    u32 subtype;
    u32 i;
    u32 *counters = work->counters;
    WlBufferList *queue = &work->txQueues[1];
    u32 station = frame->station;

    if (!(frame->result & WIFI_SEND_RESULT_UNSENT)) {
        counters[0]++;
        if (frame->addr1[0] & 1) {
            counters[WIFI_STAT_GROUP_SENT]++;
        } else {
            counters[4]++;
        }
        if (func_arm7_027cb100(station) && !(frame->frameControl & WIFI_FRAMECTL_MORE_BUFFERED)) {
            func_arm7_027cb324(station);
        }
    } else {
        counters[1]++;
    }
    if (WIFI_FRAMECTL_FIELDS(frame->frameControl)->protectedFrame) {
        counters[6]++;
    }
    counters[2] += frame->unk_0c & 0xff;

    subtype = frame->frameControl & 0xfc;
    switch (subtype) {
    case WIFI_FRAMECTL_AUTHENTICATION:
        if (!(frame->result & WIFI_SEND_RESULT_UNSENT)) {
            if (WIFI_MANAGEMENT_FIELDS_OF(frame)->auth.algorithm == 0 && WIFI_MANAGEMENT_FIELDS_OF(frame)->auth.sequence == 2
                && WIFI_MANAGEMENT_FIELDS_OF(frame)->auth.status == 0) {
                WifiStation_ChangeState(station, WIFI_PEER_AUTH_DONE);
                WifiHost_TellAuthenticated((const u8 *)frame->addr1, WIFI_MANAGEMENT_FIELDS_OF(frame)->auth.algorithm);
            } else if (WIFI_MANAGEMENT_FIELDS_OF(frame)->auth.algorithm == 1 && WIFI_MANAGEMENT_FIELDS_OF(frame)->auth.sequence == 4
                       && WIFI_MANAGEMENT_FIELDS_OF(frame)->auth.status == 0) {
                WifiStation_ChangeState(station, WIFI_PEER_AUTH_DONE);
                WifiHost_TellAuthenticated((const u8 *)frame->addr1, WIFI_MANAGEMENT_FIELDS_OF(frame)->auth.algorithm);
            }
        }
        break;
    case WIFI_FRAMECTL_ASSOC_ANSWER:
    case WIFI_FRAMECTL_REASSOC_ANSWER:
        if (!(frame->result & WIFI_SEND_RESULT_UNSENT)) {
            if (WIFI_MANAGEMENT_FIELDS_OF(frame)->assocResponse.status == 0) {
                WifiStation_ChangeState(station, WIFI_PEER_ASSOCIATED);
                if (subtype == WIFI_FRAMECTL_ASSOC_ANSWER) {
                    WifiHost_TellAssociated((const u8 *)frame->addr1, WIFI_MANAGEMENT_FIELDS_OF(frame)->assocResponse.aid,
                                        (const u8 *)&frame->frameControl + frame->unk_12);
                } else {
                    WifiHost_TellReassociated((const u8 *)frame->addr1, WIFI_MANAGEMENT_FIELDS_OF(frame)->assocResponse.aid,
                                        (const u8 *)&frame->frameControl + frame->unk_12);
                }
            }
        } else {
            WifiStation_ReturnAssocId(station);
        }
        break;
    case WIFI_FRAMECTL_DISASSOCIATION:
        if (link->unk_0c == 1) {
            if (station != 0) {
                if (func_arm7_027cb140(station) > WIFI_PEER_AUTH_DONE) {
                    WifiStation_ChangeState(station, WIFI_PEER_AUTH_DONE);
                }
            } else if (frame->addr1[0] & 1) {
                for (i = 1; i < WIFI_PEER_TABLE_INFO->count; i++) {
                    if (func_arm7_027cb140(i) > WIFI_PEER_AUTH_DONE) {
                        WifiStation_ChangeState(i, WIFI_PEER_AUTH_DONE);
                    }
                }
            }
        } else if (link->mode > 0x30) {
            Wifi_SwitchMode(0x30);
            WifiLink_DropAssocId();
        }
        if (mlme->state == 0x71) { /* set by WifiCmd_SendDisassoc */
            if (!(frame->result & WIFI_SEND_RESULT_UNSENT)) {
                mlme->any.reply->unk_04 = WIFI_ANSWER_OK;
            } else {
                mlme->any.reply->unk_04 = 12; /* same code as WIFI_ANSWER_PEER_SAID_NO */
            }
            mlme->state = 0;
            WifiHost_ReturnCurrentRequest();
        }
        break;
    case WIFI_FRAMECTL_DEAUTHENTICATION:
        if (link->unk_0c == 1) {
            if (station != 0) {
                if (func_arm7_027cb140(station) > WIFI_PEER_KNOWN_ONLY) {
                    WifiStation_ChangeState(station, WIFI_PEER_KNOWN_ONLY);
                }
            } else if (frame->addr1[0] & 1) {
                for (i = 1; i < WIFI_PEER_TABLE_INFO->count; i++) {
                    if (func_arm7_027cb140(i) > WIFI_PEER_KNOWN_ONLY) {
                        WifiStation_ChangeState(i, WIFI_PEER_KNOWN_ONLY);
                    }
                }
            }
        } else if (link->mode > 0x20) {
            Wifi_SwitchMode(0x20);
            WifiLink_DropAssocId();
        }
        if (mlme->state == 0x41) { /* set by WifiCmd_SendDeauth */
            if (!(frame->result & WIFI_SEND_RESULT_UNSENT)) {
                mlme->any.reply->unk_04 = WIFI_ANSWER_OK;
            } else {
                mlme->any.reply->unk_04 = 12; /* same code as WIFI_ANSWER_PEER_SAID_NO */
            }
            mlme->state = 0;
            WifiHost_ReturnCurrentRequest();
        }
        break;
    }

    WifiStation_UncountQueuedFrame(frame);
    WifiBuffer_Release(queue, WIFI_HOST_TX_HEADER_OF(frame));
    WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE)->slot[WIFI_TXLOC_MGMT].busy = 0;
    if (sendNext && queue->count) {
        WifiTx_StartNextOfQueue(1);
    }
}
