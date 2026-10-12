#include "types.h"
#include "sdk/os_arm7.h"

int func_arm7_037fe1cc(u32 tag, u32 data, int err);

void func_arm7_037fdc78(u32 command)
{
    while (func_arm7_037fe1cc(FIFO_TAG_RESET, command << 8, 0) != 0) {
    }
}
