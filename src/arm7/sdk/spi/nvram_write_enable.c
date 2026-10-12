#include "types.h"
#include "sdk/nvram.h"

/* Sets the flash's write enable latch, which each program or erase needs. */
void NVRAM_WriteEnable(void)
{
    NVRAM_SpiWait();
    REG_SPICNT = NVRAM_SPICNT_LAST;
    REG_SPIDATA = NVRAM_INSTRUCTION_WREN;
    NVRAM_SpiWait();
}
