#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba8.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Ov005_UnpackCoScreenPicture(Unk_ov005_0236bba8* work);
void Ov005_DrawCoGridRowIcon(Unk_ov005_0236bba8* work);
void Ov005_QueueCoSpriteOnBothScreens(Unk_ov005_0236bba8* work, int x, int y);
void Ov005_LoadCoGraphicsToBothScreens(Unk_ov005_0236bba8* work);

/* Per-frame update of the CO picture. When work->coId changes, the picture
 * slides 144 pixels away over 8 frames, the new CO is loaded, and after
 * 4 frames it slides back. */
void Ov005_UpdateCoPicture(Ov005Task* task) {
  Unk_ov005_0236bba8* work = task->work;
  int offset;

  switch (task->step) {
    case 0:
      task->frame = 0;
      task->frames = 8;
      task->step++;
    case 1:
      if (work->coId != work->cursorCoId) {
        task->frame++;
      } else {
        task->frame--;
      }
      if (task->frame < 0) {
        task->frame = 0;
      }
      offset = Math_Ease(1, 0, 0x90000, task->frame, task->frames);
      if (task->frame >= task->frames) {
        task->step++;
      }
      Ov005_QueueCoSpriteOnBothScreens(work, (work->unk_1fc + offset) >> 12,
                                       work->unk_200 >> 12);
      break;
    case 2:
      Ov005_LoadCoGraphicsToBothScreens(work);
      Ov005_UnpackCoScreenPicture(work);
      work->cursorCoId = work->coId;
      task->frame = 0;
      task->frames = 4;
      task->step++;
    case 3:
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step++;
      }
      break;
    case 4:
      task->frame = 8;
      task->frames = 8;
      task->step = 1;
      break;
  }
  Ov005_DrawCoGridRowIcon(work);
}
