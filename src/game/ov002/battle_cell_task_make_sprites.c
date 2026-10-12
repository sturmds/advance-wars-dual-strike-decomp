#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_scene.h"
#include "game/task.h"

#define MENU ((MenuState*)&gMenuState)

/* (&colours[2])[side] below: needed to match; colours[2 + side] adds at run time. */

/* Remakes the sprites of a cell: the first always, the second and third when the side has a
 * sprite table for them. Each comes from the side's own table, sprite index and colour. */
void BattleCellTask_MakeSprites(BattleCellTask* task) {
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

  if (MENU->battle.spriteTables2[task->side] != NULL) {
    if (CELL_TASK_ENTRY(task).sprites[1] != NULL) {
      Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[1]);
      CELL_TASK_ENTRY(task).sprites[1] = NULL;
    }
    side = task->side;
    left = (side == 0);
    CELL_TASK_ENTRY(task).sprites[1] = BattleSpriteTask_Create(
        MENU->battle.spriteTables2[side],
        MENU->battle.unk_27c[side]
            ->unk_08[Ov002_gBattleScene->sides[side].usesSecondary]
            ->spriteIndex,
        task->posX, task->posY, task->posZ, 0, task->angle, 0, task->scale,
        task->scale, (&MENU->colours[2])[side], left ? 0 : 2, 0, side,
        task->hasPos2, task->pos2X, task->pos2Y, task->pos2Z, left ? 0 : 2);
  }

  if (MENU->battle.spriteTables3[task->side] == NULL) {
    return;
  }
  if (CELL_TASK_ENTRY(task).sprites[2] != NULL) {
    Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[2]);
    CELL_TASK_ENTRY(task).sprites[2] = NULL;
  }
  side = task->side;
  left = (side == 0);
  CELL_TASK_ENTRY(task).sprites[2] = BattleSpriteTask_Create(
      MENU->battle.spriteTables3[side],
      MENU->battle.unk_27c[side]->unk_10->spriteIndex, task->posX, task->posY,
      task->posZ, 0, task->angle, 0, task->scale, task->scale,
      (&MENU->colours[4])[side], left ? 0 : 2, 0, side, task->hasPos2,
      task->pos2X, task->pos2Y, task->pos2Z, left ? 0 : 2);
}
