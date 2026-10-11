#include "types.h"

void gGxVramBankMasks(void);

void func_0210a814(void)
{
    Vram_SwitchOffAndUnlock(gGxVramBankMasks);
}
