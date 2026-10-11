#ifndef SDK_SND_H
#define SDK_SND_H

#include "types.h"

/* One request for the ARM7 sound driver. The ARM9 fills it in and pushes it. */
typedef struct SndCommand {
    struct SndCommand *next;
    u32 id;
    u32 args[4];
} SndCommand;

/* Allocate and queue a command (guessed names; see func_021104b8 and func_02110470). */
SndCommand *func_021104b8(int block);
void func_02110470(SndCommand *command);
void Sound_PostCommandArgs(u32 id, u32 arg0, u32 arg1, u32 arg2, u32 arg3);

#endif
