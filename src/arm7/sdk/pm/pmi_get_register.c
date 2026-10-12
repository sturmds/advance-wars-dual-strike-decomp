#include "types.h"
#include "sdk/spi.h"
#include "sdk/pm.h"

/* Reads one power chip register: send the register number with the read bit, then clock out a byte. */
u8 PMi_GetRegister(int reg) {
  while (REG_SPICNT & SPICNT_BUSY) {
  }
  func_arm7_038052e8(1);
  REG_SPIDATA = (u8)(reg | POWER_CHIP_READ_BIT) &
                0xff; /* needed to match: cast and mask */
  while (REG_SPICNT & SPICNT_BUSY) {
  }
  func_arm7_038052e8(0);
  REG_SPIDATA = 0;
  while (REG_SPICNT & SPICNT_BUSY) {
  }
  /* needed to match: the casts and masks give the original's extra and/shift pairs */
  return (u16)(REG_SPIDATA & 0xff);
}
