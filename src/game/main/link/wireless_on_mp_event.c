#include "types.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"

extern u32 data_0229c880;
extern u32 gWirelessState;

int func_02079180(void);
void Wireless_ClearParents(void);

/* Callback of the MP start in Wireless_StartMp. Each success sets
 * Unk_0215fe84.unk_30. When MP starts in link state 4 or 5, sets unk_08 and
 * gWirelessState to that state and runs Wireless_ClearParents; in state 0 or 3 runs
 * func_02079180. Timeouts, bad poll bitmaps and failed sends are ignored;
 * other errors go back to state 3, set unk_24 and call func_02079180. */
void Wireless_OnMpEvent(WMStartMPCallback* cb) {
  if (cb->errcode == WM_ERRCODE_SUCCESS) {
    data_0215fe84->unk_30 = 1;
    switch (cb->state) {
      case WM_STATECODE_MP_START:
        switch (data_0229c880) {
          case 0:
          case 3:
            func_02079180();
            return;
          case 4:
            data_0215fe84->unk_08 = 1;
            gWirelessState = 4;
            Wireless_ClearParents();
            return;
          case 5:
            data_0215fe84->unk_08 = 1;
            gWirelessState = 5;
            Wireless_ClearParents();
            return;
        }
        break;
      case WM_STATECODE_MPEND_IND:
        return;
      case WM_STATECODE_MP_IND:
        return;
    }
  } else {
    switch (cb->errcode) {
      case WM_ERRCODE_TIMEOUT:
      case WM_ERRCODE_INVALID_POLLBITMAP:
      case WM_ERRCODE_SEND_FAILED:
        return;
    }
    data_0229c880 = 3;
    data_0215fe84->unk_24 = 1;
    func_02079180();
  }
}
