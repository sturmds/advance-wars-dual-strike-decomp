#include "types.h"

/* Starts a task that runs `script` as a child of `parent`; returns the task. */
void *Task_CreateChild(const void *script, void *parent);

/* Script of (opcode, argument) pairs. It runs step func_020e44a4 three
 * times, each after a different step; probably one round per missile. */
extern const u32 gCoveringFireScript[];

/* Rachel's Super CO Power. */
void CoveringFire(void *parent) {
    Task_CreateChild(gCoveringFireScript, parent);
}
