#include "types.h"
#include "sdk/mi.h"
#include "sdk/wl.h"

u32 WifiStation_LookUpOrCreate(const u16* macAddress);
void WifiRx_ScanElements(WlElements* elements);
u8 WL_ReadByte(const void* addr);
void WifiScan_RecordHeardNetwork(WlRxFrame* frame, WlElements* elements);
void func_arm7_027c8594(void); /* cancels the one-shot alarm */
int WifiRf_TuneChannel(u16 channel, int fromFlash);
int WifiParam_StorePostBeaconTime(u16 value, int writeReg);
int WifiParam_ChangeListenPeriod(u16 interval);
int WifiParam_ChangeBeaconPeriod(u16 interval);
void func_arm7_027c90ec(u16* dest,
                        const u16* src); /* copies a 6-byte MAC address */
void WifiTx_FirePsPoll(void);
int func_arm7_027c94c0(int value);
void WifiScan_RememberNetwork(u16 value, WlRxFrame* frame,
                              const u8* ssidElement);

/*
 * Handles a received beacon: counts it, finds or adds the sender's station
 * and parses the elements. During a scan (MLME state 0x13) a matching beacon
 * is recorded; otherwise a beacon from our BSS completes a pending join
 * (state 0x21) and, on the child side (WlLink.unk_0c 2 or 3), takes the
 * Nintendo element, sets the microsecond compare to the next beacon time and
 * reads the TIM to send a PS-Poll when the parent holds frames for us.
 */
