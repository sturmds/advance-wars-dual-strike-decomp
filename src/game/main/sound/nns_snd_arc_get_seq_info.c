#include "types.h"
#include "game/sound_arc.h"

extern SoundArc* gCurrentSoundArc;

/* Pointer to the item at offset inside the info block, NULL for offset 0. */
static inline const void* InfoItem(const SoundArcInfo* info, u32 offset) {
  if (offset == 0) {
    return NULL;
  }
  return (const u8*)info + offset;
}

/* Same as InfoItem, reading the info block from arc. Needed to match: the
 * second lookup reloads arc->info after the offset instead of reusing it. */
static inline const void* ArcInfoItem(const SoundArc* arc, u32 offset) {
  const u8* info = (const u8*)arc->info;

  if (offset == 0) {
    return NULL;
  }
  return info + offset;
}

/* NitroSystem NNS_SndArcGetSeqInfo: the info entry of sequence seqNo in
 * the current sound archive, or NULL when the archive has no sequence
 * table or seqNo is out of range. */
const SoundArcSeqInfo* NNS_SndArcGetSeqInfo(int seqNo) {
  const SoundArcSeqInfoTable* table;

  table = InfoItem(gCurrentSoundArc->info, gCurrentSoundArc->info->seqOffset);
  if (table == NULL) {
    return NULL;
  }
  if (seqNo < 0) {
    return NULL;
  }
  if (seqNo >= table->count) {
    return NULL;
  }
  return ArcInfoItem(gCurrentSoundArc, table->offset[seqNo]);
}
