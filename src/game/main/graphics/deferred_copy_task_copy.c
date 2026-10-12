#include "types.h"
#include "game/deferred_copy_task.h"

void Mem_Copy32(const void* src, void* dst, u32 size);
void Mem_Free(void* ptr);

/* Script command of the task from DeferredCopyTask_Start: copies the buffer to its
 * destination, then frees the buffer. */
void DeferredCopyTask_Copy(Task* task) {
  Mem_Copy32(DEFERRED_COPY_TASK_ARGS(task)->src,
             DEFERRED_COPY_TASK_ARGS(task)->dest,
             DEFERRED_COPY_TASK_ARGS(task)->size);
  Mem_Free(DEFERRED_COPY_TASK_ARGS(task)->src);
}
