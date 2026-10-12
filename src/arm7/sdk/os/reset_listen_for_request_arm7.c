#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/pxi.h"

extern u16 data_arm7_03807aac;

/* The ARM7 half of OS_InitReset: registers the OS tag's PXI callback once.
 * Unlike the ARM9 side it does not wait for the other CPU. */
void Reset_ListenForRequest_Arm7(void) {
  if (data_arm7_03807aac != 0) {
    return;
  }
  data_arm7_03807aac = 1;
  Fifo_RegisterTagHandler_Arm7(FIFO_TAG_RESET, Reset_OnOsTagMessage_Arm7);
}
