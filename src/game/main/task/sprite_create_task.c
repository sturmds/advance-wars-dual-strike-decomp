#include "types.h"
#include "game/task.h"

void TableSpriteTask_Init(Task* task, const u32* table, int index, int x, s16 y,
                          s16 e, s16 scaleX, s16 scaleY, u16 tile, u8 g);

extern TaskScriptCmd data_02159114[];

/* Creates a sprite task on top-level list `list` (scripts: data_02159114) and
 * sets it up with TableSpriteTask_Init at scale 256 x 256. Sets bit 2 of unk_5c. */
Task* Sprite_CreateTask(int list, const void* script, int c, int x, s16 y,
                        s16 e, u16 tile, u8 g) {
  Task* task = Task_Create(data_02159114, (void*)list);

  TableSpriteTask_Init(task, script, c, x, y, e, 256, 256, tile, g);
  task->unk_5c |= 4;
  return task;
}
