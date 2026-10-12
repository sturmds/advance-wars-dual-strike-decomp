#include "types.h"
#include "game/sound_arc.h"

/* Result codes of the load routines; 0 means success. */
#define SOUND_FETCH_ERR_NO_SONG_ENTRY 2
#define SOUND_FETCH_ERR_SONG_READ 6

/* Bit of the load flags that asks for the sequence file to be loaded. */
#define SOUND_FETCH_WANT_SONG 1

int SoundArc_GetBank(int bankNo, int loadFlags, void* heap, int cache,
                     void** loadedOut);
const SoundArcSeqInfo* NNS_SndArcGetSeqInfo(int seqNo);
void* SoundArc_GetFileAddress(int fileId);
void* func_02107378(int fileId, void* heap, int cache);

/* Gets sequence seqNo into memory, loading it into heap when loadFlags asks
 * for it, and gets its bank the same way. Stores the sequence address in
 * *loadedOut when that is not NULL. Returns 0 on success, or a SOUND_FETCH_*
 * code. */
int SoundArc_GetSeq(int seqNo, int loadFlags, void* heap, int cache,
                    void** loadedOut) {
  const SoundArcSeqInfo* info = NNS_SndArcGetSeqInfo(seqNo);
  int result;
  void* loaded;

  if (info == NULL) {
    return SOUND_FETCH_ERR_NO_SONG_ENTRY;
  }
  result = SoundArc_GetBank(info->bankNo, loadFlags, heap, cache, NULL);
  if (result != 0) {
    return result;
  }
  if (loadFlags & SOUND_FETCH_WANT_SONG) {
    loaded = func_02107378(info->fileId, heap, cache);
    if (loaded == NULL) {
      return SOUND_FETCH_ERR_SONG_READ;
    }
  } else {
    loaded = SoundArc_GetFileAddress(info->fileId);
  }
  if (loadedOut != NULL) {
    *loadedOut = loaded;
  }
  return 0;
}
