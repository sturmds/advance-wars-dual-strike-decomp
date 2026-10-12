#include "sdk/wmsp.h"

void WMSP_InitConfigRegisters(void) {
  *IO_WIRELESS_INIT_REG_124 = 200;
  *IO_WIRELESS_INIT_REG_128 = 2000;
  *IO_WIRELESS_INIT_REG_150 = 0x202;
}
