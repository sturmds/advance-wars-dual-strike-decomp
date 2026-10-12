#include "sdk/wl.h"
#include "sdk/mi.h"

// Takes the first fragment of a received frame: copies it into a new buffer
// in a free reassembly slot. When a slot already holds this frame, only the
// part of the fragment longer than what the slot has is added.
void WifiRx_BeginReassembly(WlRxHeader *header, WlRxAddrs *addrs)
{
    WlDefragSlot *slots = WIFI_DRIVER_STATE->defragSlots;
    u32 i; /* needed to match: declared before freeSlot */
    int freeSlot = -1;
    WlDefragSlot *slot;
    WlRxBuffer *buffer;
    WlRxFrame *stored;
    u32 count;
    u32 added;
    u32 length;

    /* needed to match: some fields are reached as slots[i], others through slot */
    for (i = 0; i < WIFI_REASSEMBLY_SLOTS; i++) {
        slot = &slots[i];
        if (slots[i].ticks != 0) {
            if (WifiMac_Equal(slot->addrs.dest, addrs->dest) && WifiMac_Equal(slot->addrs.src, addrs->src)
                && WIFI_SEQCTL_FIELDS(addrs->sequence)->number == WIFI_SEQCTL_FIELDS(slot->addrs.sequence)->number) {
                count = _s32_div_f(header->flags & 0xf0, 16);
                added = count - WIFI_SEQCTL_FIELDS(slot->addrs.sequence)->fragment;
                if (added == 0 || (added & 0x80000000)) {
                    return;
                }
                stored = &slots[i].buffer->frame;
                length = header->length - stored->length - WIFI_80211_HEADER_BYTES;
                if (length == 0 || (length & 0x80000000)) {
                    return;
                }
                Mem_CopyHalfwords_Arm7((u8 *)header->body + stored->length, (u8 *)stored->body + stored->length, length);
                stored->length += length;
                WIFI_SEQCTL_FIELDS(slot->addrs.sequence)->fragment = count;
                WIFI_DRIVER_STATE->counters[9] += count;
                return;
            }
        } else {
            freeSlot = i;
        }
    }
    if (freeSlot == -1) {
        return;
    }
    /* room for the frame header and WIFI_RX_BODY_CAP body bytes */
    buffer = (WlRxBuffer *)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, 0x622);
    if (buffer != NULL) {
        slot = &slots[freeSlot];
        Mem_CopyHalfwords_Arm7(addrs, &slot->addrs, 16); /* the addresses, the sequence and the padding */
        slots[freeSlot].ticks = 5; /* WifiTask_AgeReassembly runs left before the frame is dropped */
        slots[freeSlot].buffer = buffer;
        stored = &buffer->frame;
        Mem_CopyHalfwords_Arm7(header, &stored->unk_08, header->length + 12);
        Dma_SpinUntilChannelIdle_Arm7(WIFI_DRIVER_STATE->limits.unk_e0);
        count = _s32_div_f(stored->unk_08 & 0xf0, 16);
        WIFI_SEQCTL_FIELDS(slot->addrs.sequence)->fragment = count;
        WIFI_DRIVER_STATE->counters[9] += count;
        stored->length = header->length - WIFI_80211_HEADER_BYTES;
        slots[freeSlot].fragmentSize = stored->length / count;
    } else {
        WifiHost_RaiseEventBits(WIFI_EVT_FRAGMENT_DROPPED);
    }
}
