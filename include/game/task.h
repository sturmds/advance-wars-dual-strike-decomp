#ifndef GAME_TASK_H
#define GAME_TASK_H

#include "types.h"

#define TASK_COUNT  80                 /* tasks in Unk_02080440.taskPool */
#define TASK_LEVELS 8                  /* top-level task lists in Unk_02080440.taskLists */

struct TaskScriptCmd;

/* A task: a callback the main loop runs every frame. Top-level tasks sit
 * in one of TASK_LEVELS lists; others hang under a parent task. */
typedef struct Task {
    const struct TaskScriptCmd *script;      /* Task_Create's first argument; tasks are found by it */
    const struct TaskScriptCmd *scriptStart; /* first command of the running script, for labels */
    const struct TaskScriptCmd *pc;          /* command that runs next */
    void (*onDestroy)(struct Task *task);    /* runs when the task is destroyed */
    void (*update)(struct Task *task); /* run each frame */
    struct Task *parent;               /* for a top-level task: its list level instead */
    struct Task *firstChild;
    struct Task *prev;                 /* previous task in the same list */
    struct Task *next;                 /* next task in the same list */
    u16 unk_24;
    u8 group;                          /* guess: Task_DestroyGroup destroys all tasks of one group */
    u8 flags;                          /* bit 1: child task, bit 3: being created */
    u8 childCount;                     /* live children the task waits on; its script holds while nonzero */
    u8 pad_29[0x52 - 0x29];
    u16 unk_52;                        /* palette slot PaletteCycle_Start animates */
    u8 pad_54[0x6c - 0x54];
} Task;

/* Task.flags bits */
#define TASK_FLAG_FREED    0x01        /* back on the free stack */
#define TASK_FLAG_CHILD    0x02        /* parent counts it in childCount */
#define TASK_FLAG_CREATING 0x08        /* not run yet */

/* One 8-byte command of a task script. */
typedef struct TaskScriptCmd {
    s16 op;                            /* TASK_CMD_* */
    s16 arg;                           /* TASK_CMD_LABEL: label id */
    u8 pad_04[4];
} TaskScriptCmd;

#define TASK_CMD_END   0               /* last command of a script */
#define TASK_CMD_LABEL 11              /* jump target for Task_JumpToLabel */

/* Runs one command of a task's script; nonzero: run the next one too. */
typedef BOOL (*TaskCommandFunc)(Task *task);

#endif
