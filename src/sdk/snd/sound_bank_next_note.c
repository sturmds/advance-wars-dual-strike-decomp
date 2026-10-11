#include "types.h"
#include "sdk/snd.h"

/* Instrument types, from the low byte of a bank's instrument entry. Types 1
 * to 5 are one note each; 16 is a drum set (one note per key in a range) and
 * 17 a key split with up to 8 regions. */
#define INST_TYPE_DRUM_SET  16
#define INST_TYPE_KEY_SPLIT 17

/* The five halfwords of a note after its type. Needed to match: the original
 * copies them as one struct, which pairs the loads and stores. */
typedef struct SoundBankWaveParams {
    u16 v[5];
} SoundBankWaveParams;

/* Returns the next note of the bank after the position in iter and moves iter
 * past it, or FALSE when no notes are left. The loops that always return on
 * their first pass are needed to match. */
BOOL SoundBank_NextNote(SoundBank *bank, SoundBankNote *note, SoundBankIter *iter)
{
    u32 entry;
    u8 *data;

    while (iter->instrument < bank->instrumentCount) {
        entry = bank->instruments[iter->instrument];
        note->type = entry;
        switch (note->type) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            *(SoundBankWaveParams *)&note->waveNo = *(SoundBankWaveParams *)((u8 *)bank + (entry >> 8));
            iter->instrument++;
            return TRUE;
        case INST_TYPE_DRUM_SET:
            /* data[0] and data[1] are the lowest and highest key. */
            data = (u8 *)bank + (entry >> 8);
            while (iter->note < data[1] - data[0] + 1) {
                *note = ((SoundBankNote *)(data + 2))[iter->note];
                iter->note++;
                return TRUE;
            }
            break;
        case INST_TYPE_KEY_SPLIT:
            /* One byte per region, 0 for an unused one, then the notes. */
            data = (u8 *)bank + (entry >> 8);
            while (iter->note < 8) {
                if (data[iter->note] == 0) {
                    break;
                }
                *note = ((SoundBankNote *)(data + 8))[iter->note];
                iter->note++;
                return TRUE;
            }
            break;
        }
        iter->instrument++;
        iter->note = 0;
    }
    return FALSE;
}
