#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/win_lose.h"
#include "game/math.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void Task_ResumeScript(WinLoseTask* task);

#define BLEND_FRAMES 64

/* Task update of the win/lose screen, 64 frames: eases the alpha blend of
 * both screens, first target from 2 to 9 and second target from 16 to 9
 * (frame 1 only sets the first to 1), then resumes the script. */
void WinLoseScreen_BlendIn(WinLoseTask* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  task->frame++;
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  if (task->frame == 1) {
    data_027c0280->blendEva = 1;
  } else {
    data_027c0280->blendEva = Math_Ease(4, 2, 9, task->frame, BLEND_FRAMES);
    data_027c0280->blendEvb = Math_Ease(4, 16, 9, task->frame, BLEND_FRAMES);
  }
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  if (task->frame == 1) {
    data_027c0280->blendEva = 1;
  } else {
    data_027c0280->blendEva = Math_Ease(4, 2, 9, task->frame, BLEND_FRAMES);
    data_027c0280->blendEvb = Math_Ease(4, 16, 9, task->frame, BLEND_FRAMES);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  if (task->frame != BLEND_FRAMES) {
    return;
  }
  task->frame = 0;
  Task_ResumeScript(task);
}
