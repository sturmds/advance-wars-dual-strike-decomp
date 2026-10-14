#include "types.h"
#include "game/co_rank_task.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void CoRankScreen_QueueNumber(int x, int y, int value);

/* Script step: draws the number and its two sprites at rest, as
 * CoRankTask_SlideDown leaves them. */
void CoRankTask_Draw(CoRankTask* task) {
  CoRankScreen_QueueNumber(0, 136, task->experienceGain);
  Sprite_QueueAt(7, 164, 128, &data_027c0030, 0x5000, 0xfc);
  Sprite_QueueAt(6, 212, 152, &data_027c0060, 0x1000, 0x13c);
}
