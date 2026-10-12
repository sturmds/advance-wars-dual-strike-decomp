#include "types.h"
#include "sdk/spi_thread.h"

/* TRUE when no device holds the bus. device goes unused, but every caller passes its own. */
BOOL SPIi_CheckException(u32 device) {
  return (data_arm7_03809ab0.busLocked == FALSE) ? TRUE : FALSE;
}
