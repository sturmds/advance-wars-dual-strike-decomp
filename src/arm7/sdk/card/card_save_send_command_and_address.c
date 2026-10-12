#include "types.h"
#include "sdk/card_arm7.h"

void CardSpi_ClockByteOut(CARDiSpiTransfer* transfer);
void CardSpi_MoveBytes(const u8* src, u8* dst, int count,
                       CARDiSpiByteFunction transferByte);

/*
 * Sends a save chip command byte followed by address, most significant byte
 * first, in as many bytes as the chip's addresses have. A chip with 1-byte
 * addresses takes address bit 8 in bit 3 of the command.
 */
void CardSave_SendCommandAndAddress(u32 address, u32 command) {
  u32 packet;
  u32 addrWidth = data_arm7_03809280.cmd->spec.addrWidth;

  switch (addrWidth) {
    case 1:
      packet = command | ((address >> 5) & 0x08) | ((address & 0xff) << 8);
      break;
    case 2:
      packet = command | (address & 0xff00) | ((address & 0xff) << 16);
      break;
    case 3:
      packet = command | ((address >> 8) & 0xff00) | ((address & 0xff00) << 8) |
               ((address & 0xff) << 24);
      break;
  }
  CardSpi_MoveBytes((u8*)&packet, NULL, addrWidth + 1, CardSpi_ClockByteOut);
}
