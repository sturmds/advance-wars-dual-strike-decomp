#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os.h"

void Cartridge_OnInitDoneMessage(u32 tag, u32 data, int err) {
  if ((data & GBA_SLOT_MSG_OPCODE_BITS) == GBA_SLOT_MSG_SETUP_ACK) {
    data_022acf14.commandDone = 1;
  } else {
    OS_Terminate();
  }
}
