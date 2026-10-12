#include "sdk/wl.h"
#include "sdk/wmsp.h"

void func_arm7_027c84e0(void* dest, const void* src, u32 size);

/* Nintendo multicast address 03:09:bf:00:00:00, the first address of MP poll frames. */
extern u16 data_arm7_027d6698[3];

/* Wireless registers for the CMD TX location (GBATEK names). */
#define REG_W_TXBUF_CMD (*(volatile u16*)0x04808090)
#define REG_W_CMD_TOTALTIME (*(volatile u16*)0x048080c0)
#define REG_W_CMD_REPLYTIME (*(volatile u16*)0x048080c4)
#define REG_W_CMD_COUNT (*(volatile u16*)0x04808118)

/* The CMD slot's buffer, which goes to the host when the MP exchange ends. */
#define MP_END(tx) ((WlMpEndIndication*)(tx)->cmdBuffer)

// Parent side (phase 1): builds the MP poll frame in the CMD TX slot, sets up
// the buffer that collects the children's replies (sent to the host as
// WIFI_HOSTMSG_POLL_FINISHED), and starts the CMD transfer now or at request->sendTime.
// Frames go out at 2 Mbit/s: 4 microseconds per byte.
int WifiCmd_StartMpPoll(WlMpRequest* request, WlReply* reply) {
  WlRamFrame* frame;
  WlWork* work = WIFI_DRIVER_STATE;
  WlLink* link = WIFI_LINK_VIEW_OF(work);
  WlHead* head = WIFI_SESSION_HEAD_OF(work);
  WlBufferLists* lists = WIFI_LISTS_VIEW_OF(work);
  WlTxSlots* tx = WIFI_TX_LOCATIONS_OF(work);
  u16 options = work->unk_638;
  BOOL copyData = TRUE;
  u32 bit;
  u32 childSize;
  u32 extraBytes;
  u32 extraReplyTime;
  u32 extraPollTime;
  u32 totalSize;
  u32 resendBit;
  u32 usOffset;
  u32 count;
  u32 pollTime;
  u32 replyTime;
  u32 elapsed;
  int enabled;
  u8* entry;
  u32 aid;
  u32 childCount;

  reply->status = 1;
  if (head->phase != 1) {
    return 11;
  }
  if (tx->slot[WIFI_TXLOC_MP_POLL].busy != 0) {
    return 8;
  }
  frame = tx->slot[WIFI_TXLOC_MP_POLL].frame;
  if (request->flags & 0x8000) {
    if ((request->flags & 2) == 0) {
      request->childTime = tx->mpChildTime;
    }
    if ((request->flags & 4) == 0) {
      request->pollBitmap = tx->mpPollBitmapSaved;
    }
    if ((request->flags & 8) == 0) {
      request->sendTime = tx->mpSendTime;
    }
    if ((request->flags & 0x10) == 0) {
      request->dataLength = tx->mpDataLength;
      copyData = FALSE;
    }
  }
  if (request->dataLength > 0x204) {
    return WIFI_ANSWER_OUT_OF_RANGE;
  }
  for (bit = 2, childCount = 0; bit != 0; bit = (u16)(bit << 1)) {
    if (request->pollBitmap & bit) {
      childCount++;
    }
  }
  tx->mpChildTime = request->childTime;
  /* Each child's reply slot: from a time, work out its bytes; from bytes, its time. */
  if (request->childTime & 0x8000) {
    request->childTime &= 0x7fff;
    childSize = (u32)(request->childTime - 208) >> 2;
    frame->duration = 234;
    if (childSize > 0x10000) {
      return WIFI_ANSWER_OUT_OF_RANGE;
    }
  } else {
    if (options & 2) {
      extraBytes = 1;
      extraReplyTime = 2;
      extraPollTime = 6;
    } else {
      extraBytes = 0;
      extraReplyTime = extraBytes;
      extraPollTime = extraBytes;
    }
    childSize = request->childTime + extraBytes;
    if (childSize > 0x206 + extraBytes) {
      return WIFI_ANSWER_OUT_OF_RANGE;
    }
    request->childTime = extraReplyTime + ((childSize << 2) + 208);
    frame->duration = extraPollTime + 234;
  }
  childSize = (childSize + 9) & ~1;
  totalSize = childSize * childCount;
  tx->cmdBuffer = WifiBuffer_New(&lists->unk_15c, totalSize + 26);
  if (tx->cmdBuffer == NULL) {
    return WIFI_ANSWER_NO_MEMORY;
  }
  tx->slot[WIFI_TXLOC_MP_POLL].busy = 1;
  tx->slot[WIFI_TXLOC_MP_POLL].count++;
  tx->mpSendTime = request->sendTime;
  tx->mpExpected = request->pollBitmap;
  tx->mpReceived = 0;
  tx->mpDataLength = request->dataLength;
  tx->mpUnk12 = request->unk_12;

  frame->status = 0;
  frame->unk_02 = request->pollBitmap;
  frame->unk_04 = 0;
  frame->rate = WIFI_RATE_CODE_2M;
  frame->length = request->dataLength + 34;
  frame->frameControl = 0x228; /* data + CF-Poll, from the DS */
  frame->duration += (request->childTime + 10) * childCount;
  WifiMac_CopyAddressTriple(frame->addr1, data_arm7_027d6698, link->bssid,
                            head->macAddress);
  /* A resumed poll whose frame has a sequence number sets bit 14 of
       REG_W_TXBUF_CMD; otherwise the sequence is reset to 0xffff. */
  if ((request->flags & 0x8000) && frame->sequence != 0xffff) {
    resendBit = 0x4000;
  } else {
    resendBit = 0;
    frame->sequence = 0xffff;
  }
  frame->body[0] = request->childTime;
  frame->body[1] = request->pollBitmap;
  frame->body[2] = request->header;
  if (copyData && request->dataLength != 0) {
    func_arm7_027c84e0(&frame->body[3], request->data, request->dataLength);
  }
  if (options & 4) {
    u16* footer =
        (u16*)(((u32)&frame->body[2] + (request->dataLength + 2) + 3) & ~3);
    footer[0] = WIFI_GUARD_WORD_A;
    footer[1] = WIFI_GUARD_WORD_B;
  }

  MP_END(tx)->ind.command = WIFI_HOSTMSG_POLL_FINISHED;
  MP_END(tx)->ind.length = (totalSize + 11) >> 1;
  MP_END(tx)->recv.bitmap = request->pollBitmap;
  MP_END(tx)->recv.count = childCount;
  MP_END(tx)->recv.stride = childSize;
  MP_END(tx)->recv.unk_08 = 0;
  entry = MP_END(tx)->recv.firstEntry;
  for (aid = 1, bit = 2; bit != 0; bit = (u16)(bit << 1), aid++) {
    if (request->pollBitmap & bit) {
      WMMpRecvData* data = (WMMpRecvData*)entry;
      data->length = 0xffff;
      data->unk_02 = 0;
      data->noResponse = 0;
      data->aid = aid;
      entry += childSize;
    }
  }

  REG_W_CMD_REPLYTIME = request->childTime;
  REG_W_CMD_TOTALTIME = frame->duration;
  usOffset = 0x10000 - request->usCountBase;
  if (request->sendTime == 0) {
    pollTime = ((request->dataLength + 34) << 2) + 96;
    replyTime = request->childTime * childCount + 904;
    count = (pollTime + replyTime + 50) / 10;
    enabled = Cpu_MaskIrq_Arm7();
    REG_W_CMD_COUNT = count;
    REG_W_TXBUF_CMD =
        ((((u32)frame & 0x3fff) >> 1) | W_TXBUF_ENABLE) | resendBit;
    Cpu_RestoreIrqBit_Arm7(enabled);
  } else {
    enabled = Cpu_MaskIrq_Arm7();
    elapsed = ((usOffset + IO_WIRELESS_USEC_LOW) & 0xffff) / 10;
    if (elapsed + 3 < request->sendTime) {
      REG_W_CMD_COUNT = request->sendTime - elapsed - 1;
      REG_W_TXBUF_CMD =
          ((((u32)frame & 0x3fff) >> 1) | W_TXBUF_ENABLE) | resendBit;
      Cpu_RestoreIrqBit_Arm7(enabled);
    } else {
      Cpu_RestoreIrqBit_Arm7(enabled);
      WifiBuffer_Release(&lists->unk_15c, tx->cmdBuffer);
      tx->slot[WIFI_TXLOC_MP_POLL].busy = 0;
      tx->slot[WIFI_TXLOC_MP_POLL].count--;
      return WIFI_ANSWER_OUT_OF_RANGE;
    }
  }
  return WIFI_ANSWER_OK;
}
