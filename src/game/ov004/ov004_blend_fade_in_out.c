#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Task_ResumeScript(void* task);

void Ov004_BlendFadeInOut(Ov004Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  int alpha;

  switch (task->step) {
    case 0: /* fade in over tweenFrames frames */
      task->frame = 0;
      task->duration = task->tweenFrames;
      task->step++;
    case 1:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      alpha = Math_Ease(0, 0, 16, task->frame, task->duration);
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
        task->step++;
      }
      break;
    case 2: /* hold for unk_48 frames */
      task->frame = 0;
      task->duration = task->unk_48;
      task->step++;
    case 3:
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        task->step++;
      }
      break;
    case 4: /* fade out over unk_4c frames */
      task->frame = 0;
      task->duration = task->unk_4c;
      task->step++;
    case 5:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      alpha = Math_Ease(0, 16, 0, task->frame, task->duration);
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
