#ifndef GAME_SOUND_STREAM_H
#define GAME_SOUND_STREAM_H

#include "types.h"
#include "game/sound_handle.h"

/* Flag bits of SoundStream.flags. Signed one-bit fields: the original code
 * reads them with shift and arithmetic shift. */
typedef struct SoundStreamFlags {
    s32 fileOpen : 1;                  /* bit 0: the stream file is open */
    s32 playing : 1;                   /* bit 1: started on the hardware channels */
    s32 bit2 : 1;
    s32 stopping : 1;                  /* bit 3: fading out, closes when the fade ends */
} SoundStreamFlags;

/* NitroSystem sound stream (NNSSndStrm), only the fields the game code
 * reads. */
typedef struct SoundStream {
    u8 pad_00[0xe4];
    SoundFader volumeFader;            /* 0xe4 */
    u8 pad_f4[0x18];
    SoundStreamFlags flags;            /* 0x10c */
    u8 pad_110[0x3c];
    u32 unk_14c;
} SoundStream;

#endif
