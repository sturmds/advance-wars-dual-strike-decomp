#include "types.h"
#include "game/task.h"
#include "game/battle_fighter_task.h"

#include "game/map_select.h"

void Task_ResumeScript(Task* task);
int Battle_GetMatchupValue(int side);
void func_ov002_0235643c(void* table, int index, int x, int y, int a4, int a5,
                         int a6, int a7, int a8, int a9, int a10, int a11,
                         int a12, int side);
void Battle_StartCellEffectAtPoint(int side, int step);

/* Update of a side's fighter task. When the sprite task it last made reports
 * that its frame is done (unk_5b is 5), the next step of the side's list
 * makes the next sprite task and plays the side's second table; the script
 * resumes once the sprite task has unk_5c set. The size and offset of each step
 * scale with the fighter's size. */
void BattleFighterTask_SpawnStep(BattleFighterTask* task) {
  int step;
  int side;
  int scale;
  const BattleFighterLayout* anim;

  if (task->sprite->unk_5b == 5) {
    task->sprite->unk_5b = 0;
    step = task->step;
    if (task->hitList->count != step) {
      side = task->side;
      if (MENU->battle.unk_40[side] != 0) {
        scale = task->scale;
        anim = &task->layout->fighter;
        func_ov002_0235643c(
            MENU->battle.unk_40[task->side], Battle_GetMatchupValue(side),
            task->fighter->posX, task->fighter->posY, task->fighter->posZ,
            scale * anim->steps[step].x >> 8, scale * anim->steps[step].y >> 8,
            -1, scale, scale, (&MENU->colours[6])[side], 8, 0, side);
      }
      if (MENU->battle.unk_48[task->side] != 0) {
        Battle_StartCellEffectAtPoint(task->side, task->step);
      }
      task->step++;
    }
  }
  if (task->sprite->unk_5c != 0) {
    Task_ResumeScript((Task*)task);
  }
}
