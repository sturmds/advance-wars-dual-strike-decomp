#include "types.h"
#include "game/Unk_0215fe84.h"

extern u32 data_0229c880;
extern void* data_0229c8ac;

int WM_StartDataSharing(void* sharing, int port, int aidBitmap, int dataLength,
                        int doubleMode);

/* Starts data sharing on port 13 with all aids (bitmap 0xff) and 8 bytes per
 * aid; only in link state 4 or 5. True when it runs. */
BOOL Wireless_StartDataSharing(void) {
  BOOL started;

  if (data_0215fe84->dataSharingActive == 1) {
    return TRUE;
  }
  switch (data_0229c880) {
    case 4:
    case 5:
      break;
    default:
      return FALSE;
  }
  if (WM_StartDataSharing(data_0229c8ac, 13, 0xff, 8, data_0215fe84->unk_0c) !=
      0) {
    started = FALSE;
  } else {
    data_0215fe84->dataSharingActive = 1;
    started = TRUE;
  }
  return started;
}
