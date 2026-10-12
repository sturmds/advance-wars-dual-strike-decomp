#include "types.h"
#include "sdk/card.h"

void func_arm7_038031ac(void);
void func_arm7_038032a4(u32 byteCount);
void CardSave_SendCommandAndAddress(u32 address, u32 command);
void func_arm7_03802fe0(void);
void CardSave_WaitUntilIdle(int minWait, int maxWait);

/* Erases the save chip sector that holds address. */
void CardSave_EraseSectorAt(u32 address)
{
    func_arm7_038031ac();
    func_arm7_03802fe0();
    func_arm7_038032a4(data_arm7_03809280.cmd->spec.addrWidth + 1);
    CardSave_SendCommandAndAddress(address, SAVE_CHIP_OP_WIPE_SECTOR);
    CardSave_WaitUntilIdle(data_arm7_03809280.cmd->spec.unk_20, 0);
}
