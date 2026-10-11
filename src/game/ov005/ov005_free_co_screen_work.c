#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba8.h"

void SpriteAnim_Free(void *handle);
void Mem_Free(void *block);

void Ov005_FreeCoScreenWork(Ov005Task *task)
{
    Unk_ov005_0236bba8 *work = task->work;
    int i;

    for (i = 0; i < 9; i++) {
        SpriteAnim_Free(work->unk_21c[i]);
    }
    Mem_Free(work);
}
