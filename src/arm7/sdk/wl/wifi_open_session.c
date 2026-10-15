#include "sdk/wl.h"

void Wifi_CloseSession(void);
void Wifi_SeedLcg(u32 mul, u32 add);
void Wifi_SwitchMode(int mode);
void WifiBeacon_Enable(void);
void func_arm7_027c92e8(void);
int func_arm7_027c94c0(int value);
void WifiStation_ForgetPeers(void);
void WifiScan_ForgetNetworks(void);
void WifiTx_LayOutSlots(void);
void WifiRx_SetUpRing(void);

/* Interrupts every session type enables. */
#define W_IRQ_BASIC                                                  \
  (W_IRQ_RX_DONE | W_IRQ_TX_DONE | W_IRQ_RX_COUNT | W_IRQ_TX_COUNT | \
   W_IRQ_COUNT_OVERFLOW)

/*
 * Stops the current session, reseeds the random generator from the hardware,
 * resets the driver's frame state, then sets the interrupt mask, RX filters
 * and timers for the session type in WlLink.unk_0c and switches the mode:
 * 0 and 1 end in mode 64 (1 also starts the beacon), 2-4 in mode 32. Type 4
 * is the channel measurement (WifiMlme_RunChannelSurvey).
 */
void Wifi_OpenSession(void) {
  WlLink* link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
  WlStateView* view = WIFI_FROM_STATION_TABLE_OF(WIFI_DRIVER_STATE);
  u64 time;
  u16* words;
  u32 interval;

  Wifi_CloseSession();
  Wifi_SeedLcg(REG_W_RANDOM + (REG_W_RANDOM << 8), REG_W_RANDOM);
  /* 802.11 capability: ESS, plus short preamble and privacy when set up */
  link->flags_7c = 1;
  if (view->state.flags.bit2 == 1) {
    link->flags_7c |= 0x20;
  }
  if (view->state.unk_08 != 0) {
    link->flags_7c |= 0x10;
  }
  link->unk_12 = 0;
  REG_W_WEP_CNT = 0x8000;
  WIFI_POST_BEACON_TIMER = 0xffff;
  IO_WIRELESS_STATION_ID = 0;
  IO_WIRELESS_STATION_ID_4BIT = 0;
  WIFI_MAC_WAKE_CONTROL = 15;
  WifiStation_ForgetPeers();
  WifiScan_ForgetNetworks();
  WifiTx_LayOutSlots();
  WifiRx_SetUpRing();
  REG_W_RXCNT = 0x8000;
  WIFI_IRQ_PENDING_FLAGS = 0xffff;
  WIFI_MAC_REGISTER(0x1ae) = 0x1fff;
  if (WIFI_DRIVER_STATE->unk_638 & 8) {
    WIFI_MAC_REGISTER(0x1aa) = 0x400;
  } else {
    WIFI_MAC_REGISTER(0x1aa) = 0;
  }
  WIFI_MAC_REGISTER(0x008) = 0;
  WIFI_MAC_REGISTER(0x00a) = 0;

  switch (link->unk_0c) {
    case 0:
      WIFI_IRQ_ENABLE_MASK = W_IRQ_BASIC;
      WIFI_MAC_REGISTER(0x0d0) = 0xffff;
      WIFI_MAC_REGISTER(0x0e0) = 8;
      WIFI_MAC_REGISTER(0x008) = 0;
      WIFI_MAC_REGISTER(0x00a) = 0;
      WIFI_MAC_REGISTER(0x0e8) = 0;
      WIFI_MAC_RUN = 1;
      Wifi_SwitchMode(64);
      break;
    case 1:
      WIFI_IRQ_ENABLE_MASK =
          W_IRQ_BASIC | W_IRQ_MP_END | W_IRQ_POST_BEACON | W_IRQ_BEACON;
      WIFI_MAC_REGISTER(0x1ae) = 0x1fff;
      WIFI_MAC_REGISTER(0x0d0) = 0x301;
      WIFI_MAC_REGISTER(0x0e0) = 13;
      WIFI_MAC_REGISTER(0x008) = 0xe000;
      WIFI_MAC_RUN = 1;
      /* first beacon: the microsecond counter rounded up to the next
           whole beacon interval (in units of 1024 us); bit 0 enables it */
      words = (u16*)&time;
      words[0] = REG_W_US_COUNT0;
      words[1] = WIFI_MAC_REGISTER(0x0fa);
      words[2] = WIFI_MAC_REGISTER(0x0fc);
      words[3] = WIFI_MAC_REGISTER(0x0fe);
      interval = link->beaconInterval << 10;
      time = _ll_udiv_Arm7(time, interval);
      time += 1;
      time *= interval;
      WIFI_MAC_REGISTER(0x0f6) = words[3];
      WIFI_MAC_REGISTER(0x0f4) = words[2];
      WIFI_MAC_REGISTER(0x0f2) = words[1];
      WIFI_MAC_REGISTER(0x0f0) = words[0] | 1;
      WIFI_MAC_REGISTER(0x0e8) = 1;
      WIFI_MAC_REGISTER(0x0ea) = 1;
      Wifi_SwitchMode(64);
      WifiBeacon_Enable();
      WIFI_TX_REQUEST_RAISE = 2;
      break;
    case 2:
      WIFI_IRQ_ENABLE_MASK =
          W_IRQ_BASIC | W_IRQ_POST_BEACON | W_IRQ_BEACON | W_IRQ_PRE_BEACON;
      if (WIFI_DRIVER_STATE->unk_638 & 0x20) {
        WIFI_IRQ_ENABLE_MASK |= W_IRQ_RX_START | W_IRQ_TX_START;
      }
      if (WIFI_DRIVER_STATE->unk_638 & 0x20) {
        WIFI_MAC_REGISTER(0x1aa) |= 0x68;
      }
      /* RX filter: bit 0 of the BSSID marks a group (broadcast) address */
      if (link->bssid[0] & 1) {
        WIFI_MAC_REGISTER(0x0d0) = 0x581;
      } else {
        WIFI_MAC_REGISTER(0x0d0) = 0x181;
      }
      WIFI_MAC_REGISTER(0x0e0) = 11;
      WIFI_MAC_RUN = 1;
      WIFI_MAC_REGISTER(0x0e8) = 1;
      WIFI_MAC_REGISTER(0x0ea) = 1;
      Wifi_SwitchMode(32);
      break;
    case 3:
      WIFI_IRQ_PENDING_FLAGS = 0xffff;
      WIFI_IRQ_ENABLE_MASK = W_IRQ_BASIC | W_IRQ_BEACON | W_IRQ_PRE_BEACON;
      if (link->bssid[0] & 1) {
        WIFI_MAC_REGISTER(0x0d0) = 0x401;
      } else {
        WIFI_MAC_REGISTER(0x0d0) = 1;
      }
      WIFI_MAC_REGISTER(0x0e0) = 11;
      WIFI_MAC_RUN = 1;
      WIFI_MAC_REGISTER(0x0e8) = 1;
      WIFI_MAC_REGISTER(0x0ea) = 1;
      WIFI_MAC_POWER_AUX = 0;
      Wifi_SwitchMode(32);
      break;
    case 4:
      WIFI_IRQ_ENABLE_MASK = 0;
      WIFI_MAC_REGISTER(0x1ae) = 0;
      WIFI_MAC_RUN = 1;
      Wifi_SwitchMode(32);
      break;
  }
  WIFI_MAC_POWER_AUX = 0;
  func_arm7_027c92e8();
  WIFI_TX_REQUEST_RAISE = 2;
  if (link->unk_0e == 1) {
    func_arm7_027c94c0(2);
  }
  WifiRf_AwaitReceiverOn();
}
