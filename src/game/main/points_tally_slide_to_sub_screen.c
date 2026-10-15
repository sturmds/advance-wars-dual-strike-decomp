#include "types.h"
#include "game/screen_other_map.h"
#include "game/points_tally_task.h"
#include "game/math.h"

void PointsTally_Draw(int x, int y, int added, int total, int popFrame);
int func_0204cd00(void);
void Task_ResumeScript(PointsTallyTask* task);

/* Sets the Y scroll of layers 1 and 0 of the current screen to `value`. */
static inline void SetLayer01ScrollY(int value) {
  u8 layer1 = data_027c0280->layer1Bg;
  u8 layer0 = data_027c0280->layer0Bg;

  data_027c0280->bgScrollY[layer1] = value;
  data_027c0280->bgScrollY[layer0] = data_027c0280->bgScrollY[layer1];
}

/* Script step, 24 frames: eases the points box from y 88 on the screen of
 * data_027c0288.unk_00 (PointsTally_HoldOnMain) to y 136 on the screen of
 * unk_04 (PointsTally_HoldOnSubScreen), as one 280 to 136 move across both
 * screens stacked, and scrolls layers 0 and 1 of both screens up 144 pixels
 * with it. Then resumes the task of script data_0215b22c (func_0204cd00)
 * and this script. */
void PointsTally_SlideToSubScreen(PointsTallyTask* task) {
  int y;
  int scroll;
  Unk_027c0280* saved;

  task->frame++;
  y = Math_EaseInOut(280, 136, task->frame, 24);
  scroll = Math_EaseInOut(0, -144, task->frame, 24);

  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  SetLayer01ScrollY(-192 - scroll);
  if (y < 192) {
    PointsTally_Draw(56, y, task->added, task->total, -1);
  }
  Screen_SetCurrentGfx(saved);

  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  SetLayer01ScrollY(-scroll);
  if (y > 160) {
    PointsTally_Draw(56, y - 192, task->added, task->total, -1);
  }
  Screen_SetCurrentGfx(saved);

  if (task->frame == 24) {
    func_0204cd00();
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
