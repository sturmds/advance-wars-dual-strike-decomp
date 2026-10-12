#include "types.h"
#include "sdk/pm.h"

/* Reads one power chip register: send the register number with the read bit, then clock out a byte. */
u8 PMi_GetRegister(int reg)
{
    while (*REG_SPI_CNT & SPI_BUSY) {
    }
    func_arm7_038052e8(1);
    *REG_SPI_DATA = (u8)(reg | POWER_CHIP_READ_BIT) & 0xff; /* needed to match: cast and mask */
    while (*REG_SPI_CNT & SPI_BUSY) {
    }
    func_arm7_038052e8(0);
    *REG_SPI_DATA = 0;
    while (*REG_SPI_CNT & SPI_BUSY) {
    }
    /* needed to match: the casts and masks give the original's extra and/shift pairs */
    return (u16)(*REG_SPI_DATA & 0xff);
}
