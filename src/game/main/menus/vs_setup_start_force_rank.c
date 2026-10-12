#include "types.h"
#include "game/task.h"
#include "game/force_rank_task.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

void* Task_FindByScript(const void* script);

extern const u32 data_0215cfe8[];
extern const TaskScriptCmd data_0215d150[];
extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offsets out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Starts the Force Rank screen task for CO slot coSlot (player * 2 + front or
 * back CO), under the task of the setup screen. */
void VsSetup_StartForceRank(int coSlot) {
  ForceRankTask* task;

  MenuButtons_RemoveSelectable();
  MenuButtons_DisableTouch();
  task = (ForceRankTask*)Task_Create(data_0215d150,
                                     Task_FindByScript(data_0215cfe8));
  task->co = MENU->setup.cos[coSlot >> 1][coSlot & 1];
  task->forceRankSet = MENU->setup.forceRankSets[coSlot >> 1][coSlot & 1];
  MENU->setup.unk_2ea = 1;
}
