#include "sdk/rtc.h"
#include "sdk/pxi.h"

void func_arm7_027d58a8(int command, int result) {
  u32 word = CLOCK_ANSWER_BIT | ((command << 8) & 0x7f00) | (result & 0xff);

  while (Fifo_PushWord_Arm7(PXI_FIFO_TAG_RTC, word, 0) < 0) {
  }
}
