#include "types.h"
#include "game/task.h"

s16 BreakText_GetCentredX(u8 slot);
void BreakText_QueueLetterSized(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define HOLD_FRAMES 60

/* Step of the tag break script: keeps drawing the task->unk_58 name letters
 * at y 48 for HOLD_FRAMES frames, then resumes the script. */
void TagBreak_HoldName(Task* task) {
  int i;

  FRAME(task)++;
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetterSized(i, BreakText_GetCentredX(i) & 0x1ff, 48, 0, 0);
  }
  if (FRAME(task) == HOLD_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
