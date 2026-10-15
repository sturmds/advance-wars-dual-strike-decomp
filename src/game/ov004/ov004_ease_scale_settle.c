#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Task_ResumeScript(void* task);

extern Unk_ov004_022bf158* data_ov004_022bf158;

void Ov004_EaseScaleSettle(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  int scale;

  switch (task->step) {
    case 0: /* falls through into the first frame */
      task->frame = 0;
      task->duration = 16;
      task->step++;
    case 1:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      scale = Math_Ease(0, 336, 256, task->frame, task->duration);
      Ov004_SetBg3Scale(state, scale, scale);
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        Task_ResumeScript(task);
      }
      break;
  }
}
