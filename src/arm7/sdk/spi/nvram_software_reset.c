#include "types.h"
#include "sdk/nvram.h"

/* Sends the software reset instruction, which also clears a failed write or
 * erase from the status register. */
void NVRAM_SoftwareReset(void) {
  NVRAM_SpiWait();
  REG_SPICNT = NVRAM_SPICNT_LAST;
  REG_SPIDATA = NVRAM_INSTRUCTION_SOFTWARE_RESET;
  NVRAM_SpiWait();
}
