#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_scene.h"
#include "game/task.h"

#define MENU ((MenuState*)&gMenuState)

/* Remakes the first sprite of a cell when its BattleSpriteTask.unk_5b is 4, then sets unk_5b to
 * 0xff. */
void BattleCellTask_RestartSprite(BattleCellTask* task) {
  if (CELL_TASK_ENTRY(task).sprites[0] == NULL) {
    return;
  }
  if (CELL_TASK_ENTRY(task).sprites[0]->unk_5b == 4) {
    BOOL left;
    int side;

    if (CELL_TASK_ENTRY(task).sprites[0] != NULL) {
      Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[0]);
      CELL_TASK_ENTRY(task).sprites[0] = NULL;
    }
    side = task->side;
    left = (side == 0);
    CELL_TASK_ENTRY(task).sprites[0] = BattleSpriteTask_Create(
        MENU->battle.spriteTables[side],
        MENU->battle.unk_27c[side]
            ->weapons[Ov002_gBattleScene->sides[side].usesSecondary]
            ->spriteIndex,
        task->posX, task->posY, task->posZ, 0, task->angle, 0, task->scale,
        task->scale, MENU->colours[side], left ? 0 : 2, 0, side, task->hasPos2,
        task->pos2X, task->pos2Y, task->pos2Z, left ? 0 : 2);
  }
  CELL_TASK_ENTRY(task).sprites[0]->unk_5b = 0xff;
}
