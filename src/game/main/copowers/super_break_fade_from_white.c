#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define FADE_FRAMES 32

/* Step of the Super CO Power cut-in script: over FADE_FRAMES frames the master
 * brightness eases from 16 (white) to 0, then the script resumes. */
void SuperBreak_FadeFromWhite(Task* task) {
  FRAME(task)++;
  data_027c0280->masterBrightness =
      Math_Ease(4, 16, 0, FRAME(task), FADE_FRAMES);
  if (FRAME(task) == FADE_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
