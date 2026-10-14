#include "sdk/mi.h"
#include "sdk/wl.h"

/* Body bytes of the test frame, filled with 0x5555. */
#define TEST_FRAME_BODY_BYTES 0x5f8

void Wifi_OpenSession(void);
void Wifi_CloseSession(void);
void WifiRf_TuneChannel(int channel, int arg1);
void func_arm7_027c87b8(int a0);

// Builds a frame of 0x5555 halfwords in WIFI_PACKET_RAM and starts sending it (signal
// 3 or 4; signal 4 also clears baseband register 6 until the test stops).
void WifiTest_SendPatternFrames(WlTestModeRequest* request) {
  WlLink* link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
  volatile u16* ram =
      WIFI_PACKET_RAM; /* needed to match: keeps the last line unfolded */
  u32 i;
  volatile u16* p;

  Wifi_OpenSession();
  Wifi_CloseSession();
  link->savedBbReg6 = WifiBaseband_Get(6);
  if (request->signal == 4) {
    WifiBaseband_Put(6, 0);
  }
  {
    volatile u16 zero = 0;
    Mem_FillHalfwords_Arm7(zero, (void*)ram,
                           offsetof(WlRamFrame, frameControl));
  }
  ram[4] = 20;
  ram[5] = 2000;
  p = &ram[6];
  for (i = 0; i < TEST_FRAME_BODY_BYTES; i += 2) {
    *p++ = 0x5555;
  }
  ram[6] = 8;
  WIFI_TX_HEADER_CONTROL = 6;
  link->mode = WIFI_MODE_FRAME_TEST;
  link->testTxActive = 1;
  WifiRf_TuneChannel(request->channel, 1);
  WIFI_MAC_FORCE_POWER = 0x8000;
  func_arm7_027c87b8(1500);
  WIFI_IRQ_ENABLE_MASK = 2;
  WIFI_MAC_RUN = 1;
  WIFI_TX_REQUEST_RAISE = 1;
  WIFI_TX_LOC1_FRAME_ADDR = 0x8000 | (u16)(((u32)ram & 0x3fff) >> 1);
}
