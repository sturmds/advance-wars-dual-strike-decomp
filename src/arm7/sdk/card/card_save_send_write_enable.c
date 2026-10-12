#include "types.h"
#include "sdk/card_arm7.h"

extern const u8 data_arm7_038072a8;   /* save chip command 6, write enable */

void func_arm7_038032a4(u32 byteCount);
void CardSpi_ClockByteOut(CARDiSpiTransfer *transfer);
void CardSpi_MoveBytes(const u8 *src, u8 *dst, int count, CARDiSpiByteFunction transferByte);
void CardSave_WaitUntilIdle(int minWait, int maxWait);

/* Sends the write enable command, which the save chip needs before each write or erase. */
void CardSave_SendWriteEnable(void)
{
    func_arm7_038032a4(1);
    CardSpi_MoveBytes(&data_arm7_038072a8, NULL, 1, CardSpi_ClockByteOut);
    CardSave_WaitUntilIdle(0, 0);
}
