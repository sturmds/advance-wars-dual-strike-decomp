#include "types.h"
#include "sdk/wmsp.h"

int Wmsp_RssiToLinkLevel(u32 rssi);

int Wmsp_GetLinkLevel(void) {
  int i;
  u32 sum = 0;
  // Needed to match: with &WMSP_WORK->state the compiler folds the whole
  // address into one constant instead of keeping the base in a register.
  u8* work = (u8*)WMSP_WORK;

  for (i = 0; i < RSSI_HISTORY_LENGTH; i++) {
    sum += ((WmspState*)(work + WMSP_STATE_OFFSET))->history[i];
  }
  return Wmsp_RssiToLinkLevel(sum >> 4);
}
