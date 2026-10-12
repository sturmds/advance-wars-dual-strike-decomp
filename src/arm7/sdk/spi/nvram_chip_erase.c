#include "types.h"
#include "sdk/nvram.h"

/* Sends the chip erase instruction; the flash then erases itself whole. */
void NVRAM_ChipErase(void)
{
    NVRAM_SpiWait();
    REG_SPICNT = NVRAM_SPICNT_LAST;
    REG_SPIDATA = NVRAM_INSTRUCTION_CHIP_ERASE;
    NVRAM_SpiWait();
}
