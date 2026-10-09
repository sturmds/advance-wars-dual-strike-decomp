#ifndef GAME_SOUND_HANDLE_H
#define GAME_SOUND_HANDLE_H

#include "types.h"

/* NitroSystem sequence player (NNSSndSeqPlayer), only the fields the game
 * code reads. */
typedef struct SoundSeqPlayer {
    u8 pad_00[0x41];
    u8 volume;                         /* 0x41: set by NNS_SndPlayerSetVolume (NNS_SndPlayerSetVolume) */
} SoundSeqPlayer;

/* NitroSystem sound handle (NNSSndHandle): the player a sequence was
 * started on, NULL when nothing plays on it. */
typedef struct SoundHandle {
    SoundSeqPlayer *player;
} SoundHandle;

#endif
