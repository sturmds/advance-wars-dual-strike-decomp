#include "types.h"
#include "game/script_slot.h"
#include "game/unit_show_task.h"

extern ScriptCommand data_02169e38[];

s8 ScriptSlot_Find(const ScriptCommand *script);

/* State handler: waits until the slot script data_02169e38 has stopped, then moves. */
void UnitShowTask_StateScript(UnitShowTask *task)
{
    if (ScriptSlot_Find(data_02169e38) == -1) {
        task->unk_35 = UNIT_SHOW_STATE_MOVE;
    }
}
