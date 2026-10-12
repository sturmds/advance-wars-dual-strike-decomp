#include "sdk/snd.h"

/* Empty stubs; their definitions take an int that no caller passes. */

/* Writes the note that program programNo plays for key into note. FALSE when
 * the program or key has none. */
BOOL SndArm7_ReadInstData(SoundBank *bank, int programNo, int key, SoundBankNote *note)
{
    u32 entry;
    u8 *data;
    int i;
    int lowestKey;
    int highestKey;

    if (programNo < 0) {
        return FALSE;
    }
    SNDi_LockMutex();
    if (programNo >= bank->instrumentCount) {
        SNDi_UnlockMutex();
        return FALSE;
    }
    entry = bank->instruments[programNo];
    note->type = entry;
    switch (note->type) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        *(SoundBankWaveParams *)&note->waveNo = *(SoundBankWaveParams *)((u8 *)bank + (entry >> 8));
        break;
    case INST_TYPE_DRUM_SET:
        data = (u8 *)bank + (entry >> 8);
        highestKey = data[1];
        lowestKey = data[0];
        if (key < lowestKey || key > highestKey) {
            SNDi_UnlockMutex();
            return FALSE;
        }
        *note = ((SoundBankNote *)(data + 2))[key - lowestKey];
        break;
    case INST_TYPE_KEY_SPLIT:
        /* The highest key of each region, then the notes. */
        i = 0;
        data = (u8 *)bank + (entry >> 8);
        while (key > data[i]) {
            i++;
            if (i >= INST_KEY_SPLIT_REGIONS) {
                SNDi_UnlockMutex();
                return FALSE;
            }
        }
        *note = ((SoundBankNote *)(data + 8))[i];
        break;
    case 0: /* needed to match: makes the switch one jump table over 0-17 */
    default:
        SNDi_UnlockMutex();
        return FALSE;
    }
    SNDi_UnlockMutex();
    return TRUE;
}
