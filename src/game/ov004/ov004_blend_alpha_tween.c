#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(void* task);

void Ov004_BlendAlphaTween(Ov004Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  int alpha;

  switch (task->step) {
    case 0: /* falls through into the first frame */
      task->frame = 0;
      task->duration = task->tweenFrames;
      task->step++;
    case 1:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      alpha = Math_Ease(0, task->tweenFrom, task->tweenTo, task->frame,
                        task->duration);
      /* alpha blend: the eased value weights the first target, 16 minus it the second */
      data_027c0280->blendMode = 1;
      data_027c0280->blendEva = alpha;
      data_027c0280->blendEvb = 16 - alpha;
      data_027c0280->blendEvy = 0;
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
