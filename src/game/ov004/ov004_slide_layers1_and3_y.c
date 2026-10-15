#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Task_ResumeScript(void* task);

void Ov004_SlideLayers1And3Y(Ov004Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  int scroll;

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
      /* layer 1 scrolls 256 pixels further than layer 2 */
      scroll = Math_Ease(0, task->tweenFrom, task->tweenTo, task->frame,
                         task->duration);
      data_027c0280->bgScrollY[data_027c0280->layer1Bg] = scroll + 256;
      data_027c0280->bgScrollY[data_027c0280->layer3Bg] = scroll;
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
