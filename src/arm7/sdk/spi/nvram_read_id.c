#include "types.h"
#include "sdk/nvram.h"

/* Sends the read id instruction and stores the first two id bytes the flash
 * returns. */
void NVRAM_ReadId(u8* id) {
  NVRAM_SpiWait();
  REG_SPICNT = NVRAM_SPICNT_HOLD;
  REG_SPIDATA = NVRAM_INSTRUCTION_READ_ID;
  NVRAM_SpiWait();

  REG_SPIDATA = 0;
  NVRAM_SpiWait();
  id[0] = NVRAM_SpiReceived();

  REG_SPICNT = NVRAM_SPICNT_LAST; /* release the flash after the last byte */
  REG_SPIDATA = 0;
  NVRAM_SpiWait();
  id[1] = NVRAM_SpiReceived();
}
