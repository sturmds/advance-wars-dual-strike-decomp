/* needed to match: the original was built at optimization level 1 */
#pragma optimization_level 1
#include "sdk/card.h"

void OS_WakeupThreadDirect(OSThread* thread);

/* PXI tag 11 message from the other CPU: clears the wait flag and wakes the waiting thread. */
void Card_OnRequestTagMessage(int tag, u32 data, int err) {
  CARDiCommon* common;

  if (tag != 11) {
    return;
  }
  if (err) {
    common = &data_022abb00;
    common->flag &= ~GAMECARD_FLAG_AWAIT_PEER_REPLY;
    OS_WakeupThreadDirect(common->waiter);
  }
}
