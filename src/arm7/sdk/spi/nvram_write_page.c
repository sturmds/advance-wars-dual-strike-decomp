#include "types.h"
#include "sdk/nvram.h"

/* Writes size bytes from src at address with the page write instruction,
 * which erases and programs. A write cannot cross a page, so size is cut
 * at the end of the page that holds address. */
void NVRAM_WritePage(u32 address, u16 size, const u8* src) {
  u16 addressBytes[NVRAM_ADDRESS_BYTES];
  int i;

  if (size < 1) { /* "== 0" compiles to a different compare */
    return;
  }
  if ((address + size - 1) / NVRAM_PAGE_SIZE > address / NVRAM_PAGE_SIZE) {
    size = (u16)(NVRAM_PAGE_SIZE - (address & 0xff));
  }
  addressBytes[0] = (u16)((address & 0xff0000) >> 16);
  addressBytes[1] = (u16)((address & 0xff00) >> 8);
  addressBytes[2] = (u16)(address & 0xff);

  NVRAM_SpiWait();
  REG_SPICNT = NVRAM_SPICNT_HOLD;
  REG_SPIDATA = NVRAM_INSTRUCTION_PW;
  for (i = 0; i < NVRAM_ADDRESS_BYTES; i++) {
    NVRAM_SpiWait();
    REG_SPIDATA = addressBytes[i] & 0xff;
  }
  for (i = 0; i < size - 1; i++) {
    NVRAM_SpiWait();
    REG_SPIDATA = src[i] & 0xff;
  }
  NVRAM_SpiWait();
  REG_SPICNT = NVRAM_SPICNT_LAST;
  REG_SPIDATA = src[i] & 0xff;
  NVRAM_SpiWait();
}
