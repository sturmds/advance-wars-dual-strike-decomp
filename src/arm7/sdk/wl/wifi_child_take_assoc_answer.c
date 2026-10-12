#include "types.h"
#include "sdk/wl.h"

void WifiStation_ChangeState(u16 station, u16 state);
u32 WifiStation_IndexOfMac(const u16 *macAddress);
void WifiTx_BuildPsPoll(u32 aid);
void Wifi_SwitchMode(int mode);
void func_arm7_027c8594(void);                           /* cancels the one-shot alarm */
void func_arm7_027c90ec(u16 *dest, const u16 *src);      /* copies a 6-byte MAC address */

/*
 * Association response, child side, while WifiMlme_RunAssociation waits for it
 * (state 0x51) and only from the peer it asked. On status 0 the station takes
 * the AID it was given and the link goes to WIFI_MODE_JOINED. The reply
 * gets the status and the AID, then the step ends the request (state 0x53).
 */
void WifiChild_TakeAssocAnswer(WlRxFrame *frame)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlPeerJob *job = &WIFI_ACTIVE_JOB_OF(work)->peer;
    u16 *body = frame->body; /* capability, status, AID */

    if (link->unk_0c != 2 && link->unk_0c != 3) {
        return;
    }
    if (job->state != 0x51 || !WifiMac_Equal(job->request->peer, frame->addr2)) {
        return;
    }
    func_arm7_027c8594();
    if (body[1] == 0) {
        WifiLink_TakeAssocId(body[2] & 0xfff);
        WifiTx_BuildPsPoll(link->aid);
        func_arm7_027c90ec(link->currentAp, frame->addr2);
        link->unk_88 = WifiStation_IndexOfMac(frame->addr2);
        WifiStation_ChangeState(link->unk_88, WIFI_PEER_ASSOCIATED);
    }
    if (body[1] == 0) {
        job->reply->unk_04 = WIFI_ANSWER_OK;
        job->reply->value = 0;
        Wifi_SwitchMode(WIFI_MODE_JOINED);
    } else {
        job->reply->unk_04 = WIFI_ANSWER_PEER_SAID_NO;
        job->reply->value = body[1];
    }
    job->reply->words[0] = link->aid;
    job->state = 0x53;
    WifiTask_Schedule(2, 3);
}
