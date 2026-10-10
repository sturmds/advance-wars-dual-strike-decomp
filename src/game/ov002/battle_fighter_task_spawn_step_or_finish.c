#include "types.h"
#include "game/task.h"
#include "game/FighterTask.h"
#include "game/map_select.h"

void Task_ResumeScript(Task *task);
int Battle_GetMatchupValue(int side);
void func_ov002_0235643c(void *table, int index, int x, int y, int a4, int a5, int a6, int a7,
                         int a8, int a9, int a10, int a11, int a12, int side);
void func_ov002_02357e5c(int side, int step);

/* Like BattleFighterTask_SpawnStep, but when the side's step list has run out
 * it clears the fighter's current byte of unk_30, moves on to the next one
 * (the ring of 8 wraps) and resumes the script. The same happens once the
 * sprite task has unk_5c set. */
void BattleFighterTask_SpawnStepOrFinish(FighterTask *task)
{
    int step;
    int side;
    int scale;
    BattleAnimData *anim;

    if (task->sprite->unk_5b == 5) {
        task->sprite->unk_5b = 0;
        step = task->step;
        if (task->steps->count == step) {
            task->fighter->unk_30[task->fighter->unk_5a_4] = 0;
            task->fighter->unk_5a_4++;
            if (task->fighter->unk_5a_4 == 8) {
                task->fighter->unk_5a_4 = 0;
            }
            Task_ResumeScript((Task *)task);
            return;
        }
        side = task->side;
        if (MENU->battle.unk_40[side] != 0) {
            scale = task->scale;
            anim = task->anim;
            func_ov002_0235643c(MENU->battle.unk_40[task->side], Battle_GetMatchupValue(side),
                                task->fighter->unk_38, task->fighter->unk_3a,
                                task->fighter->unk_3c, scale * anim->offsets[step].x >> 8,
                                scale * anim->offsets[step].y >> 8, -1, scale, scale,
                                (&MENU->colours[6])[side], 8, 0, side);
        }
        if (MENU->battle.unk_48[task->side] != 0) {
            func_ov002_02357e5c(task->side, task->step);
        }
        task->step++;
    }
    if (task->sprite->unk_5c != 0) {
        task->fighter->unk_30[task->fighter->unk_5a_4] = 0;
        task->fighter->unk_5a_4++;
        if (task->fighter->unk_5a_4 == 8) {
            task->fighter->unk_5a_4 = 0;
        }
        Task_ResumeScript((Task *)task);
    }
}
