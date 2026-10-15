#include "types.h"
#include "game/task.h"
#include "game/win_quote.h"

s16 BreakText_GetCentredX(u8 slot);
void BreakText_QueueLetterSized(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define HOLD_FRAMES 60

/* Step of the Super CO Power cut-in script, after SuperBreak_LettersPopInUpdate:
 * keeps drawing the power name letters at normal size for HOLD_FRAMES frames, then
 * resets the frame counter and resumes the script. */
void SuperBreak_HoldName(Task* task) {
  int i;

  FRAME(task)++;
  Sprite_QueueCoPortrait(184, 160, 0x800, 16, 2, 0);
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetterSized(i, BreakText_GetCentredX(i) & 0x1ff, 80, 0, 1);
  }
  if (FRAME(task) == HOLD_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
