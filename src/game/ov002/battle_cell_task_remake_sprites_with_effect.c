#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_scene.h"
#include "game/task.h"
#include "game/unit_stats.h"

#define MENU ((MenuState*)&gMenuState)

BOOL func_ov002_02354670(int side);
int BattleCellTask_GetCellOffsetX(BattleCellTask* task, int index);
int BattleCellTask_GetCellOffsetY(BattleCellTask* task, int index);

/* Destroys sprite i of the task's cell, when it exists. */
#define DESTROY_SPRITE(task, i)                              \
  do {                                                       \
    if (CELL_TASK_ENTRY(task).sprites[i] != NULL) {          \
      Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[i]); \
      CELL_TASK_ENTRY(task).sprites[i] = NULL;               \
    }                                                        \
  } while (0)

/* Step 5 of a cell task: makes the cell's sprites again from the unk_2b index of each table,
 * then, when the side has an unk_58 table, an extra sprite at 1.5 times the scale, stored in
 * unk_254 (from unk_70 for the unit types func_ov002_02354670 picks). Infantry, mech, air and
 * sea units, Oozium and types 27, 28, 31 and 32 then stop bobbing (flag3). */
void BattleCellTask_RemakeSpritesWithEffect(BattleCellTask* task) {
  BOOL left;
  int side;
  int scale;

  DESTROY_SPRITE(task, 0);
  side = task->side;
  left = (side == 0);
  CELL_TASK_ENTRY(task).sprites[0] = BattleSpriteTask_Create(
      MENU->battle.spriteTables[side],
      MENU->battle.unk_27c[side]
          ->weapons[Ov002_gBattleScene->sides[side].usesSecondary]
          ->unk_2b,
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
            ->unk_2b,
        task->posX, task->posY, task->posZ, 0, task->angle, 0, task->scale,
        task->scale, (MENU->colours + 2)[side], left ? 0 : 2, 0, side,
        task->hasPos2, task->pos2X, task->pos2Y, task->pos2Z, left ? 0 : 2);
  }

  if (MENU->battle.spriteTables3[task->side] != NULL) {
    DESTROY_SPRITE(task, 2);
    side = task->side;
    left = (side == 0);
    CELL_TASK_ENTRY(task).sprites[2] = BattleSpriteTask_Create(
        MENU->battle.spriteTables3[side],
        MENU->battle.unk_27c[side]->unk_10->unk_2b, task->posX, task->posY,
        task->posZ, 0, task->angle, 0, task->scale, task->scale,
        (MENU->colours + 4)[side], left ? 0 : 2, 0, side, task->hasPos2,
        task->pos2X, task->pos2Y, task->pos2Z, left ? 0 : 2);
  }

  if (MENU->battle.unk_58[task->side] != NULL) {
    if (func_ov002_02354670(task->side) == TRUE) {
      int effectX;

      scale = (task->scale * 0x180) >> 8;
      effectX = BattleCellTask_GetCellOffsetY(task, -1);
      MENU->battle.unk_254[task->side][task->cell] =
          (BattleCounterTask*)BattleSpriteTask_Create(
              MENU->battle.unk_70, 0, task->posX, task->posY, task->posZ,
              effectX, task->angle + BattleCellTask_GetCellOffsetX(task, -1),
              -1, scale, scale, MENU->battle.unk_24, 8, 0, task->side,
              task->hasPos2, task->pos2X, task->pos2Y, task->pos2Z, 8);
    } else {
      int effectX;

      side = task->side;
      scale = (task->scale * 0x180) >> 8;
      effectX = BattleCellTask_GetCellOffsetY(task, -1);
      MENU->battle.unk_254[task->side][task->cell] =
          (BattleCounterTask*)BattleSpriteTask_Create(
              MENU->battle.unk_58[task->side], 1, task->posX, task->posY,
              task->posZ, effectX,
              task->angle + BattleCellTask_GetCellOffsetX(task, -1), -1, scale,
              scale, (MENU->colours + 12)[side], 8, 0, side, task->hasPos2,
              task->pos2X, task->pos2Y, task->pos2Z, 8);
    }
  }

  switch (BATTLE_SIDE_UNIT_TYPE(task->side)) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
    case UNIT_STEALTH:
    case UNIT_BLACK_BOMB:
    case UNIT_FIGHTER:
    case UNIT_BOMBER:
    case UNIT_BLACK_BOAT:
    case UNIT_B_COPTER:
    case UNIT_T_COPTER:
    case UNIT_BATTLESHIP:
    case UNIT_CRUISER:
    case UNIT_LANDER:
    case UNIT_SUB:
    case UNIT_CARRIER:
    case UNIT_OOZIUM:
    case 27:
    case 28:
    case 31:
    case 32:
      task->unk_57 = 0;
      task->flag3 = 1;
      break;
  }
}
