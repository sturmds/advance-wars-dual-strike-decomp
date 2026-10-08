#include "types.h"

/* Starts a task that runs `script` as a child of `parent`; returns the task. */
void *func_02023a0c(const void *script, void *parent);

/* Starts a task that runs `script`. `parentOrList` below 8 picks one of the
 * top-level task lists; anything else is the parent task. */
void *func_02023a58(const void *script, void *parentOrList);

/* Script of (opcode, argument) pairs. */
extern const u32 gExMachinaScript[];

/* Von Bolt's Super CO Power. Without a parent task the script goes on
 * top-level task list 3. */
void ExMachina(void *parent) {
    if (parent != NULL) {
        func_02023a0c(gExMachinaScript, parent);
    } else {
        func_02023a58(gExMachinaScript, (void *)3);
    }
}
