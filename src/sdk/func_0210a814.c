#include "types.h"
#include "sdk/gx.h"

u32 Vram_SwitchOffAndUnlock(u16* banks);

void func_0210a814(void) { Vram_SwitchOffAndUnlock(gGxVramBankMasks); }
