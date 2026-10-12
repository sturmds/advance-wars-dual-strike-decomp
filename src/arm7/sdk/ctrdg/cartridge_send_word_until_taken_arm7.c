#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/pxi.h"

void func_arm7_037fdc04(u32 count); /* SVC_WaitByLoop through a veneer */

void Cartridge_SendWordUntilTaken_Arm7(u32 word) {
  while (Fifo_PushWord_Arm7(FIFO_TAG_GBA_SLOT, word, 0) != 0) {
    func_arm7_037fdc04(1);
  }
}
