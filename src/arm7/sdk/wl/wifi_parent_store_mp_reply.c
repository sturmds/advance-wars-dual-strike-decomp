#include "sdk/wl.h"

u16 func_arm7_027cb140(u32 station); /* the station's state */
u16 func_arm7_027cb068(u32 station); /* the station's AID */
void func_arm7_027cb27c(u32 station); /* the original passes station without narrowing it */
void func_arm7_027cb364(u16 station, u32 set);
u32 WifiStation_IndexOfMac(const u16 *macAddress);
WlTxFrame *WifiFrame_NewDeauthentication(const u16 *dest, u32 reason);
void WifiTx_EnqueueMgmt(void *frame);
void WL_WriteByte(void *addr, u32 value);
void Mem_CopyHalfwords_Arm7(const void *src, void *dest, u32 size);

/*
 * Handles a received MP reply (frame control 0x118: data + CF-Ack to the
 * DS) while an MP poll is out. The sender must be an associated station
 * (others get a deauthentication, reason 7) whose AID the poll expects and
 * has not heard from yet. Its body, rate and RSSI go into its reply slot in
 * the poll buffer; slots follow AID order, one for each expected AID.
 */
void WifiParent_StoreMpReply(WlRxFrame *frame)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlTxSlots *slots = WIFI_TX_LOCATIONS_OF(work);
    WlMpPoll *poll = &((WlMpCommand *)slots->cmdBuffer)->poll;
    u32 station;
    u16 bit;
    u16 b;
    u8 *reply;
    WlMpReplyEntry *entry;
    u8 rssi;
    u8 rate;

    if (slots->slot[WIFI_TXLOC_MP_POLL].busy == 0) {
        return;
    }
    /* state.unk_7c is WlLink.bssid, reached with one add as in the original */
    if (!WifiMac_Equal(frame->addr1, work->state.unk_7c)) {
        return;
    }
    if (frame->length - WIFI_80211_HEADER_BYTES > poll->replySize - 8) {
        return;
    }
    station = WifiStation_IndexOfMac(frame->addr2);
    if (station == WIFI_NO_PEER || station == 0) {
        return;
    }
    if (func_arm7_027cb140(station) != WIFI_PEER_ASSOCIATED) {
        WlTxFrame *deauth = WifiFrame_NewDeauthentication(frame->addr2, 7);
        if (deauth != NULL) {
            WifiTx_EnqueueMgmt(deauth);
        }
        return;
    }
    func_arm7_027cb364(station, WIFI_FRAMECTL_FIELDS(frame->frameControl)->powerManagement);
    func_arm7_027cb27c(station);
    bit = 1 << func_arm7_027cb068(station);
    if (bit & slots->mpReceived) {
        return;
    }
    if (!(bit & slots->mpExpected)) {
        return;
    }
    slots->mpReceived |= bit;
    poll->pendingAids &= ~bit;
    reply = poll->replies;
    for (b = bit >> 1; b != 1; b >>= 1) {
        if (b & slots->mpExpected) {
            reply += poll->replySize;
        }
    }
    entry = (WlMpReplyEntry *)reply;
    entry->length = frame->length - WIFI_80211_HEADER_BYTES;
    rssi = frame->unk_12;
    WL_WriteByte(&entry->rssi, rssi & 0xff);
    rate = frame->rate;
    WL_WriteByte(&entry->rate, rate & 0xff);
    if (entry->length != 0) {
        Mem_CopyHalfwords_Arm7(frame->body, entry->data, entry->length + 1);
    }
}
