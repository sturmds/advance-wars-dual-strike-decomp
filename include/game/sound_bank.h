#ifndef GAME_SOUND_BANK_H
#define GAME_SOUND_BANK_H

#include "types.h"

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

#endif
