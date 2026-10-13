#include "types.h"
#include "game/task.h"
#include "game/force_rank_screen.h"

ForceRankScreen* gForceRankScreen;

void* Mem_Alloc(u32 size);

extern const TaskScriptCmd gForceRankScreenScript[];

/* Starts the Force Rank screen task (task list 3 unless taskList is given) and
 * its work area for CO `co`, skill set `set`, saved with ForceRank_SaveSets(mode). */
void ForceRankScreen_Start(int co, int mode, int set, int taskList) {
  if (taskList == 0) {
    taskList = 3;
  }
  Task_CreateChild(gForceRankScreenScript, TASK_LIST(taskList));
  gForceRankScreen = Mem_Alloc(sizeof(ForceRankScreen));
  gForceRankScreen->co = co;
  gForceRankScreen->mode = mode;
  gForceRankScreen->set = set;
}
