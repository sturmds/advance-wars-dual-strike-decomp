#include "types.h"
#include "sdk/snd.h"
#include "game/sound_bank.h"
#include "game/sound_wave_arc.h"

BOOL SoundWaveArc_LoadWave(SoundWaveArc *wave, u32 waveNo, u32 fileId, void *heap);

/* Loads into heap the waves that notes of the bank take from the wave archive
 * in slot waveArcSlot. Returns FALSE when there is no bank or a wave fails to
 * load. */
BOOL SoundArc_LoadBankWaves(SoundWaveArc *wave, void *bank, int waveArcSlot, u32 fileId, void *heap)
{
    SoundBankIter iter = SoundBank_StartIter();
    SoundBankNote note;

    if (bank == NULL) {
        return FALSE;
    }
    while (SoundBank_NextNote(bank, &note, &iter)) {
        if (note.type == SOUND_NOTE_WAVE && waveArcSlot == note.waveArcSlot) {
            if (!SoundWaveArc_LoadWave(wave, note.waveNo, fileId, heap)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
