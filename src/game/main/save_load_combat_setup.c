#include "types.h"
#include "game/save_progress.h"

extern Unk_0229e148 data_0229e148;

int Save_LoadRecord(int recordId, void *buffer); /* the definition takes only recordId; this call also passes r1 */
void func_02017ca0(int value);
void func_02017c88(int value);

/* Loads save record 0 into data_02291774 and restores the combat setup
 * (data_0229e148) from it. Then lowers the two PlayRecords values that
 * func_02017ca0 and func_02017c88 limit to the record's. Does nothing when
 * the load fails. */
void Save_LoadCombatSetup(void)
{
    SaveProgressBlocks *record = (SaveProgressBlocks *)&data_02291774;

    if (Save_LoadRecord(0, record)) {
        return;
    }
    data_0229e148 = record->unk_15a4;
    func_02017ca0(record->playRecords.points);
    func_02017c88(record->playRecords.unk_04);
}
