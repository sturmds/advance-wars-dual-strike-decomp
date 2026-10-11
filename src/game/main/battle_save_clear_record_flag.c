#include "types.h"

void func_0201939c(int recordId, int value);

/* Sets the byte that marks battle save record recordId as holding a battle
 * (BattleSave_Save sets it to 1) back to 0. Record 0 does nothing. */
void BattleSave_ClearRecordFlag(int recordId)
{
    if (recordId == 0) {
        return;
    }
    func_0201939c(recordId, 0);
}
