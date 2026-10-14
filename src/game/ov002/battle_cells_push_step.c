#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"

void Battle_StartAttackerMoveSound(int side);
BOOL Battle_IsAttackerWeaponShown(int side);

#define MENU ((MenuState*)&gMenuState)

/* Adds one entry to the steps ring of the task of each HP cell with a nonzero fill (only the
 * first cell's task when the side's unk_48 is set): 1 when Battle_IsAttackerWeaponShown is true for the
 * side, else 0. */
void BattleCells_PushStep(void) {
  BattleCellTask* task;
  int side;
  int cell;

  for (side = 0; side < BATTLE_SIDE_COUNT; side++) {
    Battle_StartAttackerMoveSound(side);
    if (MENU->battle.unk_27c[side]->unk_48 == 0) {
      for (cell = 0; cell < BATTLE_CELL_COUNT; cell++) {
        if (MENU->battle.unk_310[side][cell].fillBefore != 0) {
          task = (BattleCellTask*)MENU->battle.unk_310[side][cell].unk_00;
          if (Battle_IsAttackerWeaponShown(side) == 1) {
            task->steps[task->stepIndex] = 1;
          } else {
            task->steps[task->stepIndex] = 0;
          }
          task->stepIndex++;
          if (task->stepIndex == 8) {
            task->stepIndex = 0;
          }
        }
      }
    } else {
      task = (BattleCellTask*)MENU->battle.unk_310[side][0].unk_00;
      task->steps[task->stepIndex] = 0;
      task->stepIndex++;
      if (task->stepIndex == 8) {
        task->stepIndex = 0;
      }
    }
  }
}
