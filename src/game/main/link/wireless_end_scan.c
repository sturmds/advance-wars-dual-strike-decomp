#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/wireless_event.h"

void Wireless_OnEndScan(WirelessEvent* event);
int WM_EndScan(void (*callback)(WirelessEvent* event));

/* Ends the scan while a scan runs (Unk_0215fe84.scanning): starts a request
 * that reports to Wireless_OnEndScan and clears the flag. True when nothing is
 * left to do or the request started. */
BOOL Wireless_EndScan(void) {
  BOOL ok;

  if (data_0215fe84->scanning == 0) {
    return TRUE;
  }
  if (WM_EndScan(Wireless_OnEndScan) != 2) {
    ok = FALSE;
  } else {
    data_0215fe84->scanning = 0;
    ok = TRUE;
  }
  return ok;
}
