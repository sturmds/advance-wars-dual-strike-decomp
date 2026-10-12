#include "types.h"
#include "game/task.h"

void TagBreak_DrawSidePiece(int x, int y, int side);
s16 BreakText_GetScreenX(u8 slot);
void BreakText_QueueLetter(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define HOLD_FRAMES 8

/* Step of the Tag Power cut-in script: keeps both side pieces and the
 * task->unk_58 name letters in place for HOLD_FRAMES frames, then resumes the
 * script. */
void TagPowerScreen_HoldName(Task* task) {
  int i;

  FRAME(task)++;
  TagBreak_DrawSidePiece(72, 224, 0);
  TagBreak_DrawSidePiece(184, 224, 1);
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetter(i, BreakText_GetScreenX(i) & 0x1ff, 80, 0, 0);
  }
  if (FRAME(task) == HOLD_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
