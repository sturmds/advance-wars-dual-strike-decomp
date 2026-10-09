#ifndef GAME_SOUND_HANDLE_H
#define GAME_SOUND_HANDLE_H

#include "types.h"

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

/* NitroSystem sequence player (NNSSndSeqPlayer), only the fields the game
 * code reads. 0x44 bytes each. */
typedef struct SoundSeqPlayer {
    u8 pad_00[0x1c];
    SoundFader volumeFader;            /* 0x1c */
    u8 state;                          /* 0x2c */
    u8 pad_2d[0x0f];
    u8 hwChannel;                      /* 0x3c: hardware channel number */
    u8 pad_3d[4];
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
