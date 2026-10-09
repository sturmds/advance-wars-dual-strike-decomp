#ifndef GAME_SOUND_HANDLE_H
#define GAME_SOUND_HANDLE_H

#include "types.h"
#include "game/link_list.h"

/* Ramps a value to a target over a number of frames (SNDi fader). */
typedef struct SoundFader {
    s32 start;
    s32 target;
    u32 elapsed;
    u32 frames;
} SoundFader;

/* SoundSeqPlayer.state */
#define SEQ_PLAYER_FREE     0
#define SEQ_PLAYER_STOPPING 2  /* fading out, stops when the fader ends */

/* One of the sequence player groups of the sound system, 0x24 bytes each
 * (array at data_022a84a8). A group holds the sequence players started on
 * it, sorted by priority, and has its own volume. */
typedef struct SoundPlayerSlot {
    LinkList players;                  /* 0x00 */
    u8 pad_0c[0x14];
    u8 volume;                         /* 0x20: index into the volume table */
    u8 pad_21[3];
} SoundPlayerSlot;

/* NitroSystem sequence player (NNSSndSeqPlayer), only the fields the game
 * code reads. 0x44 bytes each. */
typedef struct SoundSeqPlayer {
    u8 pad_00[4];
    SoundPlayerSlot *ownerSlot;        /* 0x04: group this player sits in, or NULL */
    u8 pad_08[0x14];
    SoundFader volumeFader;            /* 0x1c */
    u8 state;                          /* 0x2c */
    u8 started;                        /* 0x2d: set once the first note has begun */
    u8 pad_2e[2];
    u32 startTick;                     /* 0x30: tick passed to the has-passed check */
    u8 pad_34[8];
    u8 hwChannel;                      /* 0x3c: hardware channel number */
    u8 priority;                       /* 0x3d: the player lists are sorted by it */
    s16 hwVolume;                      /* 0x3e: last volume sent to the hardware channel */
    u8 seqVolume;                      /* 0x40: volume of the archive entry */
    u8 volume;                         /* 0x41: set by NNS_SndPlayerSetVolume (NNS_SndPlayerSetVolume) */
    u8 pad_42[2];
} SoundSeqPlayer;

/* NitroSystem sound handle (NNSSndHandle): the player a sequence was
 * started on, NULL when nothing plays on it. */
typedef struct SoundHandle {
    SoundSeqPlayer *player;
} SoundHandle;

/* Stops player now, or fades it out over fadeFrames frames. */
void SoundSeqPlayer_Stop(SoundSeqPlayer *player, int fadeFrames);

#endif
