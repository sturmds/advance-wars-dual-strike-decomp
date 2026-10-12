#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

void func_arm7_037fdc04(int count);

void Wmsp_WaitCallbackFree(void) {
  Mutex_Take_Arm7(&data_arm7_027d813c);
  while (*WIRELESS_CALLBACK_LOCK_WORD & WIRELESS_CALLBACK_BUSY_BIT) {
    func_arm7_037fdc04(0x100);
  }
  Mutex_Release_Arm7(&data_arm7_027d813c);
}
