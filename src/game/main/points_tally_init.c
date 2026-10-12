#include "types.h"
#include "game/points_tally_task.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

u32 Survival_GetExperienceBonus(void);
int PointsTally_GetPopFrames(int total);

/* Script step: starts the tally. The survival experience bonus is the
 * points to add to the run's points. Also sets how long the pop-in of the
 * final total runs (PointsTally_PopIn). */
void PointsTally_Init(PointsTallyTask* task) {
  task->frame = 0;
  task->added = Survival_GetExperienceBonus();
  task->total = data_022a7b80.points;
  task->frames = PointsTally_GetPopFrames(task->total + task->added);
}
