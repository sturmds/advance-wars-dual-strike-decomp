#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"

BOOL Battle_IsSideAir(int side);
BOOL Battle_IsAttackerWeaponShown(int side);
int Battle_IsSideSea(int side);
void Battle_FadeAttackerSubSe(int side);

#define MENU ((MenuState*)&gMenuState)

/* Per side: a ground side (neither air nor sea) runs Battle_FadeAttackerSubSe; then, when the
 * side's unk_48 is 0, step 2 goes into the steps ring of the task of each cell with a nonzero
 * fill, if Battle_IsAttackerWeaponShown. Like BattleCells_PushStep, which pushes 0 or 1. */
void BattleCells_PushStep2(void) {
  int side;
  int cell;
  BattleCellTask* task;

  for (side = 0; side < BATTLE_SIDE_COUNT; side++) {
    if (!Battle_IsSideAir(side)) {
      if (!Battle_IsSideSea(side)) {
        Battle_FadeAttackerSubSe(side);
      }
    }
    if (MENU->battle.unk_27c[side]->unk_48 == 0) {
      for (cell = 0; cell < BATTLE_CELL_COUNT; cell++) {
        if (MENU->battle.unk_310[side][cell].fillBefore != 0 &&
            Battle_IsAttackerWeaponShown(side)) {
          task = (BattleCellTask*)MENU->battle.unk_310[side][cell].unk_00;
          task->steps[task->stepIndex] = 2;
          task->stepIndex++;
          if (task->stepIndex == 8) {
            task->stepIndex = 0;
          }
        }
      }
    }
  }
}
