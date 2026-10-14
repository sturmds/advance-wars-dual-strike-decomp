#include "sdk/wl.h"

void func_arm7_027c84e0(void* dest, const void* src, u32 size);

/* Nintendo multicast address 03:09:bf:00:00:10, the third address of MP reply frames. */
static u16 data_arm7_027d6688[] = {
#include "data/arm7/data_arm7_027d6688.inc.c"
};

// Child side (phase 2): builds the frame the hardware sends in reply to the
// parent's next MP poll, in the first free of two WIFI_PACKET_RAM slots.
int WifiCmd_ArmMpReply(WlMpReplyRequest* request, WlReply* reply) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlLink* link = WIFI_LINK_VIEW_OF(work);
  WlStateView* view = WIFI_FROM_STATION_TABLE_OF(work);
  WlTxSlotView* mpReply = WIFI_TX_SLOT_RECORDS_OF(work); /* needed to match */
  u16 options = work->unk_638;
  u32 index;
  WlRamFrame* frame;
  WlMpReplySlot* slot;

  index = 1;
  reply->status = 1;
  if (view->state.phase != 2) {
    return 11;
  }
  if (request->length > 0x204) {
    return 5;
  }
  if (mpReply->slots[0].busy == 0) {
    index = 0;
  }
  slot = &mpReply->slots[index];
  if (slot->busy != 0) {
    return 8;
  }
  if (WIFI_TX_NEXT_POLL_ANSWER & 0x8000) {
    return 8;
  }
  frame = mpReply->slots[index].frame;
  frame->status = 0;
  frame->unk_04 = 0;
  frame->rate = WIFI_RATE_CODE_2M;
  frame->length = request->length + 30;
  frame->frameControl = 0x118; /* data + CF-Ack, to the DS */
  WifiMac_CopyAddressTriple(frame->addr1, link->bssid, view->macAddress,
                            data_arm7_027d6688);
  frame->body[0] = request->header;
  if (request->length != 0) {
    func_arm7_027c84e0(&frame->body[1], request->data, request->length);
  }
  if (options & 4) {
    u16* footer = (u16*)(((u32)frame->body + (request->length + 2) + 3) & ~3);
    footer[0] = 0xb6b8;
    footer[1] = 0x1d46;
  }
  slot->busy = 1;
  mpReply->slots[index].count++;
  WIFI_TX_NEXT_POLL_ANSWER = 0x8000 | (((u32)frame & 0x3fff) >> 1);
  return 0;
}
