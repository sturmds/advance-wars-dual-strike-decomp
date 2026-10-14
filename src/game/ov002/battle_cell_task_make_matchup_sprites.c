#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSpriteTask.h"
#include "game/task.h"

#define MENU ((MenuState*)&gMenuState)

/* Colour passed to BattleSpriteTask_Create for one of the four sprites of a cell, per side:
 * colours[0] and colours[1] for the first sprite, colours[2] and colours[3] for the second, and
 * so on. */
#define SIDE_COLOUR(sprite, side) (((u16(*)[2])MENU->colours)[sprite][side])

int Battle_GetMatchupNibble0(int side);
int Battle_GetMatchupNibble1(int side);
int Battle_GetSideMatchupValue(int side);
int Battle_GetMatchupValue(int side);

/* Makes the sprites of an HP cell again, at the task's position: the first one always, the
 * second and third when the side has a table for them, and the task's own shadow sprite when
 * battle.unk_40 has one. Each frame comes from the side's matchup. Sets hasShadow. */
void BattleCellTask_MakeMatchupSprites(BattleCellTask* task) {
  u16 scale;
  int side;
  BOOL left;
  int field58;
  int field56;

  if (CELL_TASK_ENTRY(task).sprites[0] != NULL) {
    Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[0]);
    CELL_TASK_ENTRY(task).sprites[0] = NULL;
  }
  scale = task->scale;
  side = task->side;
  left = (side == 0);
  field58 = left ? 0 : 2;
  field56 = left ? 0 : 2;
  CELL_TASK_ENTRY(task).sprites[0] = BattleSpriteTask_Create(
      MENU->battle.spriteTables[task->side], Battle_GetMatchupNibble0(side),
      task->posX, task->posY, task->posZ, 0, task->angle, 0, scale, scale,
      SIDE_COLOUR(0, side), field56, 0, side, task->hasPos2, task->pos2X,
      task->pos2Y, task->pos2Z, field58);

  if (MENU->battle.spriteTables2[task->side] != NULL) {
    if (CELL_TASK_ENTRY(task).sprites[1] != NULL) {
      Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[1]);
      CELL_TASK_ENTRY(task).sprites[1] = NULL;
    }
    scale = task->scale;
    side = task->side;
    left = (side == 0);
    field58 = left ? 0 : 2;
    field56 = left ? 0 : 2;
    CELL_TASK_ENTRY(task).sprites[1] = BattleSpriteTask_Create(
        MENU->battle.spriteTables2[task->side], Battle_GetMatchupNibble1(side),
        task->posX, task->posY, task->posZ, 0, task->angle, 0, scale, scale,
        SIDE_COLOUR(1, side), field56, 0, side, task->hasPos2, task->pos2X,
        task->pos2Y, task->pos2Z, field58);
  }

  if (MENU->battle.spriteTables3[task->side] != NULL) {
    if (CELL_TASK_ENTRY(task).sprites[2] != NULL) {
      Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[2]);
      CELL_TASK_ENTRY(task).sprites[2] = NULL;
    }
    scale = task->scale;
    side = task->side;
    left = (side == 0);
    field58 = left ? 0 : 2;
    field56 = left ? 0 : 2;
    CELL_TASK_ENTRY(task).sprites[2] = BattleSpriteTask_Create(
        MENU->battle.spriteTables3[task->side],
        Battle_GetSideMatchupValue(side), task->posX, task->posY, task->posZ, 0,
        task->angle, 0, scale, scale, SIDE_COLOUR(2, side), field56, 0, side,
        task->hasPos2, task->pos2X, task->pos2Y, task->pos2Z, field58);
  }

  side = task->side;
  if (MENU->battle.unk_40[side] == NULL) {
    return;
  }
  scale = task->scale;
  {
    int frame = Battle_GetMatchupValue(side);
    int offsetX = BattleCellTask_GetOffsetX(task);
    int offsetY = BattleCellTask_GetOffsetY(task);

    task->shadow = BattleSpriteTask_Create(
        MENU->battle.unk_40[task->side], frame, task->posX, task->posY,
        task->posZ, offsetX, task->angle + offsetY, -1, scale, scale,
        SIDE_COLOUR(3, side), 8, 0, side, task->hasPos2, task->pos2X,
        task->pos2Y, task->pos2Z, 8);
  }
  task->hasShadow = 1;
}
