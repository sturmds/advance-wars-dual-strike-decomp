#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb80.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void SpriteAnim_Free(u32 *handle);
void Task_Destroy(Task *task);
void Mem_Free(void *block);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Clears the sprite handles, destroys the two tasks (on the second screen's
 * state) and frees block80. The work block itself is freed by a later step. */
void Ov005Ripple_ReleaseObjects(Ov005Task *task)
{
    Unk_ov005_0236bb80 *work = task->work;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    SpriteAnim_Free(work->spriteHandles[0]);
    SpriteAnim_Free(work->spriteHandles[1]);
    SpriteAnim_Free(work->spriteHandles[2]);
    SpriteAnim_Free(work->spriteHandles[3]);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    if (work->hasTaskA != 0) {
        Task_Destroy(work->taskA);
    }
    if (work->hasTaskB != 0) {
        Task_Destroy(work->taskB);
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    Mem_Free(work->block80);
}
