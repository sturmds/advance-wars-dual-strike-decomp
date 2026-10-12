#include "types.h"
#include "sdk/nvram.h"

/* Sends the instruction that puts the flash into deep power-down. */
void NVRAM_DeepPowerDown(void) {
  NVRAM_SpiWait();
  REG_SPICNT = NVRAM_SPICNT_LAST;
  REG_SPIDATA = NVRAM_INSTRUCTION_DEEP_POWER_DOWN;
  NVRAM_SpiWait();
}
