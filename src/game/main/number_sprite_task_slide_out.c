#include "types.h"
#include "game/number_sprite_task.h"
#include "game/task.h"

void Task_ResumeScript(Task* task);
void NumberSprite_PutHoursMinutes(int x, int y, int seconds, int firstTile,
                                  int palette);
void NumberSprite_PutLabelAfterNumber(int x, int y, int value, int firstTile,
                                      int palette);
void NumberSprite_PutLabelBeforeNumber(int x, int y, int value, int firstTile,
                                       int palette);

/* Draws the number while it slides up out of view: the update sets
 * slideOffset to 32 and the number moves from y to y - 32, two pixels a
 * frame. Once there, the task's script resumes. */
void NumberSpriteTask_SlideOut(NumberSpriteTask* task) {
  int x;
  int y;

  y = task->y - 32;
  x = task->x;

  if (task->slideOffset != 0) {
    y += task->slideOffset;
    task->slideOffset -=
        (task->slideOffset > 0 ? 1 : -(task->slideOffset < 0)) * 2;
  } else {
    Task_ResumeScript((Task*)task);
  }

  switch (task->style) {
    case 0:
      NumberSprite_PutHoursMinutes(x & 0x1ff, y & 0xff, task->value,
                                   task->firstTile, task->palette);
      break;
    case 1:
      NumberSprite_PutLabelAfterNumber(x & 0x1ff, y & 0xff, task->value,
                                       task->firstTile, task->palette);
      break;
    case 2:
      NumberSprite_PutLabelBeforeNumber(x & 0x1ff, y & 0xff, task->value,
                                        task->firstTile, task->palette);
      break;
  }
}
