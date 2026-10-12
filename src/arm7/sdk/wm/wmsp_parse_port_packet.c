#include "sdk/wmsp.h"

/* Needed to match: the early returns get their own epilogues, as -O4,p builds them. */
#pragma optimize_for_size off

/* A port packet's header word. */
#define PORT_HEADER_SIZE 0x00ff            /* payload length in halfwords; 0 means 512 bytes */
#define PORT_HEADER_PORT 0x0f00
#define PORT_HEADER_SEQUENCED 0x0800       /* ports 8 to 15: a sequence number follows the payload */
#define PORT_HEADER_DEST_BITMAP 0x1000     /* a bitmap of the receiving aids follows */

/*
 * Splits the payload of an MP frame from aid into port packets and sends a
 * PORT_RECV reply for each one this station should take. wmHeader is the first
 * packet's header; the next headers sit in data after each packet. Sequenced
 * packets that are not newer than the last one seen on their port are skipped.
 */
void WMSP_ParsePortPacket(u16 aid, u16 wmHeader, u16 *data, u16 length, void *recvBuf)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    BOOL first;
    int remain;
    u16 *p;
    u16 header;
    int size;
    int hasDestBitmap;
    int sequenced;
    int total;
    u16 port;
    u16 *payload;
    u16 *lastSeq;
    u32 seqNo;
    u32 last;
    WMPortRecvCallback *callback;

    if (length == 0) {
        return;
    }
    if (length > status->unk_190) {
        return;
    }
    if (aid == status->aid) {
        return;
    }
    if (aid >= 16) {
        return;
    }
    if (length & 1) {
        return;
    }
    first = TRUE;
    remain = length;
    while (remain > 0) {
        seqNo = 0xffff;
        if (first == TRUE) {
            first = FALSE;
            p = &wmHeader;
        } else {
            p = data++;
            remain -= 2;
        }
        header = *p;
        size = (header & PORT_HEADER_SIZE) << 1;
        if (size == 0) {
            size = 512;
        }
        hasDestBitmap = (header & PORT_HEADER_DEST_BITMAP) ? 1 : 0;
        sequenced = (header & PORT_HEADER_SEQUENCED) ? 1 : 0;
        total = size + (sequenced ? 2 : 0) + (hasDestBitmap ? 2 : 0);
        remain -= total;
        if (remain < 0) {
            return;
        }
        port = (header & PORT_HEADER_PORT) >> 8;
        payload = data;
        p = (u16 *)((u8 *)data + size);
        data = (u16 *)((u8 *)data + total);
        if (sequenced == 1) {
            /* Kept shifted left by one; bit 0 set means nothing was received yet. */
            lastSeq = &status->portSeqNo[aid][port & 7];
            last = *lastSeq;
            seqNo = *p;
            p++;
            if (last & 1) {
                *lastSeq = seqNo << 1;
            } else {
                /* This path reports the shifted number in the reply. */
                seqNo <<= 1;
                if ((u16)(last - seqNo) < 256) {
                    continue;
                }
                *lastSeq = seqNo;
            }
        }
        if (hasDestBitmap == 1) {
            if (!(*p & (1 << status->aid))) {
                continue;
            }
        }
        if (size > 0) {
            callback = Wmsp_GetCallbackBuffer();
            callback->apiid = WM_APIID_PORT_RECV;
            callback->errcode = WM_ERRCODE_SUCCESS;
            callback->state = WM_STATECODE_PORT_RECV;
            callback->port = port;
            callback->recvBuf = recvBuf;
            callback->data = payload;
            callback->length = size;
            callback->aid = aid;
            callback->myAid = status->aid;
            callback->seqNo = seqNo;
            Wmsp_SendReply(callback);
        }
    }
}
