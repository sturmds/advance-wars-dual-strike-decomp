#include "types.h"
#include "sdk/wm.h"

u16 WM_GetAllowedChannel(void);

/* Moves scan->channel to the next allowed channel, wrapping after 16, as
 * Wireless_NextScanChannel does. False when no channel is allowed. The u16
 * counter is needed to match. */
BOOL DownloadPlay_AdvanceToAllowedChannel(WMScanParam* scan) {
  u16 allowed;
  u16 candidate;
  u16 i;
  u16 current;

  allowed = WM_GetAllowedChannel();
  if (allowed == 0) {
    return FALSE;
  }
  current = scan->channel;
  i = 0;
  candidate = current;
  while (i < 16) {
    if (allowed & (1 << (candidate - 1))) {
      if (current != candidate) {
        scan->channel = candidate;
        break;
      }
    }
    i++;
    candidate = (candidate == 16) ? 1 : candidate + 1;
  }
  return TRUE;
}
