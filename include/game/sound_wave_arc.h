#ifndef GAME_SOUND_WAVE_ARC_H
#define GAME_SOUND_WAVE_ARC_H

#include "types.h"

/* A wave archive file as loaded in memory (SWAR), only the fields the code
 * in src/ reads. The wave table has count entries with the address of each
 * wave once it is loaded (an offset from the file start before that),
 * followed by count entries with where each wave starts in the archive
 * file. */
struct SoundWaveArc;

/* Links a bank to a wave archive it uses. The bank holds four of them; an
 * archive chains the links of all banks that use it. */
typedef struct SoundWaveArcLink {
  struct SoundWaveArc* waveArc;
  struct SoundWaveArcLink* next;
} SoundWaveArcLink;

typedef struct SoundWaveArc {
  u8 pad_00[8];
  u32 fileSize; /* 0x08 */
  u8 pad_0c[0x18 - 0x0c];
  SoundWaveArcLink* firstLink; /* 0x18 */
  u8 pad_1c[0x38 - 0x1c];
  u32 count;   /* 0x38 */
  u32 table[]; /* 0x3c, 2 * count entries; sizeof covers the header only */
} SoundWaveArc;

/* Bytes before the wave table. An int: with sizeof's unsigned type, mwccarm
 * computes header size + table size once for both the read and its check in
 * SoundArc_LoadWaveArcTable, where the original computes it twice. */
#define SOUND_WAVE_ARC_HEADER_SIZE ((int)sizeof(SoundWaveArc))

#endif
