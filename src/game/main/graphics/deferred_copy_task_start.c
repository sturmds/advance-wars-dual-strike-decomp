#include "types.h"
#include "game/deferred_copy_task.h"

void DeferredCopyTask_Copy(void);

/* runs DeferredCopyTask_Copy */
static TaskScriptCmd gDeferredCopyTaskScript[] = {
#include "data/main/gDeferredCopyTaskScript.inc.c"
};
u8 gCoPictureSprite[] = {
#include "data/main/gCoPictureSprite.inc.c"
};

/* Starts a task that copies size bytes from the heap buffer src to dest and
 * frees src (see DeferredCopyTask_Copy). */
void DeferredCopyTask_Start(void* src, void* dest, u32 size) {
  Task* task = Task_Create(gDeferredCopyTaskScript, TASK_LIST(0));

  DEFERRED_COPY_TASK_ARGS(task)->src = src;
  DEFERRED_COPY_TASK_ARGS(task)->dest = dest;
  DEFERRED_COPY_TASK_ARGS(task)->size = size;
}
