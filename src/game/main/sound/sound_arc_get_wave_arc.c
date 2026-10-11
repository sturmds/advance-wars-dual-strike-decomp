#include "types.h"
#include "game/sound_arc.h"

/* Result codes of the load routines; 0 means success. */
#define SOUND_FETCH_ERR_NO_SAMPLE_PACK 5
#define SOUND_FETCH_ERR_SAMPLES_READ 9

/* Bit of the load flags. */
#define SOUND_FETCH_WANT_SAMPLES 4

const SoundArcWaveArcInfo *NNS_SndArcGetWaveArcInfo(int waveArcNo);
void *SoundArc_GetFileAddress(int fileId);
void *SoundArc_LoadWaveArcTable(int fileId, void *heap, int cache);
void *func_02107204(int fileId, void *heap, int cache);

/* Gets wave archive waveArcNo into memory. With SOUND_FETCH_WANT_SAMPLES in
 * loadFlags it loads the archive into heap (the on-demand kind through
 * SoundArc_LoadWaveArcTable); without, it only looks the archive up. Stores the
 * archive's address in *loadedOut when that is not NULL. Returns 0 on
 * success, or a SOUND_FETCH_* code. */
int SoundArc_GetWaveArc(int waveArcNo, int loadFlags, void *heap, int cache, void **loadedOut)
{
    const SoundArcWaveArcInfo *info = NNS_SndArcGetWaveArcInfo(waveArcNo);
    void *wave;

    if (info == NULL) {
        return SOUND_FETCH_ERR_NO_SAMPLE_PACK;
    }
    if (loadFlags & SOUND_FETCH_WANT_SAMPLES) {
        if (info->b.flags & 1) {
            wave = SoundArc_LoadWaveArcTable(info->id.fileId, heap, cache);
        } else {
            wave = func_02107204(info->id.fileId, heap, cache);
        }
        if (wave == NULL) {
            return SOUND_FETCH_ERR_SAMPLES_READ;
        }
    } else {
        wave = SoundArc_GetFileAddress(info->id.fileId);
    }
    if (loadedOut != NULL) {
        *loadedOut = wave;
    }
    return 0;
}
