#include "types.h"

void func_arm7_037fe298(int, void *);
void SndArm7_OnPxiReceive(void);

void func_arm7_0380239c(void)
{
    func_arm7_037fe298(7, (void *)SndArm7_OnPxiReceive);
}
