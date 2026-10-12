#ifndef SDK_SND_H
#define SDK_SND_H

#include "types.h"
#include "sdk/os_common.h"
#include "game/sound_bank.h"
#include "game/sound_wave_arc.h"
#include "sdk/snd_seq.h"

/* One request for the ARM7 sound driver. The ARM9 fills it in and pushes it. */
typedef struct SndCommand {
    struct SndCommand *next;
    u32 id;
    u32 args[4];
} SndCommand;

/* SndCommand.id, as the ARM7 driver handles them. */
enum {
    SND_COMMAND_START_SEQ = 0,
    SND_COMMAND_STOP_SEQ = 1,
    SND_COMMAND_PAUSE_SEQ = 2,
    SND_COMMAND_SKIP_SEQ = 3,
    SND_COMMAND_PLAYER_PARAM = 4,
    SND_COMMAND_TRACK_PARAM = 5,
    SND_COMMAND_MUTE_TRACK = 6,
    SOUND_REQ_LIMIT_TRACK_CHANNELS = 7,
    SOUND_REQ_SET_PLAYER_VAR = 8,
    SOUND_REQ_SET_SHARED_VAR = 9,
    SND_COMMAND_START_TIMER = 10,
    SND_COMMAND_STOP_TIMER = 11,
    SND_COMMAND_SETUP_CHANNEL_PCM = 12,
    SND_COMMAND_SETUP_CHANNEL_PSG = 13,
    SND_COMMAND_SETUP_CHANNEL_NOISE = 14,
    SND_COMMAND_SETUP_CAPTURE = 15,
    SND_COMMAND_SETUP_ALARM = 16,
    SND_COMMAND_CHANNEL_TIMER = 17,
    SND_COMMAND_CHANNEL_VOLUME = 18,
    SND_COMMAND_CHANNEL_PAN = 19,
    SOUND_REQ_EDGE_PAN_DIP = 20,
    SND_COMMAND_MASTER_VOLUME = 21,
    SOUND_REQ_FORCE_ALL_PAN = 22,
    SOUND_REQ_MIXER_ROUTING = 23,
    SND_COMMAND_LOCK_CHANNEL = 24,
    SND_COMMAND_UNLOCK_CHANNEL = 25,
    SND_COMMAND_STOP_UNLOCKED_CHANNEL = 26,
    SND_COMMAND_SHARED_WORK = 27,
    SND_COMMAND_INVALIDATE_SEQ = 28,
    SND_COMMAND_INVALIDATE_BANK = 29,
    SND_COMMAND_INVALIDATE_WAVE = 30,
    SOUND_REQ_DUMP_DRIVER_STATE = 31
};

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
    OSAlarm osAlarm;
} SndArm7Alarm;

#define SOUND_TIMER_SLOTS_ARM7 8

