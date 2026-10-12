#include "types.h"
#include "sdk/card.h"

void func_arm7_038031ac(void);
void func_arm7_038032a4(u32 byteCount);
void CardSave_SendCommandAndAddress(u32 address, u32 command);
void CardSpi_ClockByteIn(CARDiSpiTransfer *transfer);
void CardSpi_MoveBytes(const u8 *src, u8 *dst, int count, CARDiSpiByteFunction transferByte);
void CardSave_WaitUntilIdle(int minWait, int maxWait);

/* Reads len bytes of the save chip at address into dst. */
void CARDi_ReadBackup(u32 address, u8 *dst, int len)
{
    func_arm7_038031ac();
    func_arm7_038032a4(data_arm7_03809280.cmd->spec.addrWidth + 1 + len);
    CardSave_SendCommandAndAddress(address, SAVE_CHIP_OP_FETCH_BYTES);
    CardSpi_MoveBytes(NULL, dst, len, CardSpi_ClockByteIn);
    CardSave_WaitUntilIdle(0, 0);
}
