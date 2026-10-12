#include "types.h"
#include "game/co_rank_task.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(CoRankTask* task);
void CoRankScreen_QueueNumber(int x, int y, int value);
void func_0204cc54(void);

extern u8 data_027c0030;
extern u8 data_027c0060;

/* Script step: holds the screen of CoRankTask_Draw for 16 frames, then adds
 * the button of func_0204cc54 and resumes the script. */
void CoRankTask_HoldThenAddButton(CoRankTask* task) {
  task->frame++;
  CoRankScreen_QueueNumber(0, 136, task->experienceGain);
  Sprite_QueueAt(7, 164, 128, &data_027c0030, 0x5000, 0xfc);
  Sprite_QueueAt(6, 212, 152, &data_027c0060, 0x1000, 0x13c);
  if (task->frame == 16) {
    task->frame = 0;
    func_0204cc54();
    Task_ResumeScript(task);
  }
}
