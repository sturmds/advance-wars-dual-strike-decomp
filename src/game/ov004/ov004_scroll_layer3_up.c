#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"

void Task_ResumeScript(void* task);

void Ov004_ScrollLayer3Up(Ov004Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  switch (task->step) {
    case 0: /* falls through into the first frame */
      task->frame = 0;
      task->duration = 80;
      task->step++;
    case 1:
      /* 32 pixels up each frame on the first screen, until the duration has passed */
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      data_027c0280->bgScrollY[data_027c0280->layer3Bg] -= 32;
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
