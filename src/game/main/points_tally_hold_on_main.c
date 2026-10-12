#include "types.h"
#include "game/points_tally_task.h"

void PointsTally_Draw(int x, int y, int added, int total, int popFrame);

/* Script step: draws the points box at rest at y 88. */
void PointsTally_HoldOnMain(PointsTallyTask* task) {
  PointsTally_Draw(56, 88, task->added, task->total, -1);
}
