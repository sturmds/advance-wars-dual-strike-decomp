#include "sdk/rtc.h"

int func_arm7_037fe1cc(u32 tag, u32 data, int err);

void func_arm7_027d58a8(int command, int result)
{
    u32 word = CLOCK_ANSWER_BIT | ((command << 8) & 0x7f00) | (result & 0xff);

    while (func_arm7_037fe1cc(PXI_FIFO_TAG_RTC, word, 0) < 0) {
    }
}
