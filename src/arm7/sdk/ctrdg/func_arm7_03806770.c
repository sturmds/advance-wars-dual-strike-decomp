#include "types.h"
#include "sdk/ctrdg.h"

void func_arm7_038067b0(const void *src, void *dest, u32 control);   /* SVC_CpuSet through a veneer */
int func_arm7_037fb81c(void);                                      /* OS_GetLockID */

/* SVC_CpuSet control word: fill mode, 32-bit units, one unit. */
#define CPUSET_FILL_32BIT_ONE_UNIT 0x05000001

extern CTRDGWork data_arm7_0380afc0;

/* ARM7 copy of Cartridge_ZeroStateTakeLockId. */
void func_arm7_03806770(void)
{
    u32 zero = 0;

    func_arm7_038067b0(&zero, &data_arm7_0380afc0, CPUSET_FILL_32BIT_ONE_UNIT);
    data_arm7_0380afc0.lockID = func_arm7_037fb81c();
}
