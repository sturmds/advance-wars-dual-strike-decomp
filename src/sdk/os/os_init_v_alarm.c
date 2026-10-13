#include "types.h"
#include "sdk/os.h"

#define IRQ_MASK_VCOUNT 0x4

static u16 gOsVAlarmInitialized;
static u32 data_022a9c24;
static u32 data_022a9c20;
static u64 data_022a9c28;

/* Once only: clears the V-count alarm state and turns the V-count interrupt off. */
void OS_InitVAlarm(void) {
  if (gOsVAlarmInitialized != 0) {
    return;
  }
  gOsVAlarmInitialized = 1;
  data_022a9c28 = 0;
  OS_DisableIrqMask(IRQ_MASK_VCOUNT);
  data_022a9c24 = 0;
  data_022a9c20 = 0;
}
