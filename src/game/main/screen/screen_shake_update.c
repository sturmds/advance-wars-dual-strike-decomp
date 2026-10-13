#include "types.h"
#include "game/screen_shake.h"
#include "game/Unk_02003f44.h"

void Task_ResumeScript(Unk_02003f44* task);

/* Task step: each frame, takes the next (x, y) pair of the shake table and
 * stores it in the offsets that Screen_Apply* subtract from the BG scroll.
 * The task resumes its script after unk_44 steps, or after one pass of the
 * table (ended by 0x7fff) when unk_44 is -1. */
void ScreenShake_Update(Unk_02003f44* task) {
  s16(*pairs)[2] = (s16(*)[2])task->unk_4c;

  if (pairs[task->unk_54][0] == 0x7fff) {
    task->unk_54 = 0;
    if (task->unk_44 == -1) {
      Task_ResumeScript(task);
      return;
    }
  }
  gScreenShakeX = pairs[task->unk_54][0];
  gScreenShakeY = pairs[task->unk_54][1];
  task->unk_54++;
  task->unk_58++;
  if (task->unk_58 != task->unk_44) {
    return;
  }
  Task_ResumeScript(task);
}
