#include "types.h"
#include "sdk/wmsp.h"

void Wmsp_ResetRssiHistory(u8 rssi) {
  int i;
  u8* work = (u8*)WMSP_WORK;

  for (i = 0; i < 16; i++) {
    ((WmspState*)(work + WMSP_STATE_OFFSET))->history[i] = rssi;
  }
  data_arm7_027d7c44.historyIndex = 0;
}
