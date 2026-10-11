#include "types.h"
#include "game/Unk_021694c0.h"
#include "game/unit_show_task.h"

extern Unk_021694c0 *data_021694c0;
extern u8 data_0213b8f0[];

ShowSprite *func_02022478(void *file, int a, int b);
void func_02021f44(ShowSprite *sprite, int frame);

void func_020f1ed0(Unk_021694c0Task *task)
{
    task->sprite = func_02022478(data_0213b8f0, 0, 2);
    func_02021f44(task->sprite, 4);
    task->sprite->unk_24 = 0x4170;
    task->frame = 0;
    task->x = data_021694c0->unk_24;
    task->y = 192;
}
