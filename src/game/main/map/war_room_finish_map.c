#include "types.h"
#include "game/Unk_02080440.h"

int BattleSave_GetRecordId(int mode, int campaignSlot);
void BattleRecordTask_Start(int recordId, void (*callback)(void));
void func_020d60a8(void);

/* End of a War Room map: starts the BattleRecordTask_Start task for this mode's record.
 * func_020d60a8 (loads the saved progress) runs as its callback. */
void WarRoom_FinishMap(void) {
  BattleRecordTask_Start(
      BattleSave_GetRecordId(data_027c027c->unk_48[data_027c0284]->gameMode, 0),
      func_020d60a8);
}
