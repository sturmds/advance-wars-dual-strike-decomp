#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_scene.h"
#include "game/task.h"

#define MENU ((MenuState*)&gMenuState)

/* Destroys sprite i of the task's cell, when it exists. */
#define DESTROY_SPRITE(task, i)                              \
  do {                                                       \
    if (CELL_TASK_ENTRY(task).sprites[i] != NULL) {          \
      Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[i]); \
      CELL_TASK_ENTRY(task).sprites[i] = NULL;               \
    }                                                        \
  } while (0)

/* Starts the cell moving (sets moving) with posX falling by 1 per frame, then makes its sprites
 * again at the task's positions: one from each of the side's tables that is set.
 * Needed to match: the palette entries are read as (colours + n)[side], not colours[side + n]. */
void BattleCellTask_StartMove(BattleCellTask* task) {
  BOOL left;
  int side;

  task->moving = 1;
  task->velX = -0x100;
  task->velY = 0;
  task->accelX = 0;
  task->accelY = 0;

  DESTROY_SPRITE(task, 0);
  side = task->side;
  left = (side == 0);
  CELL_TASK_ENTRY(task).sprites[0] = BattleSpriteTask_Create(
      MENU->battle.spriteTables[side],
      MENU->battle.unk_27c[side]
          ->weapons[Ov002_gBattleScene->sides[side].usesSecondary]
          ->unk_28_hi,
      task->posX, task->posY, task->posZ, 0, task->angle, 0, task->scale,
      task->scale, MENU->colours[side], left ? 0 : 2, 0, side, task->hasPos2,
      task->pos2X, task->pos2Y, task->pos2Z, left ? 0 : 2);

  if (MENU->battle.spriteTables2[task->side] != NULL) {
    DESTROY_SPRITE(task, 1);
    side = task->side;
    left = (side == 0);
    CELL_TASK_ENTRY(task).sprites[1] = BattleSpriteTask_Create(
        MENU->battle.spriteTables2[side],
        MENU->battle.unk_27c[side]
            ->unk_08[Ov002_gBattleScene->sides[side].usesSecondary]
            ->unk_28_hi,
        task->posX, task->posY, task->posZ, 0, task->angle, 0, task->scale,
        task->scale, (MENU->colours + 2)[side], left ? 0 : 2, 0, side,
        task->hasPos2, task->pos2X, task->pos2Y, task->pos2Z, left ? 0 : 2);
  }

  if (MENU->battle.spriteTables3[task->side] != NULL) {
    DESTROY_SPRITE(task, 2);
    side = task->side;
    left = (side == 0);
    /* The original stores this sprite in slot 1, not 2. */
    CELL_TASK_ENTRY(task).sprites[1] = BattleSpriteTask_Create(
        MENU->battle.spriteTables3[side],
        MENU->battle.unk_27c[side]->unk_10->unk_28_hi, task->posX, task->posY,
        task->posZ, 0, task->angle, 0, task->scale, task->scale,
        (MENU->colours + 4)[side], left ? 0 : 2, 0, side, task->hasPos2,
        task->pos2X, task->pos2Y, task->pos2Z, left ? 0 : 2);
  }

  if (MENU->battle.unk_60[task->side] != NULL) {
    DESTROY_SPRITE(task, 3);
    side = task->side;
    CELL_TASK_ENTRY(task).sprites[3] = BattleSpriteTask_Create(
        MENU->battle.unk_60[side], 0, task->posX, task->posY, task->posZ, 0,
        task->angle, 0, task->scale, task->scale, (MENU->colours + 14)[side], 8,
        0, side, task->hasPos2, task->pos2X, task->pos2Y, task->pos2Z, 8);
  }
}
