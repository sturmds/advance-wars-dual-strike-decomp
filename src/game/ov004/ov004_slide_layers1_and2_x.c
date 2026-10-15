#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Task_ResumeScript(void* task);

void Ov004_SlideLayers1And2X(Ov004Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  int scrollA;
  int scrollB;

  switch (task->step) {
    case 0: /* falls through into the first frame */
      task->frame = 0;
      task->duration = 30;
      task->step++;
    case 1:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      scrollA = Math_Ease(0, -256, 512, task->frame, task->duration);
      scrollB = Math_Ease(0, -256, 512, task->frame, task->duration);
      if (scrollA < -256) {
        data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 256;
      } else if (scrollA > 256) {
        data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 256;
      } else {
        data_027c0280->bgScrollX[data_027c0280->layer1Bg] = scrollA;
      }
      if (scrollB < 0) {
        data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
      } else if (scrollB > 512) {
        data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
      } else {
        data_027c0280->bgScrollX[data_027c0280->layer2Bg] = scrollB;
      }
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
