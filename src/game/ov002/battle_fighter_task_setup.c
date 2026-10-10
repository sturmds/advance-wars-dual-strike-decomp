#include "types.h"
#include "game/task.h"
#include "game/FighterTask.h"
#include "game/map_select.h"

void Task_Destroy(Task *task);
int Battle_GetMatchupNibble1(int side);

#define FIGHTER_ENTRY(task) \
    (MENU->battle.unk_310[(task)->fighter->side][(task)->fighter->unk_59_5])

/* Start of a side's fighter task: finds the fighter, the side's step list
 * and animation data, drops the fighter's old sprite task and makes a new
 * one from the side's sprite table. Partial: 96%; the original reads
 * task->fighter twice where this reads it once. */
void BattleFighterTask_Setup(FighterTask *task)
{
    BattleFighter *fighter;
    BOOL left;
    int unk_58;
    int unk_56;
    int side;
    int scale;

    task->fighter = (BattleFighter *)MENU->battle.unk_310[task->side][0].unk_00;
    task->steps = &MENU->battle.hitLists[task->side];
    task->anim = (BattleAnimData *)MENU->battle.unk_27c[task->side]->cellPositions;
    if (MENU->battle.unk_30[task->side] != 0) {
        if (FIGHTER_ENTRY(task).sprites[1] != NULL) {
            Task_Destroy((Task *)FIGHTER_ENTRY(task).sprites[1]);
            FIGHTER_ENTRY(task).sprites[1] = NULL;
        }
        fighter = task->fighter;
        left = task->fighter->side == 0 ? TRUE : FALSE;
        scale = fighter->unk_44;
        side = fighter->side;
        unk_58 = left ? 0 : 2;
        unk_56 = left ? 0 : 2;
        FIGHTER_ENTRY(task).sprites[1] = BattleSpriteTask_Create(
            (const u32 *)MENU->battle.unk_30[task->fighter->side],
            task->steps->count + Battle_GetMatchupNibble1(task->side) - 1, fighter->unk_38,
            fighter->unk_3a, fighter->unk_3c, 0, fighter->unk_46, 0, scale, scale,
            (&MENU->colours[2])[side], unk_56, 0, side, fighter->unk_58_0, fighter->unk_3e,
            fighter->unk_40, fighter->unk_42, unk_58);
        task->sprite = FIGHTER_ENTRY(task).sprites[1];
    }
    task->step = 0;
}
