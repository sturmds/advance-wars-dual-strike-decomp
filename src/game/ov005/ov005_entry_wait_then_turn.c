#include "types.h"
#include "game/ov005_entry_task.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);

/* Waits firstFrames, eases the entry's angle from 0xb000 up to a full turn
 * (0x10000, which wraps to 0) over 24 frames, then sets its id to 2. */
void Ov005Entry_WaitThenTurn(Ov005EntryTask* task) {
  Unk_ov005_0236bb90* work = task->work;
  int index = task->index;
  int firstFrames = task->firstFrames;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->frames = firstFrames;
      task->step++;
    case 1:
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame > task->frames) {
        task->step++;
      }
      return;
    case 2:
      task->frame = 0;
      task->frames = 24;
      task->step++;
    case 3:
      work->entriesA[index].angle =
          Math_Ease(0, 0xb000, 0x10000, task->frame, task->frames);
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame > task->frames) {
        task->step++;
      }
      return;
    case 4:
      work->entriesA[index].id = 2;
      task->busy = 0;
      return;
  }
}
