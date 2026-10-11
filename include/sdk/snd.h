#ifndef SDK_SND_H
#define SDK_SND_H

#include "types.h"

/* One request for the ARM7 sound driver. The ARM9 fills it in and pushes it. */
typedef struct SndCommand {
    struct SndCommand *next;
    u32 id;
    u32 args[4];
} SndCommand;

/* Status block the ARM7 sound driver writes and the ARM9 reads after invalidating its cache lines. */
typedef struct SndPlayerInfo {
    s16 variable[16];
    u32 tickCounter;
} SndPlayerInfo;

typedef struct SndSharedWork {
    u32 finishCommandTag;
    u32 playerStatus;
    u16 channelStatus;
    u16 captureStatus;
    u8 pad_0C[20];
    SndPlayerInfo player[16];
    s16 globalVariable[16];
} SndSharedWork;

/* One software alarm slot: the handler runs when the ARM7 reports the alarm, if tag still matches. */
typedef void (*SndAlarmHandler)(u32 arg);

typedef struct SndAlarm {
    SndAlarmHandler handler;
    u32 arg;
    u8 tag;
    u8 pad_09[3];
} SndAlarm;

#define SND_ALARM_COUNT 8

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

#define SOUND_FIFO_TAG 7 /* FIFO tag of the sound driver */
#define SOUND_BATCHES_IN_FLIGHT_MAX 8   /* flushed batches the ARM7 may hold at once */
#define SOUND_SEND_WAIT_IF_FULL 1   /* SND_FlushCommand: wait instead of failing */
#define SOUND_SEND_THEN_PING 2    /* SND_FlushCommand: wake the ARM7 after sending */

void SND_Init(void);
SndCommand *SND_PopFreeCommand(void);
void Sound_PingArm7Driver(void);
int Sound_PendingCommandCount(void);
int Sound_FreeCommandCount(void);
int SND_IsFinishedCommandTag(u32 tag);
void Sound_WaitForArm7Listener(void);
void SoundAlarm_OnArm7Message(int tag, u32 data, int err);

/* Allocate and queue a command (SND_AllocCommand is the allocator, guessed name SND_AllocCommand). */
SndCommand *SND_AllocCommand(int block);
/* Take one reply batch off the ring and return its commands to the free list; NULL if none and not blocking. */
SndCommand *SND_RecvCommandReply(int block);
void Sound_LinkCommandAtTail(SndCommand *command);
void Sound_PostCommandArgs(u32 id, u32 arg0, u32 arg1, u32 arg2, u32 arg3);
void SoundAlarm_RunIfTagMatches(int alarmId);
/* Sets the slot's handler and argument, bumps its tag and returns the new tag. */
u32 SoundAlarm_AssignHandler(u32 alarmNo, SndAlarmHandler handler, u32 arg);
/* Makes alarm ids already in flight for this slot stale. */
void SoundAlarm_BumpTag(int alarmNo);
void SoundAlarm_ClearAll(void);
void Sound_ResetArm7StatusBlock(SndSharedWork *work);
/* Reads the ARM7 shared work after invalidating its cache lines. */
u32 Sound_LastDoneCommandTag(void);

#endif
