#include "types.h"
#include "sdk/card_arm7.h"

/*
 * Clocks one byte in from the save chip and compares it with the next source
 * byte. On a difference it clears verifyOk and cuts the transaction short:
 * the chip select is released on the next byte.
 */
void CardSpi_ClockByteInAndCompare(CARDiSpiTransfer *transfer)
{
    IO_GAMECARD_SPI_BYTE = 0;
    while (IO_GAMECARD_SPI_CTRL & CARD_SPI_BUSY) {
    }
    /* the chip answers in the low byte of the data register */
    if ((u8)IO_GAMECARD_SPI_BYTE != *transfer->src) {
        transfer->verifyOk = FALSE;
        /* needed to match: the original compares remaining unsigned */
        if ((u32)transfer->remaining > 1) {
            transfer->remaining = 1;
        }
    }
    transfer->src++;
}
