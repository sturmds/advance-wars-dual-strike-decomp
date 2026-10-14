#include "types.h"
#include "game/task.h"
#include "game/battle_fighter_task.h"
#include "game/map_select.h"

void Task_Destroy(Task* task);
int Battle_GetMatchupNibble1(int side);

#define FIGHTER_ENTRY(task) \
  (MENU->battle.unk_310[(task)->fighter->side][(task)->fighter->cell])

/* Start of a side's fighter task: finds the fighter, the side's step list
 * and animation data, destroys the fighter's old sprite task and makes a new
 * one from the side's sprite table. Needed to match: `left` reads
 * task->fighter through a cast, so the compiler loads it again for
 * `fighter` instead of sharing the load. */
void BattleFighterTask_Setup(BattleFighterTask* task) {
  BattleCellTask* fighter;
  BOOL left;
  int unk_58;
  int unk_56;
  int side;
  int scale;

  task->fighter = (BattleCellTask*)MENU->battle.unk_310[task->side][0].unk_00;
  task->hitList = &MENU->battle.hitLists[task->side];
  task->layout = MENU->battle.unk_27c[task->side]->layout;
  if (MENU->battle.spriteTables2[task->side] != 0) {
    if (FIGHTER_ENTRY(task).sprites[1] != NULL) {
      Task_Destroy((Task*)FIGHTER_ENTRY(task).sprites[1]);
      FIGHTER_ENTRY(task).sprites[1] = NULL;
    }
    left = (*(BattleCellTask**)&task->fighter)->side == 0 ? TRUE : FALSE;
    fighter = task->fighter;
    scale = fighter->scale;
    unk_58 = left ? 0 : 2;
    unk_56 = left ? 0 : 2;
    side = fighter->side;
    FIGHTER_ENTRY(task).sprites[1] = BattleSpriteTask_Create(
        (const u32*)MENU->battle.spriteTables2[task->fighter->side],
        task->hitList->count + Battle_GetMatchupNibble1(task->side) - 1,
        fighter->posX, fighter->posY, fighter->posZ, 0, fighter->angle, 0,
        scale, scale, (&MENU->colours[2])[side], unk_56, 0, side,
        fighter->hasPos2, fighter->pos2X, fighter->pos2Y, fighter->pos2Z,
        unk_58);
    task->sprite = FIGHTER_ENTRY(task).sprites[1];
  }
  task->step = 0;
}
