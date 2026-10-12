#include "types.h"
#include "sdk/card_arm7.h"

/* Clocks one byte out to the save chip. */
void CardSpi_ClockByteOut(CARDiSpiTransfer* transfer) {
  volatile u16 received;

  IO_GAMECARD_SPI_BYTE = *transfer->src;
  transfer->src++;
  while (IO_GAMECARD_SPI_CTRL & CARD_SPI_BUSY) {
  }
  received = IO_GAMECARD_SPI_BYTE;
}
