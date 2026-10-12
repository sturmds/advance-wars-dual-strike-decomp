#include "types.h"
#include "sdk/pm.h"
#include "sdk/spi.h"
#include "sdk/pxi.h"

/* Sends a one-word reply to the ARM9 on the tag of the device the request
 * id belongs to, retrying while the FIFO is full. */
void SPIi_ReturnResult(u16 command, u16 result) {
  u32 tag;
  u32 data;

  switch (command & SERIAL_REQ_GROUP_BITS) {
    case SERIAL_REQ_GROUP_TOUCH:
    case SERIAL_REQ_GROUP_TOUCH + 0x10:
      tag = FIFO_TAG_TOUCH;
      break;
    case SERIAL_REQ_GROUP_MIC:
    case SERIAL_REQ_GROUP_MIC + 0x10:
      tag = PXI_FIFO_TAG_MIC;
      break;
    case SERIAL_REQ_GROUP_POWER:
    case SERIAL_REQ_GROUP_POWER + 0x10:
      tag = PXI_FIFO_TAG_PM;
      break;
    case SERIAL_REQ_GROUP_FLASH:
    case SERIAL_REQ_GROUP_FLASH + 0x10:
      tag = PXI_FIFO_TAG_NVRAM;
      break;
  }

  data = SERIAL_MSG_FIRST_WORD | SPI_PXI_END_BIT |
         (((command & 0xff) | SERIAL_MSG_ANSWER_BIT) << 8) | (result & 0xff);
  while (Fifo_PushWord_Arm7(tag, data, FALSE) < 0) {
  }
}
