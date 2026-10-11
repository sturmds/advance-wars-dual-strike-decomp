#ifndef SDK_SND_H
#define SDK_SND_H

#include "types.h"

/* One request for the ARM7 sound driver. The ARM9 fills it in and pushes it. */
typedef struct SndCommand {
    struct SndCommand *next;
    u32 id;
    u32 args[4];
} SndCommand;

/* Command queue state of the sound driver. The roles are read from the code. */
extern int data_022a9c50;      /* SND_Init has run */
extern u8 data_022a9c54[];     /* lock around the command lists */
extern SndCommand *data_022a9c68; /* free list head */
extern u32 data_022a9c6c;      /* tag of the last finished command batch */
extern SndCommand *data_022a9c70; /* queued, unflushed commands: head */
extern SndCommand *data_022a9c74; /* queued, unflushed commands: tail */
extern SndCommand *data_022a9c78; /* free list tail */
extern int data_022a9c80;      /* next slot in data_022a9c8c */
extern int data_022a9c84;      /* batches sent and not yet finished */
extern u32 data_022a9c88;      /* tag of the newest flushed batch */
extern SndCommand *data_022a9c8c[9]; /* first command of each batch in flight */
extern SndCommand data_022a9f40[256]; /* command pool */

void SND_Init(void);
SndCommand *SND_PopFreeCommand(void);

/* Allocate and queue a command (guessed names; see func_021104b8 and func_02110470). */
SndCommand *func_021104b8(int block);
void func_02110470(SndCommand *command);
void Sound_PostCommandArgs(u32 id, u32 arg0, u32 arg1, u32 arg2, u32 arg3);

#endif
