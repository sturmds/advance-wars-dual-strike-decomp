#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/screen_wipe.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task *task);
void Screen_EnableEmptyWindows(void);
void Screen_ResetWindowRects(void);
void func_02006098(void);

#define WIPE_END 352

/* Task update: adds the task's speed (unk_64) to the wipe progress and eases
 * data_0216e29c from 0 up to 352 (curve 0 for speeds up to 8, else curve 4).
 * Each frame func_02006098 updates window 1's rows on the wipe's screens. At
 * 352 it empties the window insides, resets the window rectangles and
 * resumes the task's script. */
void ScreenWipe_EaseUpUpdate(Task *task)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    /* The original reads the speed signed (ldrsh). */
    data_0216e2a8 += (s16)task->unk_64;
    if (data_0216e2a8 >= WIPE_END) {
        data_0216e2a8 = WIPE_END;
    }
    if ((s16)task->unk_64 <= 8) {
        data_0216e29c = Math_Ease(0, 0, WIPE_END, data_0216e2a8, WIPE_END);
    } else {
        data_0216e29c = Math_Ease(4, 0, WIPE_END, data_0216e2a8, WIPE_END);
    }

    if (ScreenEffect_GetScreens() == WIPE_BOTH || ScreenEffect_GetScreens() == WIPE_SCREEN0
        || (data_027c027c == data_027c0288.unk_00 && ScreenEffect_GetScreens() == WIPE_CURRENT)) {
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        func_02006098();
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }
    if (ScreenEffect_GetScreens() == WIPE_BOTH || ScreenEffect_GetScreens() == WIPE_SCREEN1
        || (data_027c027c == data_027c0288.unk_04 && ScreenEffect_GetScreens() == WIPE_CURRENT)) {
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        func_02006098();
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }

    if (data_0216e29c >= WIPE_END) {
        data_0216e29c = WIPE_END;
        if (ScreenEffect_GetScreens() == WIPE_BOTH || ScreenEffect_GetScreens() == WIPE_SCREEN0
            || (data_027c027c == data_027c0288.unk_00 && ScreenEffect_GetScreens() == WIPE_CURRENT)) {
            saved0280 = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_00);
            saved027c = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_00);
            Screen_EnableEmptyWindows();
            Screen_ResetWindowRects();
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);
        }
        if (ScreenEffect_GetScreens() == WIPE_BOTH || ScreenEffect_GetScreens() == WIPE_SCREEN1
            || (data_027c027c == data_027c0288.unk_04 && ScreenEffect_GetScreens() == WIPE_CURRENT)) {
            saved0280 = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_04);
            saved027c = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_04);
            Screen_EnableEmptyWindows();
            Screen_ResetWindowRects();
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);
        }
        Task_ResumeScript(task);
        return;
    }
    /* A second clamp that cannot fire here. Needed to match: the volatile
     * read makes the compiler load the value again, as the original does. */
    if (*(volatile s16 *)&data_0216e29c >= WIPE_END) {
        data_0216e29c = WIPE_END;
    }
}
