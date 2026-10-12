#include "types.h"
#include "sdk/card.h"

/* Clocks one byte in from the save chip. */
void CardSpi_ClockByteIn(CARDiSpiTransfer *transfer)
{
    IO_GAMECARD_SPI_BYTE = 0;
    while (IO_GAMECARD_SPI_CTRL & CARD_SPI_BUSY) {
    }
    *transfer->dst = IO_GAMECARD_SPI_BYTE;
    transfer->dst++;
}
