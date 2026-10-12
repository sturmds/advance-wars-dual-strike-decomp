#include "types.h"
#include "game/task.h"

/* Script of (opcode, argument) pairs. It runs step CoveringFire_StartBlast three
 * times, each after a different step; probably one round per missile. */
extern const TaskScriptCmd gCoveringFireScript[];

/* Rachel's Super CO Power. */
void CoveringFire(void* parent) {
  Task_CreateChild(gCoveringFireScript, parent);
}
