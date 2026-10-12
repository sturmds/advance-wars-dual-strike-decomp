#include "types.h"
#include "sdk/nvram.h"

/* Reads size bytes at address into dest with the fast read instruction,
 * which wants one dummy byte after the address. Each byte comes in while
 * a zero goes out. */
void NVRAM_FastRead(u32 address, u32 size, u8 *dest)
{
    u16 addressBytes[NVRAM_ADDRESS_BYTES];
    int i;

    if (size < 1) {    /* "== 0" compiles to a different compare */
        return;
    }
    addressBytes[0] = (u16)((address & 0xff0000) >> 16);
    addressBytes[1] = (u16)((address & 0xff00) >> 8);
    addressBytes[2] = (u16)(address & 0xff);

    NVRAM_SpiWait();
    REG_SPICNT = NVRAM_SPICNT_HOLD;
    REG_SPIDATA = NVRAM_INSTRUCTION_FAST_READ;
    for (i = 0; i < NVRAM_ADDRESS_BYTES; i++) {
        NVRAM_SpiWait();
        REG_SPIDATA = addressBytes[i] & 0xff;
    }
    NVRAM_SpiWait();
    REG_SPIDATA = 0;
    NVRAM_SpiWait();
    for (i = 0; i < size - 1; i++) {
        REG_SPIDATA = 0;
        NVRAM_SpiWait();
        dest[i] = NVRAM_SpiReceived();
    }
    REG_SPICNT = NVRAM_SPICNT_LAST;
    REG_SPIDATA = 0;
    NVRAM_SpiWait();
    dest[i] = NVRAM_SpiReceived();
}
