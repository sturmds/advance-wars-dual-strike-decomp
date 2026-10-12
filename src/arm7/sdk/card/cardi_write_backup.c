#include "types.h"
#include "sdk/card_arm7.h"

void func_arm7_038031ac(void);
void func_arm7_038032a4(u32 byteCount);
void CardSave_SendCommandAndAddress(u32 address, u32 command);
void CardSave_SendWriteEnable(void);
void CardSpi_ClockByteOut(CARDiSpiTransfer *transfer);
void CardSpi_MoveBytes(const u8 *src, u8 *dst, int count, CARDiSpiByteFunction transferByte);
void CardSave_WaitUntilIdle(int minWait, int maxWait);

/*
 * Writes len bytes from src to the save chip at address with the page write
 * command (erase and write), one page at a time. Stops at the first error.
 */
void CARDi_WriteBackup(u32 address, const u8 *src, u32 len)
{
    u32 pageSize = data_arm7_03809280.cmd->spec.pageSize;

    func_arm7_038031ac();
    while (len != 0) {
        u32 count = pageSize - (address & (pageSize - 1));

        if (count > len) {
            count = len;
        }
        CardSave_SendWriteEnable();
        func_arm7_038032a4(data_arm7_03809280.cmd->spec.addrWidth + 1 + count);
        CardSave_SendCommandAndAddress(address, SAVE_CHIP_OP_ERASE_AND_WRITE);
        CardSpi_MoveBytes(src, NULL, count, CardSpi_ClockByteOut);
        CardSave_WaitUntilIdle(data_arm7_03809280.cmd->spec.unk_14, data_arm7_03809280.cmd->spec.unk_18);
        if (data_arm7_03809280.cmd->result != CARD_RESULT_SUCCESS) {
            break;
        }
        src += count;
        address += count;
        len -= count;
    }
}
