#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSideData.h"
#include "game/BattleSpriteTask.h"
#include "game/battle_fighter_task.h"
#include "game/task.h"

#define MENU ((MenuState*)&gMenuState)

/* Needed to match: every use reads task->fighter again; a local merges the loads. */
#define CELL (task->fighter)

int Battle_GetMatchupNibble0(int side);
int Battle_GetMatchupNibble1(int side);

/* Binds the task to the first HP cell of its side, then remakes that cell's sprite: the second
 * sprite when the side has a second sprite table, else the first. The sprite index comes from
 * the side's air/ground matchup. */
void BattleCellSpriteTask_Init(BattleFighterTask* task) {
  BOOL left;

  task->fighter = (BattleCellTask*)MENU->battle.unk_310[task->side][0].unk_00;
  task->hitList = &MENU->battle.hitLists[task->side];
  task->cells = MENU->battle.unk_27c[task->side]->cellPositions;
  if (MENU->battle.spriteTables2[task->side] != NULL) {
    if (CELL_TASK_ENTRY(CELL).sprites[1] != NULL) {
      Task_Destroy((Task*)CELL_TASK_ENTRY(CELL).sprites[1]);
      CELL_TASK_ENTRY(CELL).sprites[1] = NULL;
    }
    left = (CELL->side == 0);
    /* (&colours[2])[side]: needed to match; colours[2 + side] adds at run time */
    CELL_TASK_ENTRY(CELL).sprites[1] = BattleSpriteTask_Create(
        MENU->battle.spriteTables2[CELL->side],
        Battle_GetMatchupNibble1(task->side), CELL->posX, CELL->posY,
        CELL->posZ, 0, CELL->angle, 0, CELL->scale, CELL->scale,
        (&MENU->colours[2])[CELL->side], left ? 0 : 2, 0, CELL->side,
        CELL->hasPos2, CELL->pos2X, CELL->pos2Y, CELL->pos2Z, left ? 0 : 2);
    task->sprite = CELL_TASK_ENTRY(CELL).sprites[1];
  } else {
    if (CELL_TASK_ENTRY(CELL).sprites[0] != NULL) {
      Task_Destroy((Task*)CELL_TASK_ENTRY(CELL).sprites[0]);
      CELL_TASK_ENTRY(CELL).sprites[0] = NULL;
    }
    left = (CELL->side == 0);
    CELL_TASK_ENTRY(CELL).sprites[0] = BattleSpriteTask_Create(
        MENU->battle.spriteTables[CELL->side],
        Battle_GetMatchupNibble0(task->side), CELL->posX, CELL->posY,
        CELL->posZ, 0, CELL->angle, 0, CELL->scale, CELL->scale,
        MENU->colours[CELL->side], left ? 0 : 2, 0, CELL->side, CELL->hasPos2,
        CELL->pos2X, CELL->pos2Y, CELL->pos2Z, left ? 0 : 2);
    task->sprite = CELL_TASK_ENTRY(CELL).sprites[0];
  }
  task->step = 0;
}
