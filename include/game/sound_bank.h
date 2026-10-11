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

#define SOUND_NOTE_WAVE 1              /* note plays a wave from a wave archive */

/* One note of a bank instrument, 12 bytes as the bank walker returns it. */
typedef struct SoundBankNote {
    u8 type;                           /* SOUND_NOTE_* */
    u8 unk_01;
    u16 waveNo;                        /* wave number inside the wave archive */
    u16 waveArcSlot;                   /* which of the bank's four wave archives */
    u16 unk_06;
    u16 unk_08;
    u16 unk_0a;
} SoundBankNote;

/* A bank file as loaded in memory (SBNK), only the fields the code in src/
 * reads. instruments[i] holds the instrument type in its low byte and the
 * offset of its note data from the file start in the upper 24 bits. */
typedef struct SoundBank {
    u8 pad_00[0x18];
    SoundWaveArcLink waveArcs[4];      /* 0x18 */
    u32 instrumentCount;               /* 0x38 */
    u32 instruments[1];                /* 0x3c */
} SoundBank;

#endif
