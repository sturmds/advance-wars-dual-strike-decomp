#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Screen_SetCurrentBoth(void* state);
s16 BreakText_GetCentredX(u8 slot);
void BreakText_QueueLetterSized(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Step of the tag power cut-in: redraws the task->unk_58 text sprites at y 48
 * and fades both screens to white over 32 frames, then resumes the script. */
void TagBreak_FadeToWhite(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int i;

  FRAME(task)++;
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetterSized(i, BreakText_GetCentredX(i) & 0x1ff, 48, 0, 0);
  }
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->masterBrightness = Math_Ease(4, 0, 16, FRAME(task), 32);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->masterBrightness = Math_Ease(4, 0, 16, FRAME(task), 32);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  if (FRAME(task) == 32) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
