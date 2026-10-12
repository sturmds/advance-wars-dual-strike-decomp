#include "types.h"
#include "sdk/card.h"

extern const u8 data_arm7_038072ac;

void func_arm7_038032a4(u32 byteCount);
void CardSpi_ClockByteOut(CARDiSpiTransfer *transfer);
void CardSpi_ClockByteIn(CARDiSpiTransfer *transfer);
void CardSpi_MoveBytes(const u8 *src, u8 *dst, int count, CARDiSpiByteFunction transferByte);
void CardSave_WaitUntilIdle(int minWait, int maxWait);

#define SAVE_CHIP_BUSY_WRITING 0x01

/* Reads the save chip status register (command 5). Returns whether its write-in-progress bit is clear. */
int CardSave_IsChipIdle(void)
{
    u8 status;

    func_arm7_038032a4(2);
    CardSpi_MoveBytes(&data_arm7_038072ac, NULL, 1, CardSpi_ClockByteOut);
    CardSpi_MoveBytes(NULL, &status, 1, CardSpi_ClockByteIn);
    CardSave_WaitUntilIdle(0, 0);
    return (status & SAVE_CHIP_BUSY_WRITING) == 0;
}
