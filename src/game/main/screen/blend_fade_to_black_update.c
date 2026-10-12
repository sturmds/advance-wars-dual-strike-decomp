#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/screen_effect.h"

void Task_Destroy(Task* task);

/* Task update: raises the darken level of the chosen screens by unk_64 per
 * frame, up to 16 << 8 (black). The task ends once the level is 16. */
void BlendFadeToBlack_Update(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (data_027c0280->blendEvy == 16) {
    Task_Destroy(task);
    return;
  }
  task->unk_66 += task->unk_64;
  if (task->unk_66 >= 16 << 8) {
    task->unk_66 = 16 << 8;
  }
  if (ScreenEffect_GetScreens() == EFFECT_SCREEN_BOTH ||
      ScreenEffect_GetScreens() == EFFECT_SCREEN_MAIN ||
      (data_027c027c == data_027c0288.unk_00 &&
       ScreenEffect_GetScreens() == EFFECT_SCREEN_CURRENT)) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    data_027c0280->blendEvy = task->unk_66 >> 8;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  if (ScreenEffect_GetScreens() == EFFECT_SCREEN_BOTH ||
      ScreenEffect_GetScreens() == EFFECT_SCREEN_SUB ||
      (data_027c027c == data_027c0288.unk_04 &&
       ScreenEffect_GetScreens() == EFFECT_SCREEN_CURRENT)) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    data_027c0280->blendEvy = task->unk_66 >> 8;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
}
