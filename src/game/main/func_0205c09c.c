#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

#define FADE_FRAMES 32
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
s16 BreakText_GetScreenX(u8 slot);
void BreakText_QueueLetter(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);
#define FRAME(task) (*(s16*)&(task)->unk_4c)

void func_0205c09c(Task* task) {
  int i;

  FRAME(task)++;
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetter(i, BreakText_GetScreenX(i) & 0x1ff, 80, 0, 1);
  }
  data_027c0280->masterBrightness =
      Math_Ease(4, 0, 16, FRAME(task), FADE_FRAMES);
  if (FRAME(task) == FADE_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
