#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/pxi.h"

void func_arm7_037fdc78(u32 command)
{
    while (Fifo_PushWord_Arm7(FIFO_TAG_RESET, command << 8, 0) != 0) {
    }
}
