#ifndef GAME_UNK_022A8A10_H
#define GAME_UNK_022A8A10_H

#include "types.h"
#include "game/sound_handle.h"

/* State of one running sound output, at data_022a8a10 (main bss). Only the
 * fields the code in src/ uses. */
typedef struct Unk_022a8a10 {
    int running;                       /* 0x00: nonzero while started */
    int unk_04;                        /* 0x04: nonzero stops the per-frame volume update */
    u8 pad_08[0x18];
    int unk_20;                        /* 0x20: first argument of the volume call */
    u8 pad_24[0x14];
    SoundFader fader;                  /* 0x38 */
    int closeWhenFaded;                /* 0x48: close the output once the fader ends */
    int appliedVolume;                 /* 0x4c: last volume sent to the hardware */
} Unk_022a8a10;

#endif
