#include "types.h"
#include "game/power_hit_effect_task.h"
#include "game/task.h"
#include "game/script_slot.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

#define STATE (data_027c027c->unk_48[data_027c0284])

void Task_ResumeScript(Task *task);
s16 ScriptSlot_Find(const ScriptCommand *script);
void func_020e1bc8(int x, int y, int kind);

extern ScriptCommand data_02169e38[];
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Script step of the tile effect task: waits while the map scroll script
 * runs, then starts the effect on the task's tile and resumes the script. */
void PowerHitEffectTask_Play(PowerHitEffectTask *task)
{
    if (ScriptSlot_Find(data_02169e38) != -1) {
        return;
    }
    /* needed to match: the callee gets the tile as bytes */
    func_020e1bc8((u8)task->tileX, (u8)task->tileY, STATE->players[task->player].front.unk_12);
    Task_ResumeScript((Task *)task);
}
