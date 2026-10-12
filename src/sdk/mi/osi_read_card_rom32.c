#include "types.h"
#include "sdk/card.h"
#include "sdk/rom_header.h"

/*
 * Reads size bytes from the cartridge at romOffset into dest, a 512-byte
 * page at a time. The reads start at the page holding romOffset; words
 * before romOffset or past size are dropped.
 */
void OSi_ReadCardRom32(u32 romOffset, void* dest, s32 size) {
  u32 control = (ROM_HEADER_BUF->cardControl & ~CARD_COMMAND_MASK) |
                (CARD_START | CARD_RESET_HI | GAMECARD_XFER_ONE_PAGE);
  s32 pos = -(s32)(romOffset & (GAMECARD_ROM_PAGE_BYTES - 1));

  while (IO_GAMECARD_ROM_CTRL & CARD_START) {
  }
  IO_GAMECARD_MASTER = CARDMST_ENABLE;

  for (romOffset += pos; pos < size; romOffset += GAMECARD_ROM_PAGE_BYTES) {
    IO_GAMECARD_CMD_BYTES[0] = CARD_CMD_READ_PAGE;
    IO_GAMECARD_CMD_BYTES[1] = romOffset >> 24;
    IO_GAMECARD_CMD_BYTES[2] = romOffset >> 16;
    IO_GAMECARD_CMD_BYTES[3] = romOffset >> 8;
    IO_GAMECARD_CMD_BYTES[4] = romOffset;
    IO_GAMECARD_CMD_BYTES[5] = 0;
    IO_GAMECARD_CMD_BYTES[6] = 0;
    IO_GAMECARD_CMD_BYTES[7] = 0;
    IO_GAMECARD_ROM_CTRL = control;
    for (;;) {
      u32 status = IO_GAMECARD_ROM_CTRL;

      if (status & CARD_DATA_READY) {
        u32 data = IO_GAMECARD_READ_PORT;

        if (pos >= 0 && pos < size) {
          /* needed to match: a u8 * sum becomes a walking pointer */
          *(u32*)((u32)dest + pos) = data;
        }
        pos += 4;
      }
      if (!(status & CARD_START)) {
        break;
      }
    }
  }
}
