#include "types.h"
#include "sdk/nvram.h"

/* Sends the sector erase instruction for the sector that holds address. */
void NVRAM_EraseSector(u32 address)
{
    u16 addressHigh = (u16)((address & 0xff0000) >> 16);
    u16 addressMid = (u16)((address & 0xff00) >> 8);
    u16 addressLow = (u16)(address & 0xff);

    NVRAM_SpiWait();
    REG_SPICNT = NVRAM_SPICNT_HOLD;
    REG_SPIDATA = NVRAM_INSTRUCTION_SE;
    NVRAM_SpiWait();
    REG_SPIDATA = addressHigh & 0xff;
    NVRAM_SpiWait();
    REG_SPIDATA = addressMid & 0xff;
    NVRAM_SpiWait();
    REG_SPICNT = NVRAM_SPICNT_LAST;
    REG_SPIDATA = addressLow & 0xff;
    NVRAM_SpiWait();
}
