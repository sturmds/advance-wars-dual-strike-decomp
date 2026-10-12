#include "types.h"
#include "game/task.h"
#include "game/force_rank_task.h"
#include "game/menu_state.h"

void func_020b9864(void);
void func_02069b14(void);
void VsSetup_Init(int unused);
void* Task_FindByScript(const void* script);
int Task_JumpToLabel(ForceRankTask* task, int label);

extern const u32 data_0215cfe8[];
extern const TaskScriptCmd data_0215cae8[];
extern MenuState gMenuState;

/* Script step of the Force Rank screen task. With a cancel pending (state 4
 * in setup.unk_2ea) it runs the cancel task and ends the Force Rank task.
 * Otherwise it sets state 3, builds the Vs setup screen again and starts
 * its task under the setup task. */
void VsSetup_ReturnFromForceRank(ForceRankTask* task) {
  if (gMenuState.setup.unk_2ea == 4) {
    func_020b9864();
    func_02069b14();
    Task_JumpToLabel(task, 255);
    return;
  }
  gMenuState.setup.unk_2ea = 3;
  VsSetup_Init(0);
  Task_Create(data_0215cae8, Task_FindByScript(data_0215cfe8));
}
