#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/ctrdg.h"
#include "sdk/pxi.h"

void Cartridge_SendWordUntilTaken_Arm7(u32 word) {
  while (Fifo_PushWord_Arm7(FIFO_TAG_GBA_SLOT, word, 0) != 0) {
    Cpu_DelayLoop_Arm7(1);
  }
}
