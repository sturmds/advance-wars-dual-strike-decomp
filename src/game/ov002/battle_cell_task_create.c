#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/task.h"

extern const TaskScriptCmd data_ov002_0236963c[];

BOOL Battle_IsAttackerWeaponShown(int side);

/* Makes the task of one HP cell. pos2Index is -1 for no second position. */
BattleCellTask* BattleCellTask_Create(int side, int cell, int pos2Index) {
  BattleCellTask* task =
      (BattleCellTask*)Task_Create(data_ov002_0236963c, TASK_LIST(3));
  /* Needed to match: reading gMenuState through a MenuState pointer folds the offset. */
  const BattleCellPos* positions =
      ((MenuState*)&gMenuState)->battle.unk_27c[side]->cellPositions;

  task->side = side;
  task->cell = cell;
  task->posX = positions[cell].x;
  task->posY = positions[cell].y;
  task->posZ = positions[cell].z;
  task->scale = 0x100;
  if (pos2Index == -1) {
    task->hasPos2 = 0;
  } else {
    task->hasPos2 = 1;
    task->pos2Index = pos2Index;
    task->pos2X = positions[pos2Index].x + 128;
    task->pos2Y = positions[pos2Index].y;
    task->pos2Z = positions[pos2Index].z;
  }
  if (Battle_IsAttackerWeaponShown(side) == 1) {
    task->posX += 50;
    task->pos2X += 50;
  }
  task->stepIndex = 0;
  return task;
}