void WifiRx_HandleBeaconFrame(WlRxFrame* frame) {
  u64 next;
  /* Needed to match: two u64s, not an array; mwcc puts arrays after the scalars on the stack. */
  u64 now;
  u64 nowAgain;
  volatile u32 zero;
  u16* half;
  u32 bit;
  u32 elapsed;
  const u16* body;
  WlLink* link;
  WlMlme* mlme;
  WlStateView* view;
  u32 first;
  WlBufferLists* lists;
  u32 last;
  u32 low;
  u32 bodyLength;
  WlElements elements;
  WlWork* work;
  u64 interval;
  u8 dtimCount;
  u32 station;
  const u8* element;
  work = WIFI_DRIVER_STATE;
  link = WIFI_LINK_VIEW_OF(work);
  mlme = WIFI_ACTIVE_JOB_OF(work);
  view = WIFI_FROM_STATION_TABLE_OF(work);
  lists = WIFI_LISTS_VIEW_OF(work);

  work->counters[13]++;
  station = WifiStation_LookUpOrCreate(frame->addr2);
  frame->station = station;
  if (station == WIFI_NO_PEER) {
    return;
  }
  WifiStation_StoreSignal(station, frame->unk_12);
  body = frame->body;
  bodyLength = frame->bodyLength;
  if (bodyLength <= 12) {
    return;
  }

  zero = 0;
  Mem_FillWords_Arm7(zero, &elements, sizeof(elements));
  elements.data = (const u8*)(body + 6);
  elements.length = bodyLength - 12;
  elements.result = WIFI_IE_FITS_CHANNEL;
  if (link->ssidLength == 0) {
    elements.result |= WIFI_IE_FITS_SSID;
  }
  elements.present = WIFI_IE_WANT_BSSID_TEST | WIFI_IE_WANT_CAPS_TEST;
  elements.rxStatus = frame->unk_08;
  elements.capability = body[5];
  WifiRx_ScanElements(&elements);

  element = elements.cfParams;
  if (element != NULL && (frame->duration & 0x8000)) {
    low = WL_ReadByte(element + 6);
    REG_W_CONTENTFREE = low + (WL_ReadByte(element + 7) << 8);
  }

  if (mlme->state == 0x13 && mlme->scan.request->unk_38 == 1) {
    if ((elements.result & (WIFI_IE_FITS_SSID | WIFI_IE_FITS_BSSID)) ==
        (WIFI_IE_FITS_SSID | WIFI_IE_FITS_BSSID)) {
      WifiScan_RecordHeardNetwork(frame, &elements);
    }
    goto done;
  }
  if (!(elements.result & WIFI_IE_FITS_BSSID)) {
    goto done;
  }
  if (elements.present & WIFI_IE_SEEN_NETWORK_NAME) {
    if ((elements.present & WIFI_IE_SEEN_NETWORK_NAME) == 0) {
      goto done;
    }
    if ((elements.result & WIFI_IE_FITS_SSID) == 0) {
      goto done;
    }
  }

  if (mlme->state == 0x21) {
    func_arm7_027c8594();
    if ((elements.result & (WIFI_IE_FITS_NET_KIND | WIFI_IE_FITS_PRIVACY)) !=
        (WIFI_IE_FITS_NET_KIND | WIFI_IE_FITS_PRIVACY)) {
      mlme->task1.result = 12;
      mlme->task1.unk_06 = 10;
    } else if (!(elements.result & WIFI_IE_FITS_RATES)) {
      mlme->task1.result = 12;
      mlme->task1.unk_06 = 18;
    } else if (WIFI_MANAGEMENT_FIELDS_OF(frame)->probeResponse.beaconInterval >
               1000) {
      mlme->task1.result = 12;
      mlme->task1.unk_06 = 1;
    } else {
      mlme->task1.result = WIFI_ANSWER_OK;
      if ((elements.present & WIFI_IE_SEEN_DS_SET) &&
          !(elements.result & WIFI_IE_FITS_CHANNEL)) {
        WifiRf_TuneChannel(elements.channel, 0);
      }
      WifiStation_StoreRates(station, elements.supportedRates);
      if (link->unk_0c == 2) {
        element = elements.nintendo;
        if (element != NULL) {
          low = WL_ReadByte(element + 6);
          WifiParam_StorePostBeaconTime(low + (WL_ReadByte(element + 7) << 8),
                                        1);
          low = WL_ReadByte(elements.nintendo + 8);
          WIFI_SHARED_GAME_WORD =
              low + (WL_ReadByte(elements.nintendo + 9) << 8);
        } else {
          WifiParam_StorePostBeaconTime(0xffff, 1);
          WIFI_SHARED_GAME_WORD = 0;
        }
      }
      WifiParam_ChangeListenPeriod(WL_ReadByte(elements.tim + 3));
      link->unk_76 = WL_ReadByte(elements.tim + 2);
      WifiParam_ChangeBeaconPeriod(
          WIFI_MANAGEMENT_FIELDS_OF(frame)->probeResponse.beaconInterval);
      link->unk_12 = 1;
      link->unk_1a = 1;
      if (link->unk_0c == 2) {
        WIFI_MAC_POWER_AUX = 3;
      }
      WIFI_MAC_WAKE_CONTROL |= 1;
    }
    func_arm7_027c90ec(mlme->task1.reply->words, frame->addr2);
    mlme->state = 0x25;
    WifiTask_Schedule(2, 1);
  }

  switch (link->unk_0c) {
    case 2:
      element = elements.nintendo;
      if (element != NULL) {
        low = WL_ReadByte(element + 6);
        WifiParam_StorePostBeaconTime(low + (WL_ReadByte(element + 7) << 8), 0);
        low = WL_ReadByte(elements.nintendo + 8);
        WIFI_SHARED_GAME_WORD = low + (WL_ReadByte(elements.nintendo + 9) << 8);
        link->gameInfoLength = WL_ReadByte(elements.nintendo + 1) - 8;
        if (link->gameInfoLength != 0) {
          if ((u32)elements.nintendo & 1) {
            Mem_CopyHalfwords_Arm7(elements.nintendo + 9, link->gameInfo,
                                   link->gameInfoLength + 2);
            link->unk_a2 = 1;
          } else {
            Mem_CopyHalfwords_Arm7(elements.nintendo + 10, link->gameInfo,
                                   link->gameInfoLength + 1);
            link->unk_a2 = 0;
          }
        }
      }
      /* fall through */
    case 3:
      link->unk_80 = 0;
      WifiStation_RefreshTimeout(station);

      /* next target beacon time: the timestamp rounded up to the next beacon interval */
      next = *(u64*)WIFI_MANAGEMENT_FIELDS_OF(frame)->probeResponse.timestamp;
      interval = (u32)link->beaconInterval << 10;
      next = _ll_udiv_Arm7(next, interval);
      next = next + 1;
      next = next * interval;
      half = (u16*)&next;
      REG_W_US_COMPARE3 = half[3];
      REG_W_US_COMPARE2 = half[2];
      REG_W_US_COMPARE1 = half[1];
      REG_W_US_COMPARE0 = half[0] | 1;

      if (link->unk_0c == 2 && link->unk_1a) {
        int irq;

        next -= interval;
        irq = Cpu_MaskIrq_Arm7();
        ((u16*)&now)[0] = REG_W_US_COUNT0;
        ((u16*)&now)[1] = REG_W_US_COUNT1;
        ((u16*)&now)[2] = REG_W_US_COUNT2;
        ((u16*)&now)[3] = REG_W_US_COUNT3;
        ((u16*)&nowAgain)[0] = REG_W_US_COUNT0;
        ((u16*)&nowAgain)[1] = REG_W_US_COUNT1;
        ((u16*)&nowAgain)[2] = REG_W_US_COUNT2;
        ((u16*)&nowAgain)[3] = REG_W_US_COUNT3;
        Cpu_RestoreIrqBit_Arm7(irq);
        if (((u16*)&now)[0] < ((u16*)&nowAgain)[0]) {
          elapsed = (now - next) >> 10;
        } else {
          elapsed = (nowAgain - next) >> 10;
        }
        if (elapsed < view->state.unk_10) {
          WIFI_POST_BEACON_TIMER = view->state.unk_10 - elapsed;
        } else {
          WIFI_POST_BEACON_TIMER = 0;
        }
      }

      if (link->mode == WIFI_MODE_JOINED && elements.tim != NULL &&
          link->unk_0e == 1) {
        dtimCount = WL_ReadByte(elements.tim + 2);
        if (link->unk_76 != dtimCount) {
          link->unk_76 = dtimCount;
        }
        link->unk_8e = 0;
        if (dtimCount == 0 && (WL_ReadByte(elements.tim + 4) & 1)) {
          link->unk_8e |= 1;
        }
        low = WL_ReadByte(elements.tim + 4) & 0xfe;
        first = low << 3;
        last = (low + WL_ReadByte(elements.tim + 1) - 3) << 3;
        if (first <= link->aid && link->aid <= last) {
          bit = link->aid - first;
          if ((1 << (bit & 7)) & WL_ReadByte(elements.tim + 5 + (bit >> 3))) {
            link->unk_8e |= 2;
            WifiTx_FirePsPoll();
          }
        }
        if (lists->txQueues[0].count == 0 && lists->txQueues[1].count == 0 &&
            link->unk_8e == 0) {
          func_arm7_027c94c0(1);
        }
      }
      break;
  }

  if (view->state.flags.bit6) {
    WifiHost_TellBeaconHeard((Unk_027cb9e4*)frame);
  }
done:
  if (elements.ssid != NULL) {
    WifiScan_RememberNetwork(elements.channel, frame, elements.ssid);
  }
}
