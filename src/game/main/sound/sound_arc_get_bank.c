#include "types.h"
#include "game/sound_arc.h"

/* Result codes of the load routines; 0 means success. */
#define SOUND_FETCH_ERR_NO_INST_ENTRY     4
#define SOUND_FETCH_ERR_NO_SAMPLE_PACK 5
#define SOUND_FETCH_ERR_INST_READ 8
#define SOUND_FETCH_ERR_SAMPLES_READ 9

/* Bits of the load flags. */
#define SOUND_FETCH_WANT_INST_SET      2
#define SOUND_FETCH_WANT_SAMPLES 4

const SoundArcBankInfo *NNS_SndArcGetBankInfo(int bankNo);
const SoundArcWaveArcInfo *NNS_SndArcGetWaveArcInfo(int waveArcNo);
void *func_02106304(int fileId);
void *func_02107280(int fileId, void *heap, int cache);
int func_021074a8(int waveArcNo, int loadFlags, void *heap, int cache, void **loadedOut);
BOOL func_02106d7c(void *wave, void *bank, int index, u32 fileId, void *heap);
void func_02110d7c(void *bank, int index, void *wave);

/* Gets bank bankNo into memory, loading it and its wave archives into heap
 * as loadFlags asks, and links each wave archive to the bank. Stores the
 * bank's address in *loadedOut when that is not NULL. Returns 0 on success,
 * or a SOUND_FETCH_* code. */
int SoundArc_GetBank(int bankNo, int loadFlags, void *heap, int cache, void **loadedOut)
{
    const SoundArcBankInfo *info = NNS_SndArcGetBankInfo(bankNo);
    void *bank;
    int i;
    int loadWaveData;

    if (info == NULL) {
        return SOUND_FETCH_ERR_NO_INST_ENTRY;
    }
    if (loadFlags & SOUND_FETCH_WANT_INST_SET) {
        bank = func_02107280(info->fileId, heap, cache);
        if (bank == NULL) {
            return SOUND_FETCH_ERR_INST_READ;
        }
    } else {
        bank = func_02106304(info->fileId);
    }
    loadWaveData = loadFlags & SOUND_FETCH_WANT_SAMPLES;
    for (i = 0; i < 4; i++) {
        const SoundArcWaveArcInfo *waveInfo;
        void *wave;
        int result;

        if (info->waveArc[i] == SOUND_ARC_NO_WAVE_ARC) {
            continue;
        }
        waveInfo = NNS_SndArcGetWaveArcInfo(info->waveArc[i]);
        if (waveInfo == NULL) {
            return SOUND_FETCH_ERR_NO_SAMPLE_PACK;
        }
        result = func_021074a8(info->waveArc[i], loadFlags, heap, cache, &wave);
        if (result != 0) {
            return result;
        }
        if ((waveInfo->b.flags & 1) && loadWaveData) {
            if (!func_02106d7c(wave, bank, i, waveInfo->id.fileId, heap)) {
                return SOUND_FETCH_ERR_SAMPLES_READ;
            }
        }
        if (bank != NULL && wave != NULL) {
            func_02110d7c(bank, i, wave);
        }
    }
    if (loadedOut != NULL) {
        *loadedOut = bank;
    }
    return 0;
}
