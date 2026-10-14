#include "types.h"
#include "game/Unk_022a7a9c.h"
#include "game/menu_button.h"
#include "game/result_sprite_task.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Task_ResumeScript(ResultSpriteTask* task);
MenuButton* func_02032a9c(void);
void ResultPath_DrawAll(int step);

/* One frame of an 8-frame shake: draws the four sprites of ResultPath_DrawAll,
 * and the data_027c0060 sprite up to 2 pixels off (200, 88). On the last
 * frame adds the button of func_02032a9c and resumes the task's script. */
void ResultSprite_Shake(ResultSpriteTask* task) {
  task->frame++;
  ResultPath_DrawAll(195 - (5 - data_022a7a9c.rank) * 35);
  switch (task->frame) {
    case 1:
      Sprite_QueueAt(6, 202, 86, &data_027c0060, 0x1000, 0x40a);
      break;
    case 2:
      Sprite_QueueAt(6, 198, 90, &data_027c0060, 0x1000, 0x40a);
      break;
    case 3:
      Sprite_QueueAt(6, 202, 90, &data_027c0060, 0x1000, 0x40a);
      break;
    case 4:
      Sprite_QueueAt(6, 198, 86, &data_027c0060, 0x1000, 0x40a);
      break;
    case 5:
      Sprite_QueueAt(6, 202, 88, &data_027c0060, 0x1000, 0x40a);
      break;
    case 6:
      Sprite_QueueAt(6, 200, 86, &data_027c0060, 0x1000, 0x40a);
      break;
    case 7:
      Sprite_QueueAt(6, 198, 88, &data_027c0060, 0x1000, 0x40a);
      break;
    case 8:
      Sprite_QueueAt(6, 200, 90, &data_027c0060, 0x1000, 0x40a);
      break;
  }
  if (task->frame == 8) {
    task->frame = 0;
    func_02032a9c();
    Task_ResumeScript(task);
  }
}
