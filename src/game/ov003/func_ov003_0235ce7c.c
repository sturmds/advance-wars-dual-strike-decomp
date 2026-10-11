#include "types.h"
#include "game/Unk_ov003_0235dd38.h"
#include "game/ov003_unit.h"

void Sprite_QueueCoPortrait(int x, int y, int arg2, int arg3, int list, BOOL flip);
s16 BreakText_GetLetterX(int index);
void BreakText_QueueLetterSized(u8 index, u16 value, int arg2, int arg3, int arg4);
void Task_ResumeScript(void *task);

/* Task step: while Ov003State.unk_1d7 is 0, counts frames. Each call queues a
 * sprite and then one entry per filled slot; after (16 - count) * 6 frames it
 * resumes the task script. The (u8) cast is needed to match. */
void func_ov003_0235ce7c(Unk_ov003_0235dd38 *obj)
{
    int i;

    if (data_ov003_02364928->unk_1d7 == 0) {
        obj->frame++;
    }
    Sprite_QueueCoPortrait(184, 160, 0x3000, 0x58e, 3, 0);
    for (i = 0; i < obj->count; i++) {
        BreakText_QueueLetterSized(i, BreakText_GetLetterX((u8)i) & 0x1ff, 80, 0, 2);
    }
    if (obj->frame == (16 - obj->count) * 6) {
        obj->frame = 0;
        Task_ResumeScript(obj);
    }
}
