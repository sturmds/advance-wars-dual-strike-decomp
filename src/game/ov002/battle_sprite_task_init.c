#include "types.h"
#include "game/BattleSpriteTask.h"

void BattleSpriteTask_Init(BattleSpriteTask* task, const u32* table, int index,
                           int unk_3c, int unk_3e, int unk_40, int unk_48,
                           int unk_4a, int unk_4c, int unk_4e, int unk_50,
                           int unk_54, int unk_56, int unk_59, int unk_5a,
                           int unk_57, int unk_42, int unk_44, int unk_46,
                           int unk_58) {
  task->unk_2c = table[0];
  table += index;
  task->unk_30 = table[1];
  task->unk_3c = unk_3c;
  task->unk_3e = unk_3e;
  task->unk_40 = unk_40;
  task->unk_48 = unk_48;
  task->unk_4a = unk_4a;
  task->unk_4c = unk_4c;
  task->unk_4e = unk_4e;
  task->unk_50 = unk_50;
  task->unk_54 = unk_54;
  task->unk_56 = unk_56;
  task->unk_59 = unk_59;
  task->unk_5a = unk_5a;
  task->unk_5b = 0xff;
  task->unk_5c = 0;
  task->unk_5d = 1;
  task->unk_57 = unk_57;
  task->unk_42 = unk_42;
  task->unk_44 = unk_44;
  task->unk_46 = unk_46;
  task->unk_58 = unk_58;
}
