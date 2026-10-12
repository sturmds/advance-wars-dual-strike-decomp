#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_scene.h"
#include "game/task.h"

#define MENU ((MenuState*)&gMenuState)

void func_ov002_0235643c(const u32* table, int index, int unk_3c, int unk_3e,
                         int unk_40, int unk_48, int unk_4a, int unk_4c,
                         int unk_4e, int unk_50, int unk_54, int unk_56,
                         int unk_59, int unk_5a);

/* Destroys sprite i of the task's cell, when it exists. */
#define DESTROY_SPRITE(task, i)                              \
  do {                                                       \
    if (CELL_TASK_ENTRY(task).sprites[i] != NULL) {          \
      Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[i]); \
      CELL_TASK_ENTRY(task).sprites[i] = NULL;               \
    }                                                        \
  } while (0)

/* Stops the cell's movement (clears moving) and makes its sprites again at the task's positions:
 * one from each of the side's tables that is set, the last through func_ov002_0235643c.
 * Needed to match: the palette entries are read as (colours + n)[side], not colours[side + n]. */
void BattleCellTask_RemakeSprites(BattleCellTask* task) {
  BOOL left;
  int side;

  task->moving = 0;

  DESTROY_SPRITE(task, 0);
  side = task->side;
  left = (side == 0);
  CELL_TASK_ENTRY(task).sprites[0] = BattleSpriteTask_Create(
      MENU->battle.spriteTables[side],
      MENU->battle.unk_27c[side]
          ->weapons[Ov002_gBattleScene->sides[side].usesSecondary]
          ->unk_29_lo,
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
            ->unk_29_lo,
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
        MENU->battle.unk_27c[side]->unk_10->unk_29_lo, task->posX, task->posY,
        task->posZ, 0, task->angle, 0, task->scale, task->scale,
        (MENU->colours + 4)[side], left ? 0 : 2, 0, side, task->hasPos2,
        task->pos2X, task->pos2Y, task->pos2Z, left ? 0 : 2);
  }

  if (MENU->battle.unk_60[task->side] != NULL) {
    DESTROY_SPRITE(task, 3);
    side = task->side;
    func_ov002_0235643c(MENU->battle.unk_60[side], 1, task->posX, task->posY,
                        task->posZ, 0, task->angle, 0, task->scale, task->scale,
                        (MENU->colours + 14)[side], 8, 0, side);
  }
}
