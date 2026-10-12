#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

#define FADE_FRAMES 16
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);
#define FRAME(task) (*(s16*)&(task)->unk_4c)

void func_0205c02c(Task* task) {
  FRAME(task)++;
  data_027c0280->masterBrightness =
      Math_Ease(4, 16, 0, FRAME(task), FADE_FRAMES);
  if (FRAME(task) == FADE_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
