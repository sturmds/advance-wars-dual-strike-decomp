#ifndef GAME_SOUND_STREAM_H
#define GAME_SOUND_STREAM_H

#include "types.h"
#include "game/sound_handle.h"

/* Flag bits of SoundStream.flags. Signed one-bit fields: the original code
 * reads them with shift and arithmetic shift. */
typedef struct SoundStreamFlags {
    s32 fileOpen : 1;                  /* bit 0: the stream file is open */
    s32 playing : 1;                   /* bit 1: started on the hardware channels */
    s32 startPending : 1;              /* bit 2: start playing once the data is ready */
    s32 stopping : 1;                  /* bit 3: fading out, closes when the fade ends */
} SoundStreamFlags;

/* NitroSystem sound stream (NNSSndStrm), only the fields the game code
 * reads. */
typedef struct SoundStream {
    u8 pad_00[0xe4];
    SoundFader volumeFader;            /* 0xe4 */
    u8 pad_f4[0x18];
    SoundStreamFlags flags;            /* 0x10c */
    int unk_110;                       /* 0: close at once */
    int unk_114;                       /* nonzero when the data is ready */
    u8 pad_118[0x34];
    u32 unk_14c;
    int volumeIndex;                   /* 0x150: index into the volume table */
    int appliedVolume;                 /* 0x154: last volume sent to the hardware */
    u8 pad_158[4];
} SoundStream;

#define SOUND_STREAM_COUNT 4

extern SoundStream data_022a9300[SOUND_STREAM_COUNT];

#endif
