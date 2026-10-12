#include "sdk/card.h"

/* Waits for the card to be idle, then sends an 8-byte command: high word first. */
void CardRom_LoadCommandBytes(u32 high, u32 low) {
  while (IO_GAMECARD_ROM_CTRL & CARD_START) {
  }
  IO_GAMECARD_MASTER = CARDMST_ENABLE | CARDMST_IRQ;
  IO_GAMECARD_CMD_BYTES[0] = high >> 24;
  IO_GAMECARD_CMD_BYTES[1] = high >> 16;
  IO_GAMECARD_CMD_BYTES[2] = high >> 8;
  IO_GAMECARD_CMD_BYTES[3] = high;
  IO_GAMECARD_CMD_BYTES[4] = low >> 24;
  IO_GAMECARD_CMD_BYTES[5] = low >> 16;
  IO_GAMECARD_CMD_BYTES[6] = low >> 8;
  IO_GAMECARD_CMD_BYTES[7] = low;
}
