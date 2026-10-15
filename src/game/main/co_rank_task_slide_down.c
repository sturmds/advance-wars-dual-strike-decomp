#include "types.h"
#include "game/co_rank_task.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"
#include "game/math.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(CoRankTask* task);
void CoRankScreen_QueueNumber(int x, int y, int value);

/* One frame of a 16-frame slide: layers 0 and 1 and the sprites on them
 * move down by an eased 0 to 64 pixels. The number from CoRankScreen_QueueNumber
 * moves with them but stops at y 174. Resumes the task's script on the
 * last frame. */
void CoRankTask_SlideDown(CoRankTask* task) {
  int offset;
  u8 layer1;
  u8 layer0;

  task->frame++;
  offset = Math_Ease(1, 0, 64, task->frame, 16);
  layer1 = data_027c0280->layer1Bg;
  layer0 = data_027c0280->layer0Bg;
  data_027c0280->bgScrollY[layer1] = -offset;
  data_027c0280->bgScrollY[layer0] = data_027c0280->bgScrollY[layer1];
  if (offset + 136 < 174) {
    CoRankScreen_QueueNumber(0, offset + 136, task->experienceGain);
  } else {
    CoRankScreen_QueueNumber(0, 174, task->experienceGain);
  }
  Sprite_QueueAt(7, 164, (offset + 128) & 0xff, &data_027c0030, 0x5000, 0xfc);
  Sprite_QueueAt(6, 212, (offset + 152) & 0xff, &data_027c0060, 0x1000, 0x13c);
  if (task->frame == 16) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
