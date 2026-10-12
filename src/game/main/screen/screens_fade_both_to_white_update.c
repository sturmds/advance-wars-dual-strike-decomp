#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define FADE_FRAMES 32

/* Task update: raises the master brightness of both screens from normal
 * to white (16) over 32 frames, then resumes the task's script. */
void ScreensFadeBothToWhite_Update(Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  FRAME(task)++;
  savedGfx = data_027c0280;
  savedState = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->masterBrightness =
      Math_Ease(4, 0, 16, FRAME(task), FADE_FRAMES);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->masterBrightness =
      Math_Ease(4, 0, 16, FRAME(task), FADE_FRAMES);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (FRAME(task) == FADE_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
