#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0229c960.h"
#include "sdk/wm.h"
#include "sdk/os.h"
#include "game/wireless.h"

extern u32 data_0229c880;

int func_02079180(void);
int Wireless_EndScan(void);
int Wireless_PrepareScan(void);
void func_02079ee0(void);
void Wireless_AddParent(u8 linkLevel);
BOOL Wireless_StartScan(void);

/* Scan callback (Wireless_StartScan passes it to the SDK). In state 3 a
 * found parent with our game group id and no download play flag goes to
 * Wireless_AddParent, then the scan restarts while data_0215fe84->scanning is set. */
void Wireless_OnScan(WMStartScanCallback* cb) {
  if (cb->errcode == 0) {
    switch (data_0229c880) {
      case 0:
        func_02079180();
        return;
      case 4:
        if (Wireless_EndScan()) {
          return;
        }
        func_02079180();
        return;
      case 5:
        if (Wireless_EndScan()) {
          return;
        }
        func_02079180();
        return;
      case 3:
        switch (cb->state) {
          case WM_STATECODE_PARENT_FOUND:
            DC_InvalidateRange(gWirelessScanBuffer, sizeof(WMBssDesc));
            /* the beacon must reach past gameInfo.ggid (0x48 bytes) */
            if (gWirelessScanBuffer->length * 2 >= 0x48 &&
                gWirelessScanBuffer->gameInfo.ggid == data_0229c960.ggid &&
                !(gWirelessScanBuffer->gameInfo.attribute & WM_ATTR_FLAG_MB)) {
              Wireless_AddParent(cb->linkLevel);
            }
            /* fall through */
          case WM_STATECODE_PARENT_NOT_FOUND:
            if (data_0215fe84->scanning == 0) {
              return;
            }
            if (!Wireless_PrepareScan()) {
              func_02079ee0();
              return;
            }
            if (Wireless_StartScan()) {
              return;
            }
            func_02079180();
            return;
          default:
            data_0229c880 = 0;
            func_02079180();
            return;
        }
    }
  } else {
    data_0229c880 = 3;
    func_02079180();
  }
}
