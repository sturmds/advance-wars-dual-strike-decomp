#include "types.h"
#include "game/screen_other_map.h"
#include "game/points_tally_task.h"

void PointsTally_Draw(int x, int y, int added, int total, int popFrame);

/* Script step: draws the points box at rest on the screen of
 * data_027c0288.unk_04, at y 136. PointsTally_HoldOnMain draws the same
 * box at y 88. */
void PointsTally_HoldOnSubScreen(PointsTallyTask* task) {
  Unk_027c0280* savedGfx = data_027c0280;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  PointsTally_Draw(56, 136, task->added, task->total, -1);
  Screen_SetCurrentGfx(savedGfx);
}
