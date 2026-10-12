#include "types.h"
#include "game/screen_other_map.h"
#include "game/number_sprite_task.h"
#include "game/task.h"

void Task_ResumeScript(Task* task);
void NumberSpriteTask_SlideOut(NumberSpriteTask* task);
void NumberSprite_PutHoursMinutes(int x, int y, int seconds, int firstTile,
                                  int palette);
void NumberSprite_PutLabelAfterNumber(int x, int y, int value, int firstTile,
                                      int palette);
void NumberSprite_PutLabelBeforeNumber(int x, int y, int value, int firstTile,
                                       int palette);
BOOL TimeLimit_GetRemaining(int* value);
BOOL Game_GetDayLimit(void);
int Game_GetDaysLeft(void);
int Game_GetShownDay(void);

/* unk_30 of the first map's state. */
static inline u32 FirstMap_GetUnk30(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  u32 value;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  value = data_027c027c->unk_48[data_027c0284]->unk_1c4->unk_30;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return value;
}

/* Per-frame update of the number task: slides the number in, then draws it
 * by style. Style 0 shows the time left from TimeLimit_GetRemaining, style 1 the days
 * left (Game_GetDaysLeft) under a day limit (Game_GetDayLimit), style 2 the day
 * from Game_GetShownDay while neither limit applies. When its value no longer
 * applies, or the first map's unk_1c4->unk_30 is set (styles 0 and 1), the
 * number slides out instead. */
void NumberSpriteTask_Update(NumberSpriteTask* task) {
  int x;
  int y;
  int seconds;

  x = task->x;
  y = task->y;

  if (task->slideOffset != 0) {
    y += task->slideOffset;
    task->slideOffset -=
        (task->slideOffset > 0 ? 1 : -(task->slideOffset < 0)) * 2;
  }

  switch (task->style) {
    case 0:
      if (FirstMap_GetUnk30() != 0 || !TimeLimit_GetRemaining(&seconds)) {
        task->slideOffset = 32;
        Task_ResumeScript((Task*)task);
        NumberSpriteTask_SlideOut(task);
        return;
      }
      task->value = seconds;
      NumberSprite_PutHoursMinutes(x & 0x1ff, y & 0xff, seconds,
                                   task->firstTile, task->palette);
      break;
    case 1:
      if (FirstMap_GetUnk30() != 0 || !Game_GetDayLimit()) {
        task->slideOffset = 32;
        Task_ResumeScript((Task*)task);
        NumberSpriteTask_SlideOut(task);
        return;
      }
      task->value = Game_GetDaysLeft();
      NumberSprite_PutLabelAfterNumber(x & 0x1ff, y & 0xff, Game_GetDaysLeft(),
                                       task->firstTile, task->palette);
      break;
    case 2:
      if (TimeLimit_GetRemaining(&seconds) || Game_GetDayLimit()) {
        task->value = Game_GetShownDay();
        task->slideOffset = 32;
        Task_ResumeScript((Task*)task);
        NumberSpriteTask_SlideOut(task);
        return;
      }
      NumberSprite_PutLabelBeforeNumber(x & 0x1ff, y & 0xff, Game_GetShownDay(),
                                        task->firstTile, task->palette);
      break;
  }
}
