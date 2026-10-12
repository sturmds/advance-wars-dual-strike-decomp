#ifndef GAME_SOUND_BANK_H
#define GAME_SOUND_BANK_H

#include "types.h"
#include "game/sound_wave_arc.h"

/* Position in a sound bank while walking its notes: the instrument and the
 * note inside it. */
typedef struct SoundBankIter {
  u32 instrument;
  u32 note;
} SoundBankIter;

#define SOUND_NOTE_WAVE 1 /* note plays a wave from a wave archive */
#define SOUND_NOTE_PSG 2  /* square wave; waveNo holds the duty */
#define SOUND_NOTE_NOISE 3
#define SOUND_NOTE_DIRECT_WAVE \
  4 /* waveNo and waveArcSlot hold the wave's address, low half first */

/* One note of a bank instrument, 12 bytes as the bank walker returns it. */
typedef struct SoundBankNote {
  u8 type; /* SOUND_NOTE_* */
  u8 unk_01;
  u16 waveNo;      /* wave number inside the wave archive */
  u16 waveArcSlot; /* which of the bank's four wave archives */
  u8 originalKey;  /* key the wave was recorded at */
  u8 attack;
  u8 decay;
  u8 sustain;
  u8 release;
  u8 pan; /* 0 to 127, 64 is the centre */
} SoundBankNote;

/* Instrument types, from the low byte of a bank's instrument entry. Types 1
 * to 5 are one note each; 16 is a drum set (one note per key in a range) and
 * 17 a key split with up to 8 regions. */
#define INST_TYPE_DRUM_SET 16
#define INST_TYPE_KEY_SPLIT 17
#define INST_KEY_SPLIT_REGIONS 8

/* The five halfwords of a note after its type. Needed to match: the bank
 * readers copy them as one struct, which pairs the loads and stores. */
typedef struct SoundBankWaveParams {
  u16 v[5];
} SoundBankWaveParams;

/* A bank file as loaded in memory (SBNK), only the fields the code in src/
 * reads. instruments[i] holds the instrument type in its low byte and the
 * offset of its note data from the file start in the upper 24 bits. */
typedef struct SoundBank {
  u8 pad_00[0x18];
  SoundWaveArcLink waveArcs[4]; /* 0x18 */
  u32 instrumentCount;          /* 0x38 */
  u32 instruments[1];           /* 0x3c */
} SoundBank;

#endif
