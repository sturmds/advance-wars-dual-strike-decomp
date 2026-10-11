#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/screen_effect.h"

BOOL func_02003d84(Task *task);
void func_020075c0(void);

/* Task step: turns on the darken blend at level 0 on the screens
 * func_02003d84 picks and sets up BlendFadeToBlack_Update to darken them by
 * one level a frame. */
void BlendFadeToBlack_Start(Task *task)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    if (func_02003d84(task) == 0) {
        return;
    }
    if (func_020076f8() == EFFECT_SCREEN_BOTH || func_020076f8() == EFFECT_SCREEN_MAIN
        || (data_027c027c == data_027c0288.unk_00 && func_020076f8() == EFFECT_SCREEN_CURRENT)) {
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        func_020075c0();
        data_027c0280->blendMode = BLEND_MODE_DARKEN;
        data_027c0280->blendEvy = 0;
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }
    if (func_020076f8() == EFFECT_SCREEN_BOTH || func_020076f8() == EFFECT_SCREEN_SUB
        || (data_027c027c == data_027c0288.unk_04 && func_020076f8() == EFFECT_SCREEN_CURRENT)) {
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        func_020075c0();
        data_027c0280->blendMode = BLEND_MODE_DARKEN;
        data_027c0280->blendEvy = 0;
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }
    task->unk_64 = 1 << 8;
    task->unk_66 = 0;
    task->unk_68 = -1;
}
