#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/math.h"

void Task_ResumeScript(Ov004Task* task);

/* Script step: scrolls layers 1 and 2 of both screens sideways for 720
 * frames, layer 1 one pixel every 6 frames and layer 2 one every 12. */
void Ov004Task_ScrollLayers1And2Parallax(Ov004Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int farScroll;
  int nearScroll;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->duration = 720;
      task->tweenFrames = 0; /* used here as the scroll counter */
      task->step++;
    case 1:
      farScroll = HwDiv32(task->tweenFrames, 12);
      nearScroll = HwDiv32(task->tweenFrames, 6);
      task->tweenFrames++;

      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedState = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      data_027c0280->bgScrollX[data_027c0280->layer1Bg] = nearScroll;
      data_027c0280->bgScrollX[data_027c0280->layer2Bg] = farScroll;
      Screen_SetCurrent(savedState);
      Screen_SetCurrentGfx(savedGfx);

      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedState = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->bgScrollX[data_027c0280->layer1Bg] = nearScroll;
      data_027c0280->bgScrollX[data_027c0280->layer2Bg] = farScroll;
      Screen_SetCurrent(savedState);
      Screen_SetCurrentGfx(savedGfx);

      if (task->frame < task->duration) {
        task->frame++;
        return;
      }
      Task_ResumeScript(task);
      break;
  }
}
