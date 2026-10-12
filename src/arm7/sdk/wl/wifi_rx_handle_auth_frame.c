#include "types.h"
#include "sdk/mi.h"
#include "sdk/wl.h"

u16 func_arm7_027cb140(u32 station);                  /* station state */
u16 func_arm7_027cb0c8(u32 station);                  /* station's challenge seed */
void func_arm7_027cb2a0(int station, int seed);       /* keeps the station's challenge seed */
void func_arm7_027c8594(void);                        /* cancels the one-shot alarm */
void WifiStation_ChangeState(u16 station, u16 state);
void Wifi_SwitchMode(int mode);
u8 WL_ReadByte(const void *addr);
WlTxFrame *WifiFrame_NewAuthentication(const u16 *dest, u16 challengeLength);
void WifiAuth_FillChallenge(int station, WlAuthFrame *frame);
BOOL WifiAuth_VerifyChallengeReply(WlAuthFrame *frame);
void WifiTx_EnqueueMgmt(WlTxFrame *frame);

/* Length of the challenge text a parent sends in shared key authentication. */
#define WIFI_SHARED_KEY_TEXT_BYTES 128

/*
 * Authentication frame. As parent (WlLink.unk_0c == 1) it answers the
 * station: open system at once, shared key with a challenge text (sequence
 * 2) and then the result of the check (sequence 4). A station that
 * authenticates again first goes back to unauthenticated. As child it
 * handles the parent's answers to WifiMlme_RunAuthentication (job state 0x31, then
 * 0x33 while the shared key challenge is out): it sends the challenge back
 * encrypted, and on the last frame fills the reply and ends the step
 * (state 0x35).
 */
