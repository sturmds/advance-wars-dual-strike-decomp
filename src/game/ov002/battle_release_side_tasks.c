#include "types.h"
#include "game/task.h"
#include "game/menu_state.h"

void Task_Destroy(Task* task);
int Battle_IsSideDrawnInAir(int side);

/* The task lists of a side's cells: unk_22c and unk_254 */
#define SIDE_TASK_LIST_COUNT 2

/* Per frame, for each task kept in the two per-side lists: when Battle_IsSideDrawnInAir reports 1
 * for the side, advance the task's two counters; destroy the task once its unk_5d is zero.
 * Needed to match: the two lists are read through a ternary on j, not as one array. */
void Battle_ReleaseSideTasks(void) {
  int found[BATTLE_SIDE_COUNT];
  int side;
  int i;
  int j;

  for (side = 0; side < BATTLE_SIDE_COUNT; side++) {
    found[side] = Battle_IsSideDrawnInAir(side);
  }
  for (side = 0; side < BATTLE_SIDE_COUNT; side++) {
    for (i = 0; i < BATTLE_CELL_COUNT; i++) {
      for (j = 0; j < SIDE_TASK_LIST_COUNT; j++) {
        BattleCounterTask* task = (j == 0) ? gMenuState.battle.unk_22c[side][i]
                                           : gMenuState.battle.unk_254[side][i];

        if (task == NULL) {
          continue;
        }
        if (found[side] == 1) {
          task->unk_3c++;
          task->unk_42++;
        }
        if (task->unk_5d == 0) {
          if (task != NULL) {
            Task_Destroy((Task*)task);
          }
          if (j == 0) {
            gMenuState.battle.unk_22c[side][i] = NULL;
          } else {
            gMenuState.battle.unk_254[side][i] = NULL;
          }
        }
      }
    }
  }
}
