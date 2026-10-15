#include "types.h"
#include "game/Unk_02080440.h"

void BattleSave_ClearRecordFlag(int recordId);
void func_02019920(void);
int Save_DeleteRecord(int recordId);
void* PlayRecords_TakeSnapshot(void);

/* Deletes battle save record recordId (a guess from the callee): clears its marker byte, then
 * Save_DeleteRecord runs for the record. In game mode 0 it also calls
 * PlayRecords_TakeSnapshot, which copies two words of gPlayRecords. Record 0 does
 * nothing. */
void BattleSave_Delete(int recordId) {
  if (recordId == 0) {
    return;
  }
  BattleSave_ClearRecordFlag(recordId);
  func_02019920();
  Save_DeleteRecord(recordId);
  if (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN) {
    PlayRecords_TakeSnapshot();
  }
}
