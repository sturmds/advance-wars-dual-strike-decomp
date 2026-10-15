#include "types.h"
#include "game/Unk_ov003_0235dd38.h"
#include "game/win_quote.h"

void BreakText_QueueLetterSized(u8 letter, u16 x, int y, int arg3, int list);
s16 BreakText_GetLetterX(u8 letter);
void Task_ResumeScript(Unk_ov003_0235dd38* task);

/* Step of the CO Power cut-in, between the grow-in and CoPowerScreen_GrowOutName:
 * keeps the banner and the power name letters in place for (16 - count) * 6
 * frames, then resumes the script. */
void CoPowerScreen_HoldName(Unk_ov003_0235dd38* task) {
  int i;

  task->frame++;
  Sprite_QueueCoPortrait(184, 160, 0, 16, 2, 0);
  for (i = 0; i < task->count; i++) {
    BreakText_QueueLetterSized(i, BreakText_GetLetterX(i) & 0x1ff, 80, 0, 1);
  }
  if (task->frame == (16 - task->count) * 6) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
