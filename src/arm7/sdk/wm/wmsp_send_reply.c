#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"
#include "sdk/pxi.h"

void Wmsp_SendReply(void* reply) {
  while (Fifo_PushWord_Arm7(PXI_FIFO_TAG_WM, (u32)reply, FALSE) < 0) {
    Cpu_DelayLoop_Arm7(0x100);
  }
  Mutex_Release_Arm7(&data_arm7_027d813c);
}
