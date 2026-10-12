#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

#define FADE_FRAMES 32

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Task update: lowers the master brightness from normal to black (-16) over
 * 32 frames, then resumes the task's script. */
void ScreenFadeToBlackSlow_Update(Task* task) {
  FRAME(task)++;
  data_027c0280->masterBrightness =
      Math_Ease(4, 0, -16, FRAME(task), FADE_FRAMES);
  if (FRAME(task) == FADE_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
