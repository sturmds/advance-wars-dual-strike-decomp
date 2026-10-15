#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Task_ResumeScript(void* task);

void Ov004Task_EaseBg1ScrollY(Ov004Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int bg;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->duration = 106;
      task->step++;
    case 1:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      bg = data_027c0280->layer1Bg;
      data_027c0280->bgScrollY[bg] =
          Math_Ease(0, 0, 32, task->frame, task->duration);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        Task_ResumeScript(task);
      }
      break;
  }
}
