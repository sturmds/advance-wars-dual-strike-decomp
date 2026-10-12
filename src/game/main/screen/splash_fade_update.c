#include "types.h"
#include "game/screen_other_map.h"
#include "game/splash_task.h"

void Screen_SetCurrentBoth(void* state);
void Task_ResumeScript(void* task);
void Screen_SetBothNibbles(int value);

/* Raises the master brightness of both screens by one level every 4 frames
 * and resumes the script after 64 frames. */
void Splash_FadeUpdate(SplashTask* task) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->masterBrightness = task->fadeFrames >> 2;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->masterBrightness = task->fadeFrames >> 2;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  task->fadeFrames++;
  if (task->fadeFrames >= 64) {
    Screen_SetBothNibbles(1);
    Task_ResumeScript(task);
  }
}
