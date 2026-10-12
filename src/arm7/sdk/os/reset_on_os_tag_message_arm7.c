#include "types.h"
#include "sdk/os_arm7.h"

/* Set to 1 when the ARM9 sends the reset command; Reset_WasRequested_Arm7 reads it. */
extern u16 data_arm7_03807ab0;

/* PXI receive callback for FIFO_TAG_RESET: the ARM7 half of
 * Reset_OnOsTagMessage. Any command but reset halts the CPU. */
void Reset_OnOsTagMessage_Arm7(u32 tag, u32 data, int err) {
  /* The command is bits 8 to 14 of data; shifting up before down is needed to match. */
  if ((((data & 0x7f00) << 8) >> 16) == OS_PXI_COMMAND_RESET) {
    data_arm7_03807ab0 = 1;
  } else {
    Cpu_HaltForever_Arm7();
  }
}