/* The driver's state block; its first 0x1180 bytes are copied into the shared work. */
typedef struct SndArm7Work {
    SndExChannel channels[16];
    SndPlayer players[SND_PLAYER_COUNT];
    SndTrack tracks[SEQ_TRACKS_SHARED_TOTAL];
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
void SndArm7_AlarmHandler(void *arg);

/* The ARM7's pointer to the ARM9's shared work; NULL until the ARM9 sends it. */
extern SndSharedWork *data_arm7_0380808c;

/* Thread of the ARM7 sound driver and the alarm that wakes it every 2728 ticks (about 5.2 ms). */

/* Messages the driver thread waits for. */
enum {
    SOUND_THREAD_MSG_TICK = 1, /* the interval alarm fired: run sequences and envelopes */
    SOUND_THREAD_MSG_NEW_REQUESTS = 2    /* the ARM9 sent commands */
};

extern OSMessageQueue data_arm7_03807b90; /* messages to the driver thread */
extern OSAlarm data_arm7_03807bc4;        /* the interval alarm */

void SND_InitIntervalTimer(void);
void SND_StartIntervalTimer(void);
void SND_StopIntervalTimer(void);
OSMessage SND_WaitForIntervalTimer(void);
void SND_SendWakeupMessage(void);

/* PXI receive callback of SOUND_FIFO_TAG: queues a command list from the ARM9, or wakes the driver for 0. */
void SndArm7_OnPxiReceive(u32 tag, u32 data, int err);

/* Driver commands and work. */
void *SndArm7_GetBankWaveData(SoundBank *bank, int waveArcSlot, u32 waveNo);
void SndArm7_Init(void);
void SndArm7_InitAlarms(void);
void SndArm7_InitSeq(void);
void SndArm7_InvalidateBank(u32 start, u32 end);
void SndArm7_InvalidateSeq(u32 start, u32 end);
BOOL SndArm7_NoteOn(SndExChannel *channel, int key, int velocity, s32 length, SoundBank *bank, SoundBankNote *note);
void SndArm7_PauseSeq(int playerNo, BOOL pause);
void SndArm7_PrepareSeq(int playerNo, u8 *seqBase, u32 seqOffset, SoundBank *bank);
void SndArm7_ProcessCommands(void);
BOOL SndArm7_ReadInstData(SoundBank *bank, int programNo, int key, SoundBankNote *note);
void SndArm7_SeqMain(BOOL stepSequence);
void SndArm7_SetChannelPan(u32 channelMask, int pan);
void SndArm7_SetChannelTimer(u32 channelMask, int timer);
void SndArm7_SetChannelVolume(u32 channelMask, int volume, int shift);
void SndArm7_SetPlayerParam(int playerNo, int offset, u32 value, int size);
void SndArm7_SetPlayerVariable(int playerNo, int varNo, s16 value);
void SndArm7_SetTrackChannelMask(int playerNo, u32 trackMask, u32 channelMask);
void SndArm7_SetTrackMute(int playerNo, u32 trackMask, int mute);
void SndArm7_SetTrackParam(int playerNo, u32 trackMask, int offset, u32 value, int size);
void SndArm7_SetupAlarm(int alarmNo, u64 start, u64 period, int tag);
void SndArm7_SkipSeq(int playerNo, u32 ticks);
void SndArm7_StartTimer(u32 channelMask, u32 captureMask, u32 alarmMask, u32 flags);
void SndArm7_StopAlarm(int alarmNo);
void SndArm7_StopSeq(int playerNo);
void SndArm7_StopTimer(u32 channelMask, u32 captureMask, u32 alarmMask, int hold);
void SndArm7_UpdateSharedWork(void);
void SndArm7_WriteSnapshot(SndArm7Snapshot *snapshot);

/* Setup, capture and the pitch and volume tables. */
u16 Sound_DecibelToHwVolume_Arm7(int volume);
u16 Sound_PitchShiftTimer_Arm7(int timer, int pitch);
void SND_CreateThread(u32 priority);
void Sound_StartDriver_Arm7(u32 priority);
BOOL SND_IsCaptureActive(int capture);
void SND_SetupCapture(int capture, int format, void *buffer, int length, BOOL loop, int in, int out);

/* Driver thread helpers. */
u16 SndDrv_NextRandom(void);
void SndDrv_OnIntervalAlarm(void *arg);
s8 SndDrv_SineWave(int phase);
void SndDrv_Thread(void *arg);

/* Channels. */
SndExChannel *SndExChannel_Alloc(u32 channelMask, int priority, BOOL strong, SndExChannelCallback callback, void *callbackArg);
u16 SndExChannel_CalcDecayRate(int setting);
int SndExChannel_CompareVolume(SndExChannel *a, SndExChannel *b);
u32 SndExChannel_GetLockedChannels(u32 flags);
void SndExChannel_Init(SndExChannel *channel, SndExChannelCallback callback, void *callbackArg, int priority);
void SndExChannel_InvalidateWave(const void *start, const void *end);
void SndExChannel_LockChannels(u32 channelMask, u32 flags);
void SndExChannel_SetAttack(SndExChannel *channel, int attack);
void SndExChannel_Start(SndExChannel *channel, int length);
BOOL SndExChannel_StartNoise(SndExChannel *channel, int length);
BOOL SndExChannel_StartPcm(SndExChannel *channel, const SndWaveParam *wave, const void *data, int length);
BOOL SndExChannel_StartPsg(SndExChannel *channel, int duty, int length);
void SndExChannel_StopUnlockedChannels(u32 channelMask, u32 flags);
void SndExChannel_UnlockChannels(u32 channelMask, u32 flags);
int SndExChannel_UpdateEnvelope(SndExChannel *channel, BOOL update);
int SndExChannel_UpdateLfo(SndExChannel *channel, BOOL update);
int SndExChannel_UpdateSweep(SndExChannel *channel, BOOL update);
void SndExChannel_InitAll(void);
void SndExChannel_Update(void);
void SndExChannel_Main(BOOL periodic);

/* LFOs. */
int SndLfo_GetValue(SndChannelLfo *lfo);
void SndLfo_Update(SndChannelLfo *lfo);

/* Players. */
void SndPlayer_Advance(SndPlayer *player);
void SndPlayer_CloseTrack(SndPlayer *player, int trackNo);
SndTrack *SndPlayer_GetTrack(SndPlayer *player, int trackNo);
s16 *SndPlayer_GetVariablePtr(SndPlayer *player, int varNo);
void SndPlayer_Setup(SndPlayer *player, SoundBank *bank);
BOOL SndPlayer_StepTracks(SndPlayer *player, int play);
void SndPlayer_Stop(SndPlayer *player);
void SndPlayer_UpdateChannels(SndPlayer *player);

/* Sequence data cache. */
void SndSeq_FillCache(u8 *address);
u8 SndSeq_ReadByte(u8 *address);

/* Tracks. */
int SndTrack_Alloc(void);
void SndTrack_ChannelCallback(SndExChannel *channel, int status, void *data);
void SndTrack_Close(SndTrack *track, SndPlayer *player);
void SndTrack_FreeChannels(SndTrack *track);
void SndTrack_Init(SndTrack *track);
void SndTrack_NoteOn(SndTrack *track, SndPlayer *player, int key, int velocity, s32 length);
int SndTrack_ReadArg(SndTrack *track, SndPlayer *player, int type);
u16 SndTrack_ReadU16(SndTrack *track);
u32 SndTrack_ReadU24(SndTrack *track);
u32 SndTrack_ReadVarLength(SndTrack *track);
void SndTrack_ReleaseChannels(SndTrack *track, SndPlayer *player, int release);
int SndTrack_Step(SndTrack *track, SndPlayer *player, int trackNo, int play);
void SndTrack_UpdateChannels(SndTrack *track, SndPlayer *player, BOOL release);

/* Taken around reads of sound data the ARM9 may change; both are empty in this build. */
void SNDi_LockMutex(void);
void SNDi_UnlockMutex(void);

#endif
