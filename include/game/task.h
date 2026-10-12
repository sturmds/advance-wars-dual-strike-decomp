#ifndef GAME_TASK_H
#define GAME_TASK_H

#include "types.h"

#define TASK_COUNT 80 /* tasks in Unk_02080440.taskPool */
#define TASK_LEVELS 8 /* top-level task lists in Unk_02080440.taskLists */

struct TaskScriptCmd;

/* A task: a callback the main loop runs every frame. Top-level tasks sit
 * in one of TASK_LEVELS lists; others hang under a parent task. */
typedef struct Task {
  const struct TaskScriptCmd*
      script; /* Task_Create's first argument; tasks are found by it */
  const struct TaskScriptCmd*
      scriptStart; /* first command of the running script, for labels */
  const struct TaskScriptCmd* pc;       /* command that runs next */
  void (*onDestroy)(struct Task* task); /* runs when the task is destroyed */
  void (*update)(struct Task* task);    /* run each frame */
  struct Task* parent; /* for a top-level task: its list level instead */
  struct Task* firstChild;
  struct Task* prev; /* previous task in the same list */
  struct Task* next; /* next task in the same list */
  s16 unk_24; /* frames left for TaskCmd_WaitFrames; its low byte is TaskCmd_StartEventScript's argument */
  u8 group;   /* guess: Task_DestroyGroup destroys all tasks of one group */
  u8 flags;   /* bit 1: child task, bit 3: being created */
  u8 childCount; /* live children the task waits on; its script holds while nonzero */
  u8 pad_29;
  u16 waitFrames; /* func_02080db8 counts frames here */
  u8 pad_2c[0x2e - 0x2c];
  u8 unk_2e; /* link slot of the player a reaction task shows (LinkReaction_Update) */
  u8 pad_2f[0x32 - 0x2f];
  u16 unk_32;   /* frame counter of func_02081f6c */
  u16 unk_34;   /* frame counter of func_02081900 */
  u16 unk_36;   /* frame counter of func_020824e4 */
  void* unk_38; /* data some scripts get from their creator, beside unk_4c */
  void (*unk_3c)(void); /* set by EventCmd_StartTextMapTask */
  u16 unk_40;           /* palette slot << 12 (tile map entry bits), same */
  u8 pad_42[0x44 - 0x42];
  s16 unk_44; /* step limit of the screen shake task (ScreenShake_Update) */
  u8 pad_46[0x4c - 0x46];
  void*
      unk_4c; /* data some scripts get from their creator; a MapUnit * for the battle tasks of ActingUnit_StartAttack */
  u8 pad_50[0x52 - 0x50];
  u16 unk_52; /* palette slot PaletteCycle_Start animates */
  int unk_54; /* frame count of the value ease (EaseTask_Update) */
  int unk_58; /* number of sprites the fade task of ScreenFadeSprites_Update draws */
  u8 unk_5c; /* bit 2 set by Sprite_CreateTask */
  u8 pad_5d[0x64 - 0x5d];
  s16 unk_64;
  s16 unk_66;
  s16 unk_68;
  s16 unk_6a; /* number of players on the battle stats screen (see ResultStats_NextPlayer) */
} Task;

/* Task.flags bits */
#define TASK_FLAG_FREED 0x01    /* back on the free stack */
#define TASK_FLAG_CHILD 0x02    /* parent counts it in childCount */
#define TASK_FLAG_CREATING 0x08 /* not run yet */

/* One 8-byte command of a task script. */
typedef struct TaskScriptCmd {
  s16 op;  /* TASK_CMD_* */
  s16 arg; /* TASK_CMD_LABEL: label id */
  u32 value; /* second argument of commands that take one (a sound id, a script pointer) */
} TaskScriptCmd;

#define TASK_CMD_END 0    /* last command of a script */
#define TASK_CMD_LABEL 11 /* jump target for Task_JumpToLabel */

/* Runs one command of a task's script; nonzero: run the next one too. */
typedef BOOL (*TaskCommandFunc)(Task* task);

/* Task_Create's second argument when it names a top-level list (below
 * TASK_LEVELS) rather than a parent task. */
#define TASK_LIST(level) ((void*)(level))

Task* Task_Create(const TaskScriptCmd* script, void* parentOrList);
Task* Task_CreateChild(const TaskScriptCmd* script, Task* parent);

#endif
