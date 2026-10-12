#include "types.h"
#include "game/task.h"

int FireTargets_GetCount(void); /* number of entries in the target list */
void func_020f5c58(Task* parent);
void Sound_PlaySe(u32 soundId);

/* Script step that starts the target list picker (TargetListPicker_Update): unk_64 is
 * the picked index, unk_66 the entry count. */
void TargetListPicker_Init(Task* task) {
  task->unk_64 = 0;
  task->unk_66 = FireTargets_GetCount();
  func_020f5c58(task);
  Sound_PlaySe(0x1000b);
}
