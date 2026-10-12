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

/* Entry number index of the table at unk_18 in the current sound archive's
 * info block, or NULL when there is no such table or index is out of range.
 * Same shape as func_02106420. */
const void* SoundArc_GetPlayerGroupInfo(int index) {
  const SoundArcSeqInfoTable* table;

  table = InfoItem(gCurrentSoundArc->info, gCurrentSoundArc->info->unk_18);
  if (table == NULL) {
    return NULL;
  }
  if (index < 0) {
    return NULL;
  }
  if (index >= table->count) {
    return NULL;
  }
  return ArcInfoItem(gCurrentSoundArc, table->offset[index]);
}
