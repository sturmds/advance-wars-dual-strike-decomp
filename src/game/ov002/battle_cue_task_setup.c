#include "types.h"
#include "game/battle_cue_task.h"

/* Picks the two values for the sequence kind (1 to 3) and restarts the cue counters. */
void BattleCueTask_Setup(BattleCueTask* task) {
  switch (task->kind) {
    case 1:
      task->unk_64 = 2;
      task->unk_52 = 2;
      break;
    case 2:
      task->unk_64 = 3;
      task->unk_52 = 4;
      break;
    case 3:
      task->unk_64 = 4;
      task->unk_52 = 6;
      break;
  }
  task->frame = 0;
  task->cueIndex = 0;
  task->sound = 0;
}
