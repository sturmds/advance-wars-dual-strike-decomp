#include "sdk/mic.h"

/* Reads one 12-bit microphone sample from the touch screen controller's ADC.
 * Returns it in the top 12 bits of a halfword. */
u16 func_arm7_038060bc(void) {
  u16 high;
  u16 low;

  while (REG_SPICNT & SPICNT_BUSY) {
  }
  REG_SPICNT =
      SPICNT_ENABLE | SPICNT_HOLD | SPICNT_DEVICE_TSC | SPICNT_BAUD_2MHZ;
  REG_SPIDATA = TSC_READ_MIC_12BIT;
  while (REG_SPICNT & SPICNT_BUSY) {
  }
  REG_SPIDATA = 0;
  while (REG_SPICNT & SPICNT_BUSY) {
  }
  high = (u16)(REG_SPIDATA & 0xff) << 8; /* the cast is needed to match */
  REG_SPICNT = SPICNT_ENABLE | SPICNT_DEVICE_TSC | SPICNT_BAUD_2MHZ;
  REG_SPIDATA = 0;
  while (REG_SPICNT & SPICNT_BUSY) {
  }
  low = REG_SPIDATA & 0xff;
  return ((high | low) & 0x7ff8) << 1;
}
