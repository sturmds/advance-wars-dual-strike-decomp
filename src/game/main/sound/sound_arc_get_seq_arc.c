#include "types.h"
#include "game/sound_arc.h"

/* Result codes of the load routines; 0 means success. */
#define SOUND_FETCH_ERR_NO_PACK_ENTRY 3
#define SOUND_FETCH_ERR_PACK_READ 7

/* Bit of the load flags that asks for the item to be loaded; without it the
 * routine only looks for an item that is already in memory. */
#define SOUND_FETCH_WANT_SONG_PACK 8

const SoundArcSeqArcInfo* NNS_SndArcGetSeqArcInfo(int seqArcNo);
void* SoundArc_GetFileAddress(int fileId);
void* func_021072fc(int fileId, void* heap, int cache);

/* Gets sequence archive seqArcNo into memory, loading it into heap when
 * loadFlags asks for it. Stores the address in *loadedOut when that is not
 * NULL. Returns 0 on success, or a SOUND_FETCH_* code. */
int SoundArc_GetSeqArc(int seqArcNo, int loadFlags, void* heap, int cache,
                       void** loadedOut) {
  const SoundArcSeqArcInfo* info = NNS_SndArcGetSeqArcInfo(seqArcNo);
  void* loaded;

  if (info == NULL) {
    return SOUND_FETCH_ERR_NO_PACK_ENTRY;
  }
  if (loadFlags & SOUND_FETCH_WANT_SONG_PACK) {
    loaded = func_021072fc(info->fileId, heap, cache);
    if (loaded == NULL) {
      return SOUND_FETCH_ERR_PACK_READ;
    }
  } else {
    loaded = SoundArc_GetFileAddress(info->fileId);
  }
  if (loadedOut != NULL) {
    *loadedOut = loaded;
  }
  return 0;
}
