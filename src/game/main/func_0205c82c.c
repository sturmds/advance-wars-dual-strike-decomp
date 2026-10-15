#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/math.h"

#define FADE_FRAMES 32
void Task_ResumeScript(Task* task);
#define FRAME(task) (*(s16*)&(task)->unk_4c)

void func_0205c82c(Task* task) {
  FRAME(task)++;
  data_027c0280->masterBrightness =
      Math_Ease(4, -16, 0, FRAME(task), FADE_FRAMES);
  if (FRAME(task) == FADE_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
