#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define EASE_FRAMES 32

/* Step of the Super CO Power cut-in script, after SuperBreak_SetUpBackgroundBlend. Over EASE_FRAMES
 * frames: EVA eases 16 to 8 and EVB 0 to 8, and layer 3 scrolls in from x = 64 to 0.
 * Then it resets the frame counter and resumes the script. */
void SuperBreak_EaseInBackground(Task* task) {
  int bg;

  FRAME(task)++;
  data_027c0280->blendEva = Math_Ease(0, 16, 8, FRAME(task), EASE_FRAMES);
  data_027c0280->blendEvb = Math_Ease(0, 0, 8, FRAME(task), EASE_FRAMES);
  bg = data_027c0280->layer3Bg;
  data_027c0280->bgScrollX[bg] = Math_Ease(4, 64, 0, FRAME(task), EASE_FRAMES);
  if (FRAME(task) == EASE_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
