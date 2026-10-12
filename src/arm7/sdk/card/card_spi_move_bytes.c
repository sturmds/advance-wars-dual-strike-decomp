#include "types.h"
#include "sdk/card_arm7.h"

/* Moves count bytes with transferByte. The chip select stays held until the last byte of the whole transaction, so a command and its answer take two calls. */
void CardSpi_MoveBytes(const u8 *src, u8 *dst, int count, CARDiSpiByteFunction transferByte)
{
    CARDiSpiTransfer *transfer = &data_arm7_03809860;

    transfer->src = src;
    transfer->dst = dst;
    IO_GAMECARD_SPI_CTRL = CARD_SPI_ENABLE | SAVE_SPI_SELECT_PORT | CARD_SPI_HOLD;
    while (count != 0) {
        transfer->remaining--;
        if (transfer->remaining == 0) {
            IO_GAMECARD_SPI_CTRL = CARD_SPI_ENABLE | SAVE_SPI_SELECT_PORT;
        }
        while (IO_GAMECARD_SPI_CTRL & CARD_SPI_BUSY) {
        }
        transferByte(transfer);
        count--;
    }
    if (transfer->remaining == 0) {
        IO_GAMECARD_SPI_CTRL = 0;
    }
}
