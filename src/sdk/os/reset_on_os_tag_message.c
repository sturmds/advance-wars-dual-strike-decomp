#include "types.h"
#include "sdk/os.h"

/* Set to 1 when the reset command arrives; OS_ResetSystem waits for it. */
extern volatile u16 data_022a9c34;

void Reset_OnOsTagMessage(u32 tag, u32 data, int err) {
  /* The command is bits 8 to 14 of data; shifting up before down is needed to match. */
  if ((((data & 0x7f00) << 8) >> 16) == OS_PXI_COMMAND_RESET) {
    data_022a9c34 = 1;
  } else {
    OS_Terminate();
  }
}
