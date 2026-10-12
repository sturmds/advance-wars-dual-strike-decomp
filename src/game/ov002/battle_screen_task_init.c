#include "types.h"
#include "game/BattleScreenTask.h"
#include "game/Unk_0229fb30.h"

/* Start of the battle screen task: clears its frame counter, data_0229fb30.unk_10 and the
 * per-side bytes. */
void BattleScreenTask_Init(BattleScreenTask* task) {
  int i;

  task->frame = 0;
  data_0229fb30.unk_10 = 0;
  for (i = 0; i < 2; i++) {
    task->fighterStarted[i] = 0;
    task->unk_2e[i] = 0;
  }
}
