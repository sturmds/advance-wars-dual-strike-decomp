#include "types.h"
#include "game/task.h"

extern TaskScriptCmd gBattleRecordTaskScript[];

/* Starts the task that runs gBattleRecordTaskScript in task list 3. The record id goes
 * in Task.unk_64 and the callback in Task.unk_4c; the script ends by calling
 * the callback (BattleRecordTask_RunCallback). The "battle record" name is
 * a guess from the record id callers pass. */
void BattleRecordTask_Start(int recordId, void* callback) {
  Task* task = Task_Create(gBattleRecordTaskScript, TASK_LIST(3));

  task->unk_64 = recordId;
  task->unk_4c = callback;
}
