#include "types.h"
#include "sdk/nvram.h"

/* Reads the flash status register into status. */
void NVRAM_ReadStatus(u8* status) {
  NVRAM_SpiWait();
  REG_SPICNT = NVRAM_SPICNT_HOLD;
  REG_SPIDATA = NVRAM_INSTRUCTION_RDSR;
  NVRAM_SpiWait();
  REG_SPICNT = NVRAM_SPICNT_LAST;
  REG_SPIDATA = 0;
  NVRAM_SpiWait();
  *status = NVRAM_SpiReceived();
}
