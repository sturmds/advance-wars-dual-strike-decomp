#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void Task_ResumeScript(Task* task);

#define FRAME(task) (*(s16*)&(task)->unk_4c)
#define FADE_FRAMES 16

/* Script task step: raises the master brightness of both screens by one
 * step per frame, from normal (0) to white (16), then resumes the script. */
void BothScreens_FadeToWhiteUpdate(Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  FRAME(task)++;
  savedGfx = data_027c0280;
  savedState = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->masterBrightness = FRAME(task);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->masterBrightness = FRAME(task);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (FRAME(task) == FADE_FRAMES) {
    Task_ResumeScript(task);
  }
}
