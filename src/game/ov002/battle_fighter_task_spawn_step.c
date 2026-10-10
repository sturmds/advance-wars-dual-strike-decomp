#include "types.h"
#include "game/task.h"
#include "game/FighterTask.h"

extern MenuState gMenuState;

/* Needed to match: the original adds the table offsets in the load, on top of
 * the address of gMenuState. */
typedef struct SideTables {
    u8 pad_00[0x0c];
    u16 tint[2];
    u8 pad_10[0x40 - 0x10];
    u32 table[2];
    u32 unk_48[2];
} SideTables;
#define TABLES ((SideTables *)&gMenuState)

void Task_ResumeScript(Task *task);
int Battle_GetMatchupValue(int side);
void func_ov002_0235643c(u32 table, int index, int x, int y, int a4, int a5, int a6, int a7,
                         int a8, int a9, int a10, int a11, int a12, int side);
void func_ov002_02357e5c(int side, int step);

/* Update of a side's fighter task. When the sprite task it last made reports
 * that its frame is done (unk_5b is 5), the next step of the side's list
 * makes the next sprite task and plays the side's second table; the script
 * resumes once the sprite task has unk_5c set. The size and offset of each step
 * scale with the fighter's size. */
void BattleFighterTask_SpawnStep(FighterTask *task)
{
    int step;
    int side;
    int scale;
    BattleAnimData *anim;

    if (task->sprite->unk_5b == 5) {
        task->sprite->unk_5b = 0;
        step = task->step;
        if (task->steps->count != step) {
            side = task->side;
            if (TABLES->table[side] != 0) {
                scale = task->scale;
                anim = task->anim;
                func_ov002_0235643c(TABLES->table[task->side], Battle_GetMatchupValue(side),
                                    task->fighter->unk_38, task->fighter->unk_3a,
                                    task->fighter->unk_3c,
                                    scale * anim->offsets[step].x >> 8,
                                    scale * anim->offsets[step].y >> 8, -1, scale, scale,
                                    TABLES->tint[side], 8, 0, side);
            }
            if (TABLES->unk_48[task->side] != 0) {
                func_ov002_02357e5c(task->side, task->step);
            }
            task->step++;
        }
    }
    if (task->sprite->unk_5c != 0) {
        Task_ResumeScript((Task *)task);
    }
}
