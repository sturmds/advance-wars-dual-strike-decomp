#ifndef GAME_SOUND_WAVE_ARC_H
#define GAME_SOUND_WAVE_ARC_H

#include "types.h"

/* A wave archive file as loaded in memory (SWAR), only the fields the code
 * in src/ reads. The wave table has count entries with the address of each
 * wave once it is loaded (an offset from the file start before that),
 * followed by count entries with where each wave starts in the archive
 * file. */
typedef struct SoundWaveArc {
    u8 pad_00[8];
    u32 fileSize;                      /* 0x08 */
    u8 pad_0c[0x38 - 0x0c];
    u32 count;                         /* 0x38 */
    u32 table[1];                      /* 0x3c */
} SoundWaveArc;

/* Bytes before the wave table. */
#define SOUND_WAVE_ARC_HEADER_SIZE 0x3c

#endif
