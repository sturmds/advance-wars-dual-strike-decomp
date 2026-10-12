#include "types.h"
#include "game/task.h"

/* Script of (opcode, argument) pairs. */
extern const TaskScriptCmd gCoNoPowerScript[];

/* Power routine for COs whose power has no extra effect: it only runs the
 * common power script. */
void CoNoPower(void* parent) { Task_CreateChild(gCoNoPowerScript, parent); }
