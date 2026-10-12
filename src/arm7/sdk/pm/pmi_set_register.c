#include "types.h"
#include "sdk/pm.h"

/* Writes one power chip register: the register number, then the value. */
void PMi_SetRegister(int reg, int value)
{
    while (*REG_SPI_CNT & SPI_BUSY) {
    }
    func_arm7_038052e8(1);
    *REG_SPI_DATA = (u8)reg & 0xff; /* needed to match: cast and mask */
    while (*REG_SPI_CNT & SPI_BUSY) {
    }
    func_arm7_038052e8(0);
    *REG_SPI_DATA = (u8)value;
}
