#include "sdk/snd.h"

/* Empty stubs; their definitions take an int that no caller passes. */
void func_arm7_037fee6c(void);
void func_arm7_037fee70(void);

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
    func_arm7_037fee70();
    if (programNo >= bank->instrumentCount) {
        func_arm7_037fee6c();
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
            func_arm7_037fee6c();
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
                func_arm7_037fee6c();
                return FALSE;
            }
        }
        *note = ((SoundBankNote *)(data + 8))[i];
        break;
    case 0: /* needed to match: makes the switch one jump table over 0-17 */
    default:
        func_arm7_037fee6c();
        return FALSE;
    }
    func_arm7_037fee6c();
    return TRUE;
}
