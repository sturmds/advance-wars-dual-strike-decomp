#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/math.h"

void Task_ResumeScript(Ov004Task* task);

void Ov004Task_SlideLayers1And2ScrollX(Ov004Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int scrollOne;
  int scrollTwo;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->duration = 30;
      task->step++;
    case 1:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedState = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      scrollOne = Math_Ease(0, task->tweenFrom, task->tweenTo, task->frame,
                            task->duration);
      scrollTwo = Math_Ease(0, task->tweenFrom, task->tweenTo, task->frame,
                            task->duration);
      /* out-of-range values snap back to the layer's rest position */
      if (scrollOne < -256) {
        data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 256;
      } else if (scrollOne > 256) {
        data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 256;
      } else {
        data_027c0280->bgScrollX[data_027c0280->layer1Bg] = scrollOne;
      }
      if (scrollTwo < 0) {
        data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
      } else if (scrollTwo > 512) {
        data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
      } else {
        data_027c0280->bgScrollX[data_027c0280->layer2Bg] = scrollTwo;
      }
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
