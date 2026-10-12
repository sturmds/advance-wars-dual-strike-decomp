#ifndef SDK_SND_SEQ_H
#define SDK_SND_SEQ_H

#include "types.h"

/* Low frequency oscillator settings. */
typedef struct SndLfo {
    u8 target; /* SND_LFO_PITCH, SND_LFO_VOLUME or SND_LFO_PAN */
    u8 speed;
    u8 depth;
    u8 range;
    u16 delay;
} SndLfo;

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
typedef struct SndExChannel SndExChannel;

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
    u8 pad_1c[8];
    u16 volume; /* bits 0-7 hardware volume, bits 8-9 volume shift */
    u8 pad_26[2];
    SndChannelLfo lfo;
    s16 sweepPitch; /* pitch offset at the start of the sweep; it falls to 0 over sweepLength */
    s32 length;
};

#endif
