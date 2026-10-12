#include "types.h"
#include "sdk/nvram.h"

/* Clears the flash's write enable latch, so it refuses programs and erases. */
void NVRAM_WriteDisable(void) {
  NVRAM_SpiWait();
  REG_SPICNT = NVRAM_SPICNT_LAST;
  REG_SPIDATA = NVRAM_INSTRUCTION_WRDI;
  NVRAM_SpiWait();
}
