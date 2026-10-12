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

/* A hardware channel as the driver's channel allocator sees it. Only the fields the code read so far are named. */

struct SndExChannel {
    u8 pad_00[2];
    u8 envStatus;
    u8 active : 1;
    u8 start : 1;
    u8 autoSweep : 1; /* advance the sweep on each periodic update */
    u8 unk_03_3 : 5;
    u8 pad_04[0xc];
    s32 envAttenuation; /* envelope level, in 1/128 steps of SND_CalcChannelVolume's volume; -723 * 128 is silent */
    s32 sweepCounter;
    s32 sweepLength;
    u8 pad_1c[0x22 - 0x1c];
    u8 unk_22;
    u16 volume; /* bits 0-7 hardware volume, bits 8-9 volume shift */
    u8 pad_26[2];
    SndChannelLfo lfo;
    s16 sweepPitch; /* pitch offset at the start of the sweep; it falls to 0 over sweepLength */
    s32 length;
    u8 pad_38[0x50 - 0x38];
    SndExChannel *next; /* next channel of the same track */
};

#endif
