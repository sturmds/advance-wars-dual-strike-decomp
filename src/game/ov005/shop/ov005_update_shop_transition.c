#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbb0.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Ov005Task* task);
void EventPicture_DrawAtStep(u32 level);

extern Unk_ov005_0236bbb0* data_ov005_0236bbb0;

/* Eases a level over 8 frames, from 6 down to 0 when transition is 1 and from 0
 * up to 6 otherwise, and passes it to EventPicture_DrawAtStep each frame, which slides the 6 by 6
 * picture in or out. Step 2 clears transition and resumes the task script. */
void Ov005_UpdateShopTransition(Ov005Task* task) {
  Unk_ov005_0236bbb0* work = data_ov005_0236bbb0;
  int level;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->frames = 8;
      task->step++;
    case 1:
      if (work->transition == 1) {
        level = Math_Ease(0, 0x6000, 0, task->frame, task->frames);
      } else {
        level = Math_Ease(0, 0, 0x6000, task->frame, task->frames);
      }
      if (task->frame >= task->frames) {
        task->step++;
      } else {
        task->frame++;
      }
      EventPicture_DrawAtStep((level >> 12) & 0xff);
      return;
    case 2:
      work->transition = 0;
      Task_ResumeScript(task);
      return;
  }
}
