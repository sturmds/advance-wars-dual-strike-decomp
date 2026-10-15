#include "types.h"
#include "game/co_rank_task.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"
#include "game/math.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(CoRankTask* task);
void CoRankScreen_QueueNumber(int x, int y, int value);
int Survival_GetRunGrade(void);
void ResultRank_PlaySe(int grade);

/* One frame of a 24-frame slide back up, the reverse of CoRankTask_SlideDown:
 * layers 0 and 1, the number and the sprite move from 64 pixels down to 0.
 * On the last frame plays the sound of the run grade and resumes the
 * task's script. */
void CoRankTask_SlideUp(CoRankTask* task) {
  int offset;
  u8 layer1;
  u8 layer0;

  task->frame++;
  offset = Math_Ease(4, 64, 0, task->frame, 24);
  layer1 = data_027c0280->layer1Bg;
  layer0 = data_027c0280->layer0Bg;
  data_027c0280->bgScrollY[layer1] = -offset;
  data_027c0280->bgScrollY[layer0] = data_027c0280->bgScrollY[layer1];
  CoRankScreen_QueueNumber(0, offset + 136, task->experienceGain);
  Sprite_QueueAt(7, 164, (offset + 128) & 0xff, &data_027c0030, 0x5000, 0xfc);
  if (task->frame == 24) {
    ResultRank_PlaySe(Survival_GetRunGrade());
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
