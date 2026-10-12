#include "sdk/wl.h"

void func_arm7_027cb27c(u32 station);

/*
 * Handles a received MP poll (frame control 0x228: data + CF-Poll from the
 * DS). While associated (mode 0x40), a new poll from our BSSID and AP goes
 * to the host as indication 0x182. Its body starts with the reply time per
 * child (body[0]) and the AIDs asked to reply (body[1]). Returns 0 when the
 * buffer went to the host, 1 when the caller keeps it.
 */
int WifiChild_ForwardMpPoll(WlRxFrame *frame)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlMpChild *child = WIFI_CHILD_POLL_STATE_OF(work);
    WlRxBuffer *buffer;
    u16 reply;
    u32 count;
    u16 mask;

    if (link->mode != WIFI_MODE_JOINED) {
        return 1;
    }
    if (child->lastSequence == frame->sequence) {
        work->counters[16]++; /* the same poll again */
        return 1;
    }
    child->lastSequence = frame->sequence;
    if (!WifiMac_Equal(frame->addr2, link->bssid) || !WifiMac_Equal(frame->addr3, link->currentAp)) {
        return 1;
    }
    if (((u16)(1 << link->aid) & frame->body[1]) == 0) {
        child->replyFlags = 0;
    } else {
        child->replyFlags = WIFI_POLL_ASKED_US;
    }
    reply = WIFI_MAC_REGISTER(0x098);
    if (reply & 0x8000) {
        if (((WlRamFrame *)&WIFI_PACKET_RAM[reply & 0x7fff])->unk_04 != 0) {
            child->replyFlags |= WIFI_POLL_ANSWER_READY;
        }
    }
    func_arm7_027cb27c(link->unk_88);
    frame->bodyLength = frame->length - WIFI_HEADER_PLUS_CHECKSUM;
    buffer = WIFI_RX_HOLDER_OF(frame);
    buffer->command = 0x182;
    buffer->length = (frame->bodyLength + 49u) >> 1;
    mask = buffer->frame.body[1];
    count = 0; /* children asked to reply */
    while (mask != 0) {
        if (mask & 1) {
            count++;
        }
        mask >>= 1;
    }
    buffer->frame.unk_08 |= child->replyFlags | ((WIFI_MAC_REGISTER(0x098) & 0x8000) >> 4) | ((WIFI_TX_NEXT_POLL_ANSWER & 0x8000) >> 3);
    /* unk_0c plus the time the replies take, in units of 16 */
    buffer->frame.unk_0a = buffer->frame.unk_0c + ((count * (buffer->frame.body[0] + 10) + 252) >> 4);
    WifiBuffer_GiveToHost(&WIFI_DRIVER_STATE->unk_15c, &buffer->header);
    return 0;
}
