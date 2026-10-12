#include "types.h"
#include "game/task.h"
#include "game/BattleSpriteTask.h"

extern const TaskScriptCmd data_ov002_023695e0[];

BattleSpriteTask* BattleSpriteTask_Create(const u32* table, int index,
                                          int unk_3c, int unk_3e, int unk_40,
                                          int unk_48, int unk_4a, int unk_4c,
                                          int unk_4e, int unk_50, int unk_54,
                                          int unk_56, int unk_59, int unk_5a,
                                          int unk_57, int unk_42, int unk_44,
                                          int unk_46, int unk_58) {
  BattleSpriteTask* task = (BattleSpriteTask*)Task_Create(
      data_ov002_023695e0, TASK_LIST(unk_5a == 0 ? 4 : 5));

  BattleSpriteTask_Init(task, table, index, unk_3c, unk_3e, unk_40, unk_48,
                        unk_4a, unk_4c, unk_4e, unk_50, unk_54, unk_56, unk_59,
                        unk_5a, unk_57, unk_42, unk_44, unk_46, unk_58);
  return task;
}
