#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

int func_02007760(void);
void Screen_SetNibble(int value);
int func_02007710(int screens);
void func_020075c0(void);
void Task_Destroy(Task *task);

/* Script step: like BlendFadeFromBlack_Start, but for the current screen
 * only. When the screen is hidden, marks it shown, darkens it fully (16 / 16)
 * and sets up ScreenBlendLighten_Update to lighten it by 1/16 a frame.
 * Otherwise there is nothing to fade in and the task ends. */
void ScreenBlendLighten_Start(Task *task)
{
    if (func_02007760() == 0) {
        Task_Destroy(task);
        return;
    }
    Screen_SetNibble(0);
    func_02007710(0);
    func_020075c0();
    data_027c0280->blendMode = BLEND_MODE_DARKEN;
    data_027c0280->blendEvy = 16;
    task->unk_64 = 256;
    task->unk_66 = 4096;
}
