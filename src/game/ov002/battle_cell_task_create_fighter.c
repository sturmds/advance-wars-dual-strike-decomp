#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/task.h"

#define MENU ((MenuState*)&gMenuState)

extern const TaskScriptCmd data_ov002_0236963c[];

BOOL Battle_IsAttackerWeaponShown(int side);

/* Makes the one task of a side whose animation data has unk_48 set (the other sides get one
 * per cell from BattleCellTask_Create). It starts at the layout's position and scale, 50 pixels
 * right of it when Battle_IsAttackerWeaponShown. */
BattleCellTask* BattleCellTask_CreateFighter(int side) {
  BattleCellTask* task =
      (BattleCellTask*)Task_Create(data_ov002_0236963c, TASK_LIST(3));
  const BattleFighterLayout* layout =
      &MENU->battle.unk_27c[side]->layout->fighter;

  task->side = side;
  task->cell = 0;
  task->posX = layout->pos.x;
  task->posY = layout->pos.y;
  task->posZ = layout->pos.z;
  task->scale = layout->scale;
  if (Battle_IsAttackerWeaponShown(side) == 1) {
    task->posX += 50;
  }
  task->stepIndex = 0;
  task->hasPos2 = 0;
  return task;
}
