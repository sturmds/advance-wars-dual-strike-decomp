#include "types.h"
#include "game/script_slot.h"
#include "game/task.h"

s8 ScriptSlot_StopByScript(
    const void*
        descriptor); /* finds the slot (of 30) using descriptor, calls ScriptSlot_Stop on it */
void Task_DestroyByScript(
    const void*
        descriptor); /* calls Task_DestroyVisitor on each of 80 tasks running descriptor */

extern ScriptCommand data_021583b4[];
extern TaskScriptCmd data_021587a8[];

/* Runs before a hint or terms script starts: passes two descriptors to
 * the two task slot systems, probably to stop the tasks using them. */
void func_0201e954(void) {
  ScriptSlot_StopByScript(data_021583b4);
  Task_DestroyByScript(data_021587a8);
}
