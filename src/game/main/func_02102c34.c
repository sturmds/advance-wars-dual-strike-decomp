#include "types.h"

/* Starts a task that runs `script`. `parentOrList` below 8 picks one of the
 * top-level task lists; anything else is the parent task. */
void *Task_Create(const void *script, void *parentOrList);

extern const u32 data_0216d644[];

/* Starts the data_0216d644 task on top-level task list 3. */
void func_02102c34(void) {
    Task_Create(data_0216d644, (void *)3);
}
