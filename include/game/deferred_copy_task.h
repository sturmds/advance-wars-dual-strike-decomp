#ifndef GAME_DEFERRED_COPY_TASK_H
#define GAME_DEFERRED_COPY_TASK_H

#include "types.h"
#include "game/task.h"

/* What DeferredCopyTask_Start stores in Task.pad_2c onwards. */
typedef struct DeferredCopyTaskArgs {
  void* src; /* heap buffer; freed after the copy */
  void* dest;
  u32 size;
} DeferredCopyTaskArgs;

#define DEFERRED_COPY_TASK_ARGS(task) ((DeferredCopyTaskArgs*)(task)->pad_2c)

#endif
