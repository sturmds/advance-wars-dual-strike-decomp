#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/screen_effect.h"

BOOL func_02003d10(Task *task);
void Screen_ResetBlendAllTop(void);

/* Task step: darkens the screens func_02003d10 picks to black (blend level
 * 16) and sets up BlendFadeFromBlack_Update to lighten them by one level a
 * frame. */
void BlendFadeFromBlack_Start(Task *task)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    if (func_02003d10(task) == 0) {
        return;
    }
    if (func_020076f8() == EFFECT_SCREEN_BOTH || func_020076f8() == EFFECT_SCREEN_MAIN
        || (data_027c027c == data_027c0288.unk_00 && func_020076f8() == EFFECT_SCREEN_CURRENT)) {
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        Screen_ResetBlendAllTop();
        data_027c0280->blendMode = BLEND_MODE_DARKEN;
        data_027c0280->blendEvy = 16;
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }
    if (func_020076f8() == EFFECT_SCREEN_BOTH || func_020076f8() == EFFECT_SCREEN_SUB
        || (data_027c027c == data_027c0288.unk_04 && func_020076f8() == EFFECT_SCREEN_CURRENT)) {
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        Screen_ResetBlendAllTop();
        data_027c0280->blendMode = BLEND_MODE_DARKEN;
        data_027c0280->blendEvy = 16;
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }
    task->unk_64 = 1 << 8;
    task->unk_66 = 16 << 8;
    task->unk_68 = -1;
}
