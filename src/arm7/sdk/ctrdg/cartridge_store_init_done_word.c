#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os_arm7.h"

extern GbaSlotInitWord gCtrdgInitMessage;

void Cartridge_StoreInitDoneWord(u32 tag, u32 data, int err) {
  if ((data & GBA_SLOT_MSG_OPCODE_BITS) == GBA_SLOT_MSG_SETUP_ACK) {
    gCtrdgInitMessage.word = data;
    gCtrdgInitMessage.received = 1;
  } else {
    Cpu_HaltForever_Arm7();
  }
}
