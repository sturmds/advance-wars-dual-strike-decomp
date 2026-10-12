#include "types.h"
#include "game/BattleSpriteTask.h"

BattleSpriteTask* BattleSpriteTask_CreateDefault(
    const u32* table, int index, int unk_3c, int unk_3e, int unk_40, int unk_48,
    int unk_4a, int unk_4c, int unk_54, int unk_56, int unk_59, int unk_5a,
    int unused_0, int unused_1, int unused_2, int unused_3, int unused_4) {
  return BattleSpriteTask_Create(table, index, unk_3c, unk_3e, unk_40, unk_48,
                                 unk_4a, unk_4c, 0x100, 0x100, unk_54, unk_56,
                                 unk_59, unk_5a, 0, 0, 0, 0, unk_56);
}
