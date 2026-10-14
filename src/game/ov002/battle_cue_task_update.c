#include "types.h"
#include "game/battle_cue_task.h"
#include "game/task.h"

static u8 data_ov002_02369784[12] = {
#include "data/ov002/data_ov002_02369784.inc.c"
};

void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(Task* task);

/* Each frame: when the frame counter reaches the next length in the table, plays
 * the sound (if any), moves to the next length, and resumes the task script at
 * the end of the table (a zero length). */
void BattleCueTask_Update(BattleCueTask* task) {
  task->frame++;
  if (task->frame != data_ov002_02369784[task->cueIndex]) {
    return;
  }
  if (task->sound != 0xffff) {
    Sound_PlaySe(task->sound);
  }
  task->frame = 0;
  task->cueIndex++;
  if (data_ov002_02369784[task->cueIndex] == 0) {
    Task_ResumeScript((Task*)task);
  }
}
