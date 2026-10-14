#include "types.h"
#include "game/task.h"

extern TaskScriptCmd gScreenShakeScript[];
extern u32 data_02157210[];
extern u32 data_021571fc[];
extern u8 data_021574c0[];
extern u8 data_02157364[];

static s16* data_021571ec[] = {(s16*)&data_02157210, (s16*)&data_021571fc,
                               (s16*)&data_021574c0, (s16*)&data_02157364};
u32 data_021571fc[] = {
#include "data/main/data_021571fc.inc.c"
};
u32 data_02157210[] = {
#include "data/main/data_02157210.inc.c"
};

/* Starts a screen shake task: tableIndex picks the offset table, steps is the
 * number of frames (-1: one pass of the table). Runs under parent if given. */
void ScreenShake_Start(int tableIndex, int steps, Task* parent) {
  Task* task;

  if (parent != NULL) {
    task = Task_CreateChild(gScreenShakeScript, parent);
  } else {
    task = Task_Create(gScreenShakeScript, TASK_LIST(1));
  }
  task->unk_4c = data_021571ec[tableIndex];
  task->unk_44 = steps;
}
