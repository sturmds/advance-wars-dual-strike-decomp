#include "types.h"

/* Starts a task that runs `script` as a child of `parent`; returns the task. */
void *Task_CreateChild(const void *script, void *parent);

/* Script of (opcode, argument) pairs. */
extern const u32 gCoNoPowerScript[];

/* Power routine for COs whose power has no extra effect: it only runs the
 * common power script. */
void CoNoPower(void *parent) {
    Task_CreateChild(gCoNoPowerScript, parent);
}
