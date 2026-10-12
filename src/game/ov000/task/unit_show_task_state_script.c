#include "types.h"
#include "game/script_slot.h"
#include "game/unit_show_task.h"

extern ScriptCommand gMapScrollScript[];

s8 ScriptSlot_Find(const ScriptCommand* script);

/* State handler: waits until the slot script gMapScrollScript has stopped, then moves. */
void UnitShowTask_StateScript(UnitShowTask* task) {
  if (ScriptSlot_Find(gMapScrollScript) == -1) {
    task->state = UNIT_SHOW_STATE_MOVE;
  }
}
