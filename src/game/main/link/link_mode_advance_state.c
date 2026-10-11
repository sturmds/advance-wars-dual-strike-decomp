#include "types.h"

int func_0207bd80(void);
void func_0207b4e8(void);

/* In link mode (func_0207bd80 returns 1), runs func_0207b4e8: link state 2 to
 * 3. */
void LinkMode_AdvanceState(void)
{
    if (func_0207bd80() != 1) {
        return;
    }
    func_0207b4e8();
}
