#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Turns the blend off on both screens, leaving EVA at 16. */
static inline void Screens_EndBlend(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}

/* Task update: lowers the darken blend of both screens by 1 / 16 every
 * 4 frames. At 0 it turns the blend off and resumes the task's script. */
void ScreenDarkenIn_Update(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  FRAME(task)++;
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  if ((FRAME(task) & 3) == 0) {
    data_027c0280->blendEvy--;
  }
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  if ((FRAME(task) & 3) == 0) {
    data_027c0280->blendEvy--;
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  if (data_027c0280->blendEvy != 0) {
    return;
  }
  Screens_EndBlend();
  FRAME(task) = 0;
  Task_ResumeScript(task);
}
