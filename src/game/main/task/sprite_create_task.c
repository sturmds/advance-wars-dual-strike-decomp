#include "types.h"
#include "game/task.h"

void func_02027b38(Task *task, const void *script, int c, int x, int y, int e, int scaleX, int scaleY, int tile, int g);

extern TaskScriptCmd data_02159114[];

/* Creates a sprite task on top-level list `list` (scripts: data_02159114) and
 * sets it up with func_02027b38 at scale 256 x 256. Sets bit 2 of unk_5c. */
Task *Sprite_CreateTask(int list, const void *script, int c, int x, s16 y, s16 e, u16 tile, u8 g)
{
    Task *task = Task_Create(data_02159114, (void *)list);

    func_02027b38(task, script, c, x, y, e, 256, 256, tile, g);
    task->unk_5c |= 4;
    return task;
}
