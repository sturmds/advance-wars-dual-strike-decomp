#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/math.h"

#define FADE_FRAMES 32

s16 BreakText_GetCentredX(u8 slot);
void BreakText_QueueLetterSized(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

void ScreenFadeSprites_Update(Task* task) {
  int i;

  FRAME(task)++;
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetterSized(i, BreakText_GetCentredX(i) & 0x1ff, 80, 0, 1);
  }
  data_027c0280->masterBrightness =
      Math_Ease(4, 0, 16, FRAME(task), FADE_FRAMES);
  if (FRAME(task) == FADE_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
