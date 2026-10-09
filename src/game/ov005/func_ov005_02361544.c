#include "types.h"

/* Starts a task that runs `script` as a child of `parent`; returns the task. */
void *Task_CreateChild(const void *script, void *parent);

/* Script whose main step is func_ov005_02361598. */
extern const u32 data_ov005_0236abfc[];

void func_ov005_02361544(void *parent) {
    Task_CreateChild(data_ov005_0236abfc, parent);
}
