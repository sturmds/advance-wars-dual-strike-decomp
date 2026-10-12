#include "sdk/wl.h"
#include "sdk/mi.h"

// Adds a later fragment of a received frame to the reassembly slot of its
// first fragment. Once the last fragment is in, the whole frame goes to the
// same list as an unfragmented one.
void WifiRx_AppendFragment(WlRxHeader* header, WlRxAddrs* addrs) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlBufferPool* pool = (WlBufferPool*)&work->allocator;
  WlDefragSlot* slots = work->defragSlots;
  u32 i;
  u32 fragments; /* needed to match: unsigned, with sign bit tests below */
  int offset;
  u32 length;
  u32 total;
  s32 count;
  WlDefragSlot* slot;
  WlRxFrame* stored;

  header->length -= WIFI_80211_HEADER_BYTES;
  for (i = 0; i < WIFI_REASSEMBLY_SLOTS; i++) {
    slot = &slots[i];
    if (slot->ticks == 0) {
      continue;
    }
    if (!WifiMac_Equal(slot->addrs.dest, addrs->dest)) {
      continue;
    }
    if (!WifiMac_Equal(slot->addrs.src, addrs->src)) {
      continue;
    }
    if (WIFI_SEQCTL_FIELDS(addrs->sequence)->number !=
        WIFI_SEQCTL_FIELDS(slot->addrs.sequence)->number) {
      continue;
    }
    /* same sequence number, so this is the difference of fragment numbers */
    fragments = slots[i].addrs.sequence - addrs->sequence;
    if (fragments & 0x80000000) {
      return;
    }
    offset = fragments * slots[i].fragmentSize;
    length = header->length - offset;
    if (length == 0 || (length & 0x80000000)) {
      return;
    }
    break;
  }
  if (i == WIFI_REASSEMBLY_SLOTS) {
    return;
  }
  stored = &slots[i].buffer->frame;
  total = stored->length + length;
  if (total > WIFI_RX_BODY_CAP) {
    WifiBuffer_Release(&pool->newBuffers, &WIFI_RX_HOLDER_OF(stored)->header);
    slots[i].ticks = 0;
    return;
  }
  Mem_CopyHalfwords_Arm7((u8*)header->body + offset,
                         (u8*)stored->body + stored->length, length + 1);
  stored->length = total;
  count = _s32_div_f(header->flags & 0xf0, 16);
  slot = &slots[i];
  WIFI_SEQCTL_FIELDS(slot->addrs.sequence)->fragment =
      count - fragments + WIFI_SEQCTL_FIELDS(slot->addrs.sequence)->fragment;
  WIFI_DRIVER_STATE->counters[9] += count;
  if (header->flags & WIFI_RX_NOT_LAST_FRAGMENT) {
    return;
  }
  slot->ticks = 0;
  stored->unk_08 = (stored->unk_08 & ~0xf0) + 0x10;
  stored->length += WIFI_80211_HEADER_BYTES;
  switch (stored->unk_08 & 0xf) {
    case 8:
      WifiBuffer_Transfer(&pool->newBuffers, &pool->lists[4],
                          &WIFI_RX_HOLDER_OF(stored)->header);
      WifiTask_Schedule(2, 6);
      break;
    case 0:
      WifiBuffer_Transfer(&pool->newBuffers, &pool->lists[6],
                          &WIFI_RX_HOLDER_OF(stored)->header);
      WifiTask_Schedule(1, 7);
      break;
    default:
      WifiBuffer_Release(&pool->newBuffers, &WIFI_RX_HOLDER_OF(stored)->header);
      break;
  }
}
