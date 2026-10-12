#include "types.h"
#include "sdk/nvram.h"

/* Sends the instruction that wakes the flash from deep power-down. */
void NVRAM_ReleasePowerDown(void) {
  NVRAM_SpiWait();
  REG_SPICNT = NVRAM_SPICNT_LAST;
  REG_SPIDATA = NVRAM_INSTRUCTION_RELEASE_POWER_DOWN;
  NVRAM_SpiWait();
}
