#include "types.h"
#include "game/task.h"

/* Script step of the task BattleRecordTask_Start makes: calls the callback
 * it stored in unk_4c, if there is one. */
void BattleRecordTask_RunCallback(Task* task) {
  void (*callback)(void) = task->unk_4c;

  if (callback != NULL) {
    callback();
  }
}
