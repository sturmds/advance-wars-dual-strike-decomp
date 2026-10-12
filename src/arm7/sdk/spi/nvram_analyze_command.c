#include "types.h"
#include "sdk/nvram.h"
#include "sdk/spi.h"

/* Takes the NVRAM words from the SPI PXI handler. Once a request is complete
 * it unpacks the address, size and ARM9 buffer, checks the buffer lies in
 * main RAM and queues the job for the SPI thread. Requests without some of
 * the three pass whatever the unused locals hold. */
void NVRAM_AnalyzeCommand(u32 data) {
  int i;
  u16 command;
  u32 buffer;
  u32 size;
  u32 address;

  if (data & SERIAL_MSG_FIRST_WORD) {
    for (i = 0; i < NVRAM_PXI_WORDS; i++) {
      nvramPxiWords[i] = 0;
    }
  }

  nvramPxiWords[(data & SERIAL_MSG_SLOT_BITS) >> SERIAL_MSG_SLOT_POS] = data;

  if (!(data & SPI_PXI_END_BIT)) {
    return;
  }

  command = (nvramPxiWords[0] & 0xff00) >> 8;
  switch (command) {
    case FLASH_REQ_STATUS_BYTE:
    case FLASH_REQ_IDENTIFY:
      buffer = ((nvramPxiWords[0] & 0xff) << 24) | (nvramPxiWords[1] << 8) |
               ((u32)(nvramPxiWords[2] & 0xff00) >>
                8); /* u32: the original shifts logically */
      if (buffer < NVRAM_BUFFER_MIN || buffer >= NVRAM_BUFFER_END) {
        SPIi_ReturnResult(command, SPI_PXI_RESULT_INVALID_PARAMETER);
        return;
      }
      break;
    case FLASH_REQ_LOAD:
    case FLASH_REQ_LOAD_QUICK:
      buffer = (nvramPxiWords[4] << 16) | nvramPxiWords[5];
      if (buffer < NVRAM_BUFFER_MIN || buffer >= NVRAM_BUFFER_END) {
        SPIi_ReturnResult(command, SPI_PXI_RESULT_INVALID_PARAMETER);
        return;
      }
      address = ((nvramPxiWords[0] & 0xff) << 16) | nvramPxiWords[1];
      size = (nvramPxiWords[2] << 16) | nvramPxiWords[3];
      break;
    case FLASH_REQ_REWRITE_PAGE:
    case FLASH_REQ_PROGRAM_PAGE:
      buffer = (nvramPxiWords[3] << 16) | nvramPxiWords[4];
      if (buffer < NVRAM_BUFFER_MIN || buffer >= NVRAM_BUFFER_END) {
        SPIi_ReturnResult(command, SPI_PXI_RESULT_INVALID_PARAMETER);
        return;
      }
      address = ((nvramPxiWords[0] & 0xff) << 16) | nvramPxiWords[1];
      size = nvramPxiWords[2];
      break;
    case FLASH_REQ_CLEAR_PAGE:
    case FLASH_REQ_CLEAR_SECTOR:
      address = ((nvramPxiWords[0] & 0xff) << 16) | nvramPxiWords[1];
      break;
    case FLASH_REQ_SLEEP:
    case FLASH_REQ_WAKE:
    case FLASH_REQ_CLEAR_ALL:
      break;
  }

  if (!SPIi_SetEntry(SERIAL_OWNER_FLASH, command, 3, address, size, buffer)) {
    SPIi_ReturnResult(command, SPI_PXI_RESULT_EXCLUSIVE);
  }
}
