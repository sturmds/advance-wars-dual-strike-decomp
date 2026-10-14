#include "types.h"
#include "game/co_rank_task.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(CoRankTask* task);
void CoRankScreen_QueueNumber(int x, int y, int value);

/* One frame of an 8-frame shake: draws the number and the sprite beside
 * it at rest, and the data_027c0060 sprite up to 2 pixels off (212, 152)
 * each frame. Resumes the task's script on the last frame. */
void CoRankTask_ShakeSprite(CoRankTask* task) {
  task->frame++;
  CoRankScreen_QueueNumber(0, 136, task->experienceGain);
  Sprite_QueueAt(7, 164, 128, &data_027c0030, 0x5000, 0xfc);
  switch (task->frame) {
    case 1:
      Sprite_QueueAt(6, 214, 150, &data_027c0060, 0x1000, 0x13c);
      break;
    case 2:
      Sprite_QueueAt(6, 210, 154, &data_027c0060, 0x1000, 0x13c);
      break;
    case 3:
      Sprite_QueueAt(6, 214, 154, &data_027c0060, 0x1000, 0x13c);
      break;
    case 4:
      Sprite_QueueAt(6, 210, 150, &data_027c0060, 0x1000, 0x13c);
      break;
    case 5:
      Sprite_QueueAt(6, 214, 152, &data_027c0060, 0x1000, 0x13c);
      break;
    case 6:
      Sprite_QueueAt(6, 212, 150, &data_027c0060, 0x1000, 0x13c);
      break;
    case 7:
      Sprite_QueueAt(6, 210, 152, &data_027c0060, 0x1000, 0x13c);
      break;
    case 8:
      Sprite_QueueAt(6, 212, 154, &data_027c0060, 0x1000, 0x13c);
      break;
  }
  if (task->frame == 8) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
