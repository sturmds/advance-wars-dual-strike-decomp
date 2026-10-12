#include "types.h"
#include "sdk/spi_thread.h"

/* Gives the bus to device. */
void SPIi_GetException(u32 device) {
  data_arm7_03809ab0.busLocked = TRUE;
  data_arm7_03809ab0.lockDevice = device;
}
