#ifndef SDK_SND_HW_H
#define SDK_SND_HW_H

#include "types.h"

/* Sound hardware registers the ARM7 driver writes. */

/* High byte of SOUNDCNT: bits 0-1 left output, 2-3 right output, 4 and 5 channel 1 and 3 direct output, 7 master enable. */
#define AUDIO_MIXER_ROUTE_BYTE (*(u8 *)0x4000501)
#define AUDIO_MIXER_MASTER_ON 0x80

/* POWCNT2: bit 0 powers the speakers and sound circuit. */
#define AUDIO_SPEAKER_POWER_REG (*(u16 *)0x4000304)

/* Sound capture control registers; bit 7 starts a capture. */
#define AUDIO_RECORD0_CTRL (*(u8 *)0x4000508)
#define AUDIO_RECORD1_CTRL (*(u8 *)0x4000509)

#define AUDIO_CH_TOTAL 16

int SndHw_ScaleVolumeForPan(int volume, int pan);

/* Strength of the volume drop near the left and right edges of the pan range (0 = none, 0x7FFF = most). */
extern int gSndHwPanEdgeAttenuation;
/* Pan and volume last requested for each channel. */
extern u8 gSndHwChannelPan[AUDIO_CH_TOTAL];
extern u8 gSndHwChannelVolume[AUDIO_CH_TOTAL];
/* When not negative, this pan replaces every channel pan. */
extern int gSndHwPanOverride;

/* One channel register block at 0x04000400 + channel * 0x10. */
typedef struct SndHwChannel {
    u8 volume;
    u8 volumeShift; /* bits 0-1 volume divider, bit 7 hold */
    u8 pan;
    u8 flags;       /* bits 3-4 repeat mode, 5-6 format, bit 7 busy and start */
    u32 source;
    u16 timer;
    u16 loopStart;
    u32 length;
} SndHwChannel;

#define AUDIO_CH_REG_BLOCKS ((SndHwChannel *)0x4000400)
/* Byte registers of the channel whose block starts blockOffset bytes (channel << 4) from the first. */
#define AUDIO_CH_LOUDNESS_BYTE(blockOffset) (*(u8 *)(0x4000400 + (blockOffset)))
#define AUDIO_CH_BALANCE_BYTE(blockOffset) (*(u8 *)(0x4000402 + (blockOffset)))
#define AUDIO_CH_LOUDNESS_DIVIDER(blockOffset) (*(u16 *)(0x4000400 + (blockOffset)))
#define AUDIO_CH_PLAYING_FLAG 0x80

/* Channels 1 and 3 can feed the mixer directly, so volume scaling skips them. */
#define AUDIO_CH_PAN_SCALE_MASK 0xFFF5

#endif
