#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wl.h"

u32 WifiStation_LookUpOrCreate(const u16* macAddress);
void WifiRx_HandleDeauthFrame(WlRxFrame* frame);
void WifiParent_ServePsPoll(WlRxFrame* frame);
void WifiRx_HandleDisassocFrame(WlRxFrame* frame);
void func_arm7_027cb364(u16 station, u32 set);
void func_arm7_027d2090(WlRxFrame* frame);
void WifiRx_HandleAuthFrame(WlRxFrame* frame);
void WifiScan_RecordHeardNetwork(WlRxFrame* frame, WlElements* elements);
void WifiParent_AnswerProbe(WlRxFrame* frame);
void WifiChild_TakeReassocAnswer(WlRxFrame* frame);
void WifiParent_AnswerReassocAsk(WlRxFrame* frame);
void WifiChild_TakeAssocAnswer(WlRxFrame* frame);
void WifiParent_AnswerAssocAsk(WlRxFrame* frame);
void WifiRx_HandleBeaconFrame(WlRxFrame* frame);

/*
 * Task 7: takes the next received management or control frame from its list
 * and hands it to the handler for its subtype. Frames from an unknown sender
 * are handled only in phase 1 (access point), and only authentication, probe
 * and association requests. A management frame that repeats the sender's
 * last sequence number is counted and ignored. Queues itself again while
 * frames are left.
 */
void WifiTask_RouteMgmtFrame(void) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlBufferPool* pool = (WlBufferPool*)&work->allocator;
  WlCounters* counters = (WlCounters*)work->counters;
  u32 phase = work->state.unk_24;
  WlRxBuffer* buffer = (WlRxBuffer*)pool->lists[6].first;
  WlRxFrame* frame;
  WlMacHeader* mac;
  u32 type;
  u32 subtype;
  int station;
  u32 sequence;

  if (&buffer->header == WIFI_LIST_END) {
    return;
  }
  frame = &buffer->frame;
  mac = (WlMacHeader*)&frame->frameControl;
  if (frame->addr1[0] & 1) {
    counters->mgmtRxMulticast++;
  } else {
    counters->mgmtRxUnicast++;
  }
  counters->unk_24 += _s32_div_f(frame->unk_08 & 0xf0, 16) - 1;
  type = WIFI_FRAMECTL_FIELDS(mac->frameControl)->type;
  subtype = WIFI_FRAMECTL_FIELDS(mac->frameControl)->subtype;
  station = WifiStation_LookUpOrCreate(mac->addr2);
  frame->station = station;
  if (station == WIFI_NO_PEER) {
    frame->station = 0;
    if (phase == 1 && type == 0) { /* management frame */
      switch (subtype) {
        case 11: /* authentication */
          WifiRx_HandleAuthFrame(frame);
          break;
        case 4: /* probe request */
          WifiParent_AnswerProbe(frame);
          break;
        case 0: /* association request */
          WifiParent_AnswerAssocAsk(frame);
          break;
      }
    }
  } else {
    WifiStation_StoreSignal(station, frame->unk_12 & 0xff);
    if (type == 0) {
      sequence = frame->sequence;
      if (sequence == WifiStation_LastSeqOf(station)) {
        counters->unk_3c++;
        goto done;
      }
      WifiStation_StoreLastSeq(station, sequence);
    }
    switch (phase) {
      case 1: /* we are the access point */
        func_arm7_027cb364(
            station, WIFI_FRAMECTL_FIELDS(mac->frameControl)->powerManagement);
        if (type == 0) {
          switch (subtype) {
            case 8: /* beacon */
              WifiRx_HandleBeaconFrame(frame);
              break;
            case 0: /* association request */
              WifiParent_AnswerAssocAsk(frame);
              break;
            case 2: /* reassociation request */
              WifiParent_AnswerReassocAsk(frame);
              break;
            case 4: /* probe request */
              WifiParent_AnswerProbe(frame);
              break;
            case 5: /* probe response */
              WifiScan_RecordHeardNetwork(frame, NULL);
              break;
            case 10: /* disassociation */
              WifiRx_HandleDisassocFrame(frame);
              break;
            case 11: /* authentication */
              WifiRx_HandleAuthFrame(frame);
              break;
            case 12: /* deauthentication */
              WifiRx_HandleDeauthFrame(frame);
              break;
          }
        } else if (type == 1 && subtype == 10) { /* PS-Poll control frame */
          WifiParent_ServePsPoll(frame);
        }
        break;
      case 2: /* we are a station */
      case 3:
        if (type == 0) {
          switch (subtype) {
            case 8: /* beacon */
              WifiRx_HandleBeaconFrame(frame);
              break;
            case 1: /* association response */
              WifiChild_TakeAssocAnswer(frame);
              break;
            case 3: /* reassociation response */
              WifiChild_TakeReassocAnswer(frame);
              break;
            case 5: /* probe response */
              WifiScan_RecordHeardNetwork(frame, NULL);
              break;
            case 10: /* disassociation */
              WifiRx_HandleDisassocFrame(frame);
              break;
            case 11: /* authentication */
              WifiRx_HandleAuthFrame(frame);
              break;
            case 12: /* deauthentication */
              WifiRx_HandleDeauthFrame(frame);
              break;
          }
        } else if (type == 1) { /* control frame */
          switch (subtype) {
            case 14: /* CF-End */
            case 15: /* CF-End + CF-Ack */
              func_arm7_027d2090(frame);
              break;
          }
        }
        break;
    }
  }
done:
  WifiBuffer_Release(&pool->lists[6], &buffer->header);
  if (pool->lists[6].count != 0) {
    WifiTask_Schedule(1, 7);
  }
}
