#include "types.h"
#include "sdk/spi_thread.h"
#include "sdk/os_arm7.h"

/* Frees the SPI bus if this device holds it, and wakes the threads waiting in SPI_Lock. */
void SPIi_ReleaseException(u32 device) {
  if (data_arm7_03809ab0.lockDevice == device) {
    data_arm7_03809ab0.lockDevice = SERIAL_OWNER_NOBODY;
    data_arm7_03809ab0.busLocked = FALSE;
    Thread_ReadyAllWaiters_Arm7(&data_arm7_03809ab0.waitQueue);
  }
}
