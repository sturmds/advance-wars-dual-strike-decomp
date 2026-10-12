#ifndef SDK_SND_SEQ_H
#define SDK_SND_SEQ_H

#include "types.h"

/* Sequence player of the ARM7 sound driver: 16 players, each with up to 16 tracks. */

#define SND_PLAYER_COUNT 16
#define SND_TRACK_COUNT 16

typedef struct SndExChannel SndExChannel;

/* Low frequency oscillator settings. */
typedef struct SndLfo {
    u8 target; /* SND_LFO_PITCH, SND_LFO_VOLUME or SND_LFO_PAN */
    u8 speed;
    u8 depth;
    u8 range;
    u16 delay;
} SndLfo;

typedef struct SndTrack {
    u8 active : 1;
    u8 noteWait : 1;
    u8 mute : 1;
    u8 tie : 1;
    u8 noteFinished : 1;
    u8 portamento : 1;
    u8 compare : 1;
    u8 channelMaskSet : 1; /* only the channels in channelMask may be used */
    u8 panRange;
    u16 program;
    u8 volume;
    u8 volume2;
    u8 pitchBend;
    u8 bendRange;
    u8 pan;
    u8 unk_09;
    u16 transpose;
    u16 unk_0c;
    u8 attack;
    u8 decay;
    u8 sustain;
    u8 release;
    u8 priority;
    u8 unk_13;
    u8 portamentoKey;
    u8 portamentoTime;
    u16 sweepPitch;
    SndLfo lfo;
    u16 channelMask;
    u32 unk_20;
    u8 *base;   /* start of the track's sequence data */
    u8 *cursor; /* next byte to read */
    u8 pad_2c[0x0f];
    u8 unk_3b;
    SndExChannel *channels;
} SndTrack;

/* How a sequence command stores one of its arguments. */
enum {
    SEQ_ARG_FROM_U8,
    SEQ_ARG_FROM_U16,
    SEQ_ARG_FROM_VARINT,
    SEQ_ARG_FROM_RANGE, /* two 16-bit limits, the value is picked between them */
    SEQ_ARG_FROM_PLAYER_VAR /* index of a player variable */
};

typedef struct SndPlayer {
    u8 active;
    u8 prepared;
    u8 number;
    u8 pad_03;
    u8 priority;
    u8 volume;
    s16 extFader;
    u8 tracks[SND_TRACK_COUNT]; /* index of each track, 0xFF when unused */
    u16 tempo;
    u16 tempoRatio;
    u16 tempoCounter;
    u16 pad_1e;
    u32 bank; /* address of the sound bank the player uses */
} SndPlayer;

extern SndPlayer data_arm7_038085d0[SND_PLAYER_COUNT];

/* Tracks are taken from one pool shared by all players. */
#define SEQ_TRACKS_SHARED_TOTAL 32
extern SndTrack data_arm7_03808810[SEQ_TRACKS_SHARED_TOTAL];

/* Sequence data is read through a 16-byte window that is copied from the data when a read falls outside it. */
typedef struct SndSeqCache {
    u8 *start;
    u8 *end;
    u32 words[4]; /* the 16 bytes at start */
} SndSeqCache;

extern SndSeqCache data_arm7_03808074;

enum {
    SND_LFO_PITCH = 0,
    SND_LFO_VOLUME = 1,
    SND_LFO_PAN = 2
};

/* A running oscillator: its settings and where it is in its cycle. */
typedef struct SndChannelLfo {
    SndLfo param;
    u16 delayCounter;
    u16 counter;
} SndChannelLfo;

/* How a PCM sample plays: format, loop and the channel timer for its rate. */
typedef struct SndWaveParam {
    u8 format;
    u8 loop;
    u16 rate;
    u16 timer;
    u16 loopStart;
    u32 loopLength;
} SndWaveParam;

/* SndExChannel.type */
#define VOICE_TYPE_SAMPLED 0
#define VOICE_TYPE_SQUARE_WAVE 1
#define VOICE_TYPE_NOISE 2

/* SndExChannel.envStatus */
#define ENV_STAGE_RISE 0
#define ENV_STAGE_FALL 1
#define ENV_STAGE_HOLD 2
#define ENV_STAGE_FADE 3

/* Reason passed to a channel's callback. */
#define VOICE_STOLEN 0   /* taken by a request of higher priority */
#define VOICE_DONE 1

/* SndExChannel.unk_03_3: hardware updates the driver still has to make for the channel. */
#define VOICE_DIRTY_STOP 2
#define VOICE_DIRTY_TIMER 4
#define VOICE_DIRTY_VOLUME 8

typedef void (*SndExChannelCallback)(SndExChannel *channel, int status, void *arg);

/* A hardware channel as the driver's channel allocator sees it. Only the fields the code read so far are named. */

struct SndExChannel {
    u8 id;   /* hardware channel number */
    u8 type; /* VOICE_* */
    u8 envStatus;
    u8 active : 1;
    u8 start : 1;
    u8 autoSweep : 1; /* advance the sweep on each periodic update */
    u8 unk_03_3 : 5;
    u8 panRange;
    u8 originalKey;
    s16 userDecay2;
    u8 key;
    u8 velocity;
    s8 initPan;
    s8 userPan;
    s16 userDecay;
    s16 userPitch;
    s32 envAttenuation; /* envelope level, in 1/128 steps of SND_CalcChannelVolume's volume; -723 * 128 is silent */
    s32 sweepCounter;
    s32 sweepLength;
    u8 attack; /* multiplier applied to the level each tick of the attack */
    u8 sustain;
    u16 decay;
    u16 release;
    u8 unk_22; /* priority: allocation replaces the lowest */
    u8 pan;
    u16 volume; /* bits 0-7 hardware volume, bits 8-9 volume shift */
    u16 timer;
    SndChannelLfo lfo;
    s16 sweepPitch; /* pitch offset at the start of the sweep; it falls to 0 over sweepLength */
    s32 length;
    SndWaveParam wave;
    const void *data; /* PCM: the sample; PSG: the duty cycle */
    SndExChannelCallback callback;
    void *callbackArg;
    SndExChannel *next; /* next channel of the same track */
};

/* Locked channels: allocation never takes them. */
extern u32 data_arm7_0380806c;
/* Weakly locked channels: only an allocation marked strong may take them. Also the top of the driver thread's stack. */
extern u32 data_arm7_03808068;

/* Lock flags: bit 0 selects the weak lock. */
#define VOICE_RESERVE_SOFT 1

#endif
