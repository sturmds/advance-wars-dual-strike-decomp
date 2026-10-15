#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define EASE_FRAMES 10

/* Step of the tag break script: eases the blend weight blendEva from 16 to 0
 * over EASE_FRAMES frames, fading the blended layer out, then resumes the
 * script. */
void TagBreak_FadeOutBlend(Task* task) {
  FRAME(task)++;
  data_027c0280->blendEva = Math_Ease(1, 16, 0, FRAME(task), EASE_FRAMES);
  if (FRAME(task) == EASE_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
