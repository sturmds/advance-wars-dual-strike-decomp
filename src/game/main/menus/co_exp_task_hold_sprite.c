#include "types.h"
#include "game/Unk_02031a14.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(Unk_02031a14* task);

/* One frame of a 16-frame wait: draws a sprite 16 pixels above the
 * task's position and resumes the task's script on the last frame. */
void CoExpTask_HoldSprite(Unk_02031a14* task) {
  Sprite_QueueAt(1, task->x & 0x1ff, (task->y - 16) & 0xff, &data_027c0030,
                 0xc000, 684);
  if (task->frame != 16) {
    task->frame++;
    return;
  }
  task->frame = 0;
  Task_ResumeScript(task);
}
