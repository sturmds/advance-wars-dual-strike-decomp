#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/win_lose.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetCurrentBoth(void *state);
void Task_ResumeScript(WinLoseTask *task);
void Gfx_RequestLayerCopy(int layer);
void Gfx_ClearLayerMap(int layer);        /* likely clears the layer's tile map */

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Blend weights for the frames left in task->frame: from about 9/9 down
 * to 0/16, so layer 2 (blended over layer 3 since WinLoseScreen_Init)
 * fades out. */
static inline void SetFadeBlend(WinLoseTask *task)
{
    if (task->frame != 0) {
        data_027c0280->blendEva = Math_Ease(1, 0, 9, task->frame - 1, 64);
        data_027c0280->blendEvb = Math_Ease(1, 18, 9, task->frame - 1, 64);
        if (data_027c0280->blendEvb > 16) {
            data_027c0280->blendEva = 0;
            data_027c0280->blendEvb = 16;
        }
    }
}

/* Clears layer 2 and turns blending off. */
static inline void EndFade(void)
{
    Gfx_ClearLayerMap(2);
    Gfx_RequestLayerCopy(2);
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
}

/* Task update of the win/lose screen: counts task->frame down from 64
 * (set by the slide-out) and on both screens eases the alpha blend so
 * layer 2 fades out. At 0 it clears layer 2 on both screens, turns
 * blending off and resumes the task's script. */
void WinLoseScreen_FadeOutLayer2(WinLoseTask *task)
{
    Unk_027c0280 *savedGfx = data_027c0280;
    Unk_02080440 *saved = data_027c027c;

    Screen_SetCurrentBoth(data_027c0288.unk_00);
    SetFadeBlend(task);
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    SetFadeBlend(task);
    Screen_SetCurrent(saved);
    Screen_SetCurrentGfx(savedGfx);
    if (task->frame != 0) {
        task->frame--;
        return;
    }
    savedGfx = data_027c0280;
    saved = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    EndFade();
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    EndFade();
    Screen_SetCurrent(saved);
    Screen_SetCurrentGfx(savedGfx);
    task->frame = 0;
    Task_ResumeScript(task);
}
