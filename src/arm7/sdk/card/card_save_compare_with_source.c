#include "types.h"
#include "sdk/card.h"

void func_arm7_038031ac(void);
void func_arm7_038032a4(u32 byteCount);
void CardSave_SendCommandAndAddress(u32 address, u32 command);
void CardSpi_ClockByteInAndCompare(CARDiSpiTransfer *transfer);
void CardSpi_MoveBytes(const u8 *src, u8 *dst, int count, CARDiSpiByteFunction transferByte);
void CardSave_WaitUntilIdle(int minWait, int maxWait);

/* Compares len bytes of the save chip at address with src; a difference sets CARD_RESULT_FAILURE. */
void CardSave_CompareWithSource(u32 address, const u8 *src, int len)
{
    func_arm7_038031ac();
    data_arm7_03809860.verifyOk = TRUE;
    func_arm7_038032a4(data_arm7_03809280.cmd->spec.addrWidth + 1 + len);
    CardSave_SendCommandAndAddress(address, SAVE_CHIP_OP_FETCH_BYTES);
    CardSpi_MoveBytes(src, NULL, len, CardSpi_ClockByteInAndCompare);
    CardSave_WaitUntilIdle(0, 0);
    if (data_arm7_03809280.cmd->result == CARD_RESULT_SUCCESS && !data_arm7_03809860.verifyOk) {
        data_arm7_03809280.cmd->result = CARD_RESULT_FAILURE;
    }
}
