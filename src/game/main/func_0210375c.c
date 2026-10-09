#include "types.h"

/* Starts a task that runs `script`. `parentOrList` below 8 picks one of the
 * top-level task lists; anything else is the parent task. */
void *Task_Create(const void *script, void *parentOrList);

extern const u32 data_0216d8ac[];

/* Starts the data_0216d8ac task on top-level task list 3. */
void func_0210375c(void) {
    Task_Create(data_0216d8ac, (void *)3);
}
