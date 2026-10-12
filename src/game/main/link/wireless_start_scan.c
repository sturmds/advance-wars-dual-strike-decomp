#include "types.h"

void Wireless_OnScan(void* event);
int WM_StartScan(void (*callback)(void* event), int arg);

extern int data_0229c8b4;

/* Starts a scan that reports to Wireless_OnScan; true when WM_StartScan
 * returns 2 (WM_ERRCODE_OPERATING). The callers fall through to the failure
 * path on false. */
BOOL Wireless_StartScan(void) {
  return WM_StartScan(Wireless_OnScan, data_0229c8b4) == 2;
}
