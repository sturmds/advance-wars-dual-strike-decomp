#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/math.h"

#define FADE_FRAMES 16

void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Task update: lowers the master brightness from white (16) to normal over
 * 16 frames, then resumes the task's script. */
void ScreenFadeInFromWhite_Update(Task* task) {
  FRAME(task)++;
  data_027c0280->masterBrightness =
      Math_Ease(4, 16, 0, FRAME(task), FADE_FRAMES);
  if (FRAME(task) == FADE_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
