#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"

void Task_ResumeScript(Ov004Task* task);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);

void Ov004Task_SlideBgScrollX(Ov004Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  u8 layer;

  switch (task->step) {
    case 0: /* start the slide */
      task->frame = 0;
      task->duration = task->tweenFrames;
      task->step++;
    case 1: /* runs from step 0 too: the first frame is drawn at once */
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedState = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      layer = data_027c0280->layer3Bg;
      data_027c0280->bgScrollX[layer] = Math_Ease(
          0, task->tweenFrom, task->tweenTo, task->frame, task->duration);
      Screen_SetCurrent(savedState);
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
