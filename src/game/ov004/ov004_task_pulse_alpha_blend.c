#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/math.h"

void Task_ResumeScript(Ov004Task* task);

void Ov004Task_PulseAlphaBlend(Ov004Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int blend;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->duration =
          20; /* alpha blend: first target weight 0 to 16, second 16 to 0 */
      task->step++;
    case 1:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedState = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      blend = Math_Ease(0, 0, 16, task->frame, task->duration);
      data_027c0280->blendMode = 1;
      data_027c0280->blendEva = blend;
      data_027c0280->blendEvb = 16 - blend;
      data_027c0280->blendEvy = 0;
      Screen_SetCurrent(savedState);
      Screen_SetCurrentGfx(savedGfx);
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        task->step++;
      }
      break;
    case 2: /* hold the full blend */
      task->frame = 0;
      task->duration = 22;
      task->step++;
    case 3:
      if (task->frame <= task->duration) {
        task->frame++;
      }
      if (task->frame > task->duration) {
        task->step++;
      }
      break;
    case 4:
      task->frame = 0;
      task->duration = 12;
      task->step++;
    case 5:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedState = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      blend = Math_Ease(0, 16, 0, task->frame, task->duration);
      data_027c0280->blendMode = 1;
      data_027c0280->blendEva = blend;
      data_027c0280->blendEvb = 16 - blend;
      data_027c0280->blendEvy = 0;
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
