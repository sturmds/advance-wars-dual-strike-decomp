#include "types.h"
#include "sdk/wmsp.h"

void Wmsp_PushRssiSample(u8 rssi) {
  data_arm7_027d8160[data_arm7_027d7c44.historyIndex] = rssi;
  data_arm7_027d7c44.historyIndex = (data_arm7_027d7c44.historyIndex + 1) & 15;
}
