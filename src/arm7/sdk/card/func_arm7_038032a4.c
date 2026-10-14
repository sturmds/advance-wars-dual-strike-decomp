#include "types.h"
#include "sdk/card_arm7.h"

/* Sets the number of bytes the next SPI transaction moves. */
void func_arm7_038032a4(u32 byteCount) {
  data_arm7_03809860.remaining = byteCount;
}