void WifiRx_HandleAuthFrame(WlRxFrame *frame)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlPeerJob *job = &WIFI_ACTIVE_JOB_OF(work)->peer;
    WlMgmtBody *body = (WlMgmtBody *)frame->body;
    /* needed to match: this declaration order */
    u16 sequence; /* of the answer */
    u32 station;
    BOOL reply;
    u16 status;
    WlTxFrame *tx;

    /* Same WEP engine restart as WifiIrq_RxEventCounted, without the busy count. */
    if (work->unk_638 & 8) {
        WlMpReplyView *mp = WIFI_REPLY_SLOTS_VIEW_OF(work);
        u16 txreq = REG_W_TXREQ_READ;

        if (!(((txreq & 1) && mp->txSlots[0].busy != 0) || ((txreq & 4) && mp->txSlots[1].busy != 0)
              || ((txreq & 8) && mp->txSlots[2].busy != 0) || (REG_W_RF_PINS & W_RF_PINS_CARRIER_SENSE))) {
            REG_W_WEP_CNT = 0;
            REG_W_WEP_CNT = 0x8000;
            WIFI_DRIVER_STATE->wepWatch.busyCount = 0;
        }
    }
    reply = FALSE;
    sequence = body->auth.sequence + 1;
    station = frame->station;
    if (station == 0) {
        status = WIFI_IEEE_STATUS_AP_FULL; /* the sender has no station table entry */
        reply = TRUE;
        goto send;
    }
    if (link->unk_0c == 1) {
        if (func_arm7_027cb140(station) > WIFI_PEER_KNOWN_ONLY) {
            WifiStation_ChangeState(station, WIFI_PEER_KNOWN_ONLY);
            WifiHost_TellDeauthenticated((const u8 *)frame->addr2, 1);
        }
        /* RX header bit 10: probably a frame the WEP engine could not decrypt. */
        if ((frame->unk_08 & 0x400) && func_arm7_027cb0c8(station) != 0) {
            reply = TRUE;
            body->auth.algorithm = WIFI_AUTHALG_CHALLENGE;
            status = WIFI_IEEE_STATUS_CHALLENGE_WRONG;
            sequence = 4;
            func_arm7_027cb2a0(station, 0);
            goto send;
        }
    }
    switch (body->auth.algorithm) {
    case WIFI_AUTHALG_OPEN:
        if (link->unk_0c == 1 && WIFI_DRIVER_STATE->state.unk_06 == 1) {
            status = WIFI_IEEE_STATUS_ALG_UNSUPPORTED;
            reply = TRUE;
        } else if (link->unk_0c == 1) {
            if (body->auth.sequence == 1) {
                status = WIFI_IEEE_STATUS_OK;
                reply = TRUE;
            } else {
                status = WIFI_IEEE_STATUS_SEQ_OUT_OF_ORDER;
                sequence = 2;
                reply = TRUE;
            }
        } else if (link->unk_0c != 1 && body->auth.sequence == 2 && job->request->authAlgorithm == WIFI_AUTHALG_OPEN
                   && WifiMac_Equal(job->request->peer, frame->addr2) && job->state == 0x31) {
            func_arm7_027c8594();
            if (body->auth.status == 0) {
                Wifi_SwitchMode(WIFI_MODE_AUTH_DONE);
                job->reply->unk_04 = WIFI_ANSWER_OK;
                job->reply->value = 0;
            } else {
                job->reply->unk_04 = WIFI_ANSWER_PEER_SAID_NO;
                job->reply->value = body->auth.status;
            }
            job->state = 0x35;
            WifiTask_Schedule(2, 2);
        }
        break;
    case WIFI_AUTHALG_CHALLENGE:
        if (link->unk_0c == 1) {
            WifiStation_ChangeState(station, WIFI_PEER_KNOWN_ONLY);
            if (body->auth.sequence == 1) {
                tx = WifiFrame_NewAuthentication(frame->addr2, WIFI_SHARED_KEY_TEXT_BYTES);
                if (tx != NULL) {
                    WIFI_MANAGEMENT_FIELDS_OF(tx)->auth.algorithm = body->auth.algorithm;
                    WIFI_MANAGEMENT_FIELDS_OF(tx)->auth.sequence = sequence;
                    WIFI_MANAGEMENT_FIELDS_OF(tx)->auth.status = WIFI_IEEE_STATUS_OK;
                    WifiAuth_FillChallenge(station, (WlAuthFrame *)tx);
                    WifiTx_EnqueueMgmt(tx);
                }
            } else if (body->auth.sequence == 3) {
                if (func_arm7_027cb140(station) != WIFI_PEER_KNOWN_ONLY || func_arm7_027cb0c8(station) == 0) {
                    status = WIFI_IEEE_STATUS_FAILURE;
                    reply = TRUE;
                } else if (!WifiAuth_VerifyChallengeReply((WlAuthFrame *)frame)) {
                    status = WIFI_IEEE_STATUS_CHALLENGE_WRONG;
                    reply = TRUE;
                    func_arm7_027cb2a0(station, 0);
                } else {
                    func_arm7_027cb2a0(station, 0);
                    status = WIFI_IEEE_STATUS_OK;
                    reply = TRUE;
                }
            } else {
                func_arm7_027cb2a0(station, 0);
                status = WIFI_IEEE_STATUS_SEQ_OUT_OF_ORDER;
                sequence = 2;
                reply = TRUE;
            }
        } else if (job->request->authAlgorithm == WIFI_AUTHALG_CHALLENGE && WifiMac_Equal(job->request->peer, frame->addr2)) {
            if (body->auth.sequence == 2) {
                if (job->state == 0x31) {
                    if (body->auth.status != 0) {
                        func_arm7_027c8594();
                        job->state = 0x35;
                        job->reply->unk_04 = WIFI_ANSWER_PEER_SAID_NO;
                        job->reply->value = body->auth.status;
                        WifiTask_Schedule(2, 2);
                        Wifi_SwitchMode(WIFI_MODE_READY);
                    } else {
                        job->state = 0x33;
                        tx = WifiFrame_NewAuthentication(frame->addr2, WL_ReadByte(&((WlAuthFrame *)frame)->challengeLength));
                        if (tx != NULL) {
                            tx->frameControl |= WIFI_FRAMECTL_WEP_ON;
                            Mem_CopyHalfwords_Arm7(frame->body,WIFI_MANAGEMENT_FIELDS_OF(tx), frame->bodyLength + 1);
                            WIFI_MANAGEMENT_FIELDS_OF(tx)->auth.algorithm = body->auth.algorithm;
                            WIFI_MANAGEMENT_FIELDS_OF(tx)->auth.sequence = 3;
                            WIFI_MANAGEMENT_FIELDS_OF(tx)->auth.status = WIFI_IEEE_STATUS_OK;
                            WifiTx_EnqueueMgmt(tx);
                        }
                    }
                }
            } else if (body->auth.sequence == 4 && job->state == 0x33) {
                func_arm7_027c8594();
                if (body->auth.status == 0) {
                    Wifi_SwitchMode(WIFI_MODE_AUTH_DONE);
                    job->reply->unk_04 = WIFI_ANSWER_OK;
                    job->reply->value = 0;
                } else {
                    job->reply->unk_04 = WIFI_ANSWER_PEER_SAID_NO;
                    job->reply->value = body->auth.status;
                }
                job->state = 0x35;
                WifiTask_Schedule(2, 2);
            }
        }
        break;
    default:
        if (link->unk_0c == 1) {
            status = WIFI_IEEE_STATUS_ALG_UNSUPPORTED;
            reply = TRUE;
        }
        break;
    }
send:
    if (reply) {
        tx = WifiFrame_NewAuthentication(frame->addr2, 0);
        if (tx != NULL) {
            WIFI_MANAGEMENT_FIELDS_OF(tx)->auth.algorithm = body->auth.algorithm;
            WIFI_MANAGEMENT_FIELDS_OF(tx)->auth.sequence = sequence;
            WIFI_MANAGEMENT_FIELDS_OF(tx)->auth.status = status;
            WifiTx_EnqueueMgmt(tx);
        }
    }
}
