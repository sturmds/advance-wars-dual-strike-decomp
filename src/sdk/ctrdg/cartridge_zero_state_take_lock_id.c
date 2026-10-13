#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/os.h"

/* SVC_CpuSet control word: fill mode, 32-bit units, one unit. */
#define CPUSET_FILL_32BIT_ONE_UNIT 0x05000001

void Cartridge_ZeroStateTakeLockId(void) {
  u32 zero = 0;

  SVC_CpuSet(&zero, &data_022acf14, CPUSET_FILL_32BIT_ONE_UNIT);
  data_022acf14.lockID = OS_GetLockID();
}
