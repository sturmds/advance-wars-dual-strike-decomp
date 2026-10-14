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

/* Like BattleFighterTask_SpawnStep, but when the side's step list has run out
 * it clears the fighter's steps[stepIndex], moves on to the next one
 * (the ring of 8 wraps) and resumes the script. The same happens once the
 * sprite task has unk_5c set. */
void BattleFighterTask_SpawnStepOrFinish(BattleFighterTask* task) {
  int step;
  int side;
  int scale;
  const BattleFighterLayout* anim;

  if (task->sprite->unk_5b == 5) {
    task->sprite->unk_5b = 0;
    step = task->step;
    if (task->hitList->count == step) {
      task->fighter->steps[task->fighter->stepIndex] = 0;
      task->fighter->stepIndex++;
      if (task->fighter->stepIndex == 8) {
        task->fighter->stepIndex = 0;
      }
      Task_ResumeScript((Task*)task);
      return;
    }
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
  if (task->sprite->unk_5c != 0) {
    task->fighter->steps[task->fighter->stepIndex] = 0;
    task->fighter->stepIndex++;
    if (task->fighter->stepIndex == 8) {
      task->fighter->stepIndex = 0;
    }
    Task_ResumeScript((Task*)task);
  }
}
