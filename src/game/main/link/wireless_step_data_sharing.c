#include "types.h"
#include "game/wireless.h"
#include "sdk/wm.h"

int WM_StepDataSharing(void* sharing, void* sendData, WMDataSet* receiveData);

/* Sends one 8-byte block through the data sharing session and, when the step
 * succeeds, copies the received set's receivedBitmap to *out. True on success. */
BOOL Wireless_StepDataSharing(void* sendData, u16* out) {
  BOOL ok;

  if (WM_StepDataSharing(data_0229c8ac, sendData, data_0229c8b0) != 0) {
    ok = FALSE;
  } else {
    *out = data_0229c8b0->receivedBitmap;
    ok = TRUE;
  }
  return ok;
}
