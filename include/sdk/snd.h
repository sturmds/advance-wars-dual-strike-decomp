#ifndef SDK_SND_H
#define SDK_SND_H

#include "types.h"
#include "sdk/os.h"
#include "game/sound_bank.h"
#include "game/sound_wave_arc.h"

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
u16 SND_GetChannelStatus(void);
u32 SND_GetPlayerStatus(void);
/* Maps a volume from -723 to 0 to the hardware volume (low byte) and volume shift (bits 8-9). */
u16 SND_CalcChannelVolume(int volume);
/* Reads the ARM7 shared work after invalidating its cache lines. */
u32 Sound_LastDoneCommandTag(void);

/* Wave archive table access (SNDWaveArc). Entries hold the wave's address once it is loaded. */
void *SND_GetWaveDataAddress(SoundWaveArc *arc, u32 waveNo);
void SND_SetWaveDataAddress(SoundWaveArc *arc, u32 waveNo, void *wave);

/* Bank and wave archive linking: a bank keeps four links to wave archives. */
void SND_AssignWaveArc(SoundBank *bank, int index, SoundWaveArc *arc);
void SND_DestroyBank(SoundBank *bank);
void SND_DestroyWaveArc(SoundWaveArc *arc);

/* Note walking of a bank. */
SoundBankIter SoundBank_StartIter(void);
BOOL SoundBank_NextNote(SoundBank *bank, SoundBankNote *note, SoundBankIter *iter);

/* ARM7 side of the sound driver. */

/* One alarm slot of the ARM7 driver. The alarm handler reports slot | tag << 8 to the ARM9. */
typedef struct SndArm7Alarm {
    u8 active;
    u8 tag;
    u8 pad_02[2];
    u64 start; /* delay before the first firing, in ticks */
    u64 period; /* 0 for a one-shot alarm */
    u8 osAlarm[44];
} SndArm7Alarm;

#define SOUND_TIMER_SLOTS_ARM7 8

/* The driver's state block; its first 0x1180 bytes are copied into the shared work. */
typedef struct SndArm7Work {
    u8 pad_0000[0xf80];
    SndArm7Alarm alarms[SOUND_TIMER_SLOTS_ARM7];
} SndArm7Work;

/* Sound capture control registers (REG_SNDCAP0CNT and REG_SNDCAP1CNT). */
#define AUDIO_RECORD0_CNT (*(u8 *)0x04000508)
#define AUDIO_RECORD1_CNT (*(u8 *)0x04000509)

/* Copy of the driver's state, written to a buffer the ARM9 passed in. */
typedef struct SndArm7Snapshot {
    SndArm7Work work;
    u32 channelControl[16]; /* SOUNDxCNT of each channel */
    SndArm7Work *driver;
    u32 unk_11C4;
} SndArm7Snapshot;

extern SndArm7Work data_arm7_03808090;
extern SndArm7Alarm data_arm7_03809010[SOUND_TIMER_SLOTS_ARM7]; /* same memory as data_arm7_03808090.alarms */

/* Command lists the ARM9 sends are queued here for the driver thread. */
extern OSMessageQueue data_arm7_03809210;
extern OSMessage data_arm7_03809224[8];

void SndArm7_StartAlarm(int alarmNo);

/* The ARM7's pointer to the ARM9's shared work; NULL until the ARM9 sends it. */
extern SndSharedWork *data_arm7_0380808c;

#endif
